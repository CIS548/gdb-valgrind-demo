# gdb-valgrind-tutorial

This is a set of buggy programs to demonstrate how gdb and valgrind are super duper useful.  This recitation has been/will be recorded, but we are providing the source code and walkthrough notes.

## Author

[Antonio Esparza](https://github.com/alesparza), a 595 TA (but mostly a debugger)

## Introduction

gdb is a great tool for debugging, but it does require some time exploring the basic capabilities to get used to it.  I do highly recommend running in the text user interface mode

Valgrind is a set of utilities that analyze code for errors.  We will be focusing on the Memcheck tool for this course.  Memcheck allows us to inspect memory usage to find bug related to memory management, such as uninitialised variables, invalid read/write errors, and memory leaks

## How this works

You can follow along in this recitation by loading the appropriate commits with `git checkout scenario-<num>`.  Each scenario features a program with bugs

We'll start with scenario-0, which is an introduction to gdb, followed by scenario-1 which is more complicated and introduces valgrind, followed by a more examples in scenario-2.

## Scenario-0

Load the code for scenario-0 and compile it

```
git checkout scenario-0
clang my_program.c -o my_program
```

You can also run `make-scenario-0` which will do all the required steps for you

```
make scenario-0 # automagically checkout the correct commit and compile
```

Let's try running the program and see what happens.

```
./my_program
argc = 1
argv[0] = ./my_program
Hello World
Segmentation fault
```

Yikes, we just got started and already have a segmentation fault.  Let's start up gdb and walk through the program.

```
gdb ./my_program
GNU gdb (Ubuntu 9.2-0ubuntu1~20.04.1) 9.2
Copyright (C) 2020 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from ./my_program...
(gdb) 
```

That's kind of busy.  I don't even know what's going on.  Here is the first trick: telling gdb to be quiet.  First, exit gdb:

```
(gdb) quit
```

Then start gdb in quiet mode.

```
gdb -q ./my_program # -q is the 'quiet' flag
Reading symbols from ./my_program...
(gdb) 
```

This is great, but it's all in the command line and we have to keep switching windows to look at the code.  Let's put it all into one place by using the Text Interface Mode.

```
(gdb) quit
gdb -q -tui ./my_program # -tui enables the Text User Interface
Reading symbols from ./my_program...
(No debugging symbols found in ./my_program)
(gdb) 
```

Cool, now we can see our source code as we debug... except it says there is No Source Available in the top panel.  Most likely this means that we did not use the `-g` flag when compiling.  I suggest always compiling with debugging symbols enabled because we'll almost certainly need to debug at some point.

We can go into the makefile and add the `-g` flag, or we can checkout a commit that already does this for us.  Again, the makefile will do this for us automagically.

```
(gdb) q
make scenario-0.1
```

Now when we run gdb, we'll see the source code!

```
gdb -q -tui ./my_program
Reading symbols from ./my_program...
(gdb)
```

Let's start the program with `start`.

```
(gdb) start
Temporary breakpoint 1 at 0x4011af: file my_program.c, line 13.
Starting program: /workspace/gdb-valgrind-demo/my_program
warning: Error disabling address space randomization: Operation not permitted

Temporary breakpoint 1, main (argc=1, argv=0x7ffc3c7b6298) at my_program.c:13
(gdb) 
```

We just started the program and we're already getting errors!?  Not exactly.  The warning here is about disabling address space randomization.  As a security feature (take 551 for more details), gdb will attempt to disable address space randomization, meaning that it will try to load the program into the same memory location for each debugging session.  gdb is reporting that it was unable to do so (which means your memory addresses may differ).  

This is not going to be an issue for any of the projects we're working on, so let's walk through the program with `next` and try to find the segmentation fault.

```
(gdb) next
(gdb) next
(gdb) next
(gdb) next  

Program received signal SIGSEGV, Segmentation fault.
0x0000000000401175 in printString (string=0x0) at my_program.c:9
```

Okay, gdb is now reporting that the program received a SEGSEGV (illegal memory access signal) due to a segmentation fault.  gdb has paused the program but if we attempt to continue walking through the program, the signal will pass to the program and the process will terminate.

We can see that the segmentation fault appears to be at line 9, which is in the `printString` function.  

At this point, you may notice that the display has gotten a little corrupted.  This happens when your program writes to stdout.  Easily fixed with the `refresh` command

```
(gdb) refresh)
```

What we can do now is inspect the variables and try to determine what happened in the program.  Since we're trying to print a character, we could start by printing `string[0]`

```
(gdb) print string[0]
Cannot access memory at address 0x0
```

Hmmm... so we seem to have a NULL pointer, and dereferencing it is the cause of the segmentation fault.  We can check if string is indeed a NULL pointer:

```
(gdb) print string
$1 = 0x0
```

Yep, that's a problem.

Let's check the stack trace and go back to the main function

```
(gdb) where
#0  0x0000000000401175 in printString (string=0x0) at my_program.c:9
#1  0x00000000004011f3 in main (argc=1, argv=0x7ffc3c7b6298)
    at my_program.c:16
(gdb) frame 1
#1  0x00000000004011f3 in main (argc=1, argv=0x7ffc3c7b6298)
    at my_program.c:16
(gdb) print argv[1]
$2 = 0x0
```

It appears that this program was expecting an argument, but failed to provide any checks in case it was not provided.  I think we can close gdb at this point.  Let's go and update the code to include some kind of check, which we can do by checking out scenario-0.2.

```
(gdb) quit
A debugging session is active.

        Inferior 1 [process 12147] will be killed.

Quit anyway? (y or n) y
make scenario-0.2
```

While we could just test the program, let's use gdb to watch the program execute.  We'll set a breakpoint on line 17 with the `break` before the conditional block executes, allow the program to run up to the breakpoint with `continue`.  Note that to step-into a function, we use the `step` command.

```
gdb -q -tui ./my_program
Reading symbols from ./my_program...
(gdb) break 17
Breakpoint 1 at 0x4011ed: file my_program.c, line 17.
(gdb) start
Temporary breakpoint 2 at 0x4011b6: file my_program.c, line 13.
Starting program: /workspace/gdb-valgrind-demo/my_program
warning: Error disabling address space randomization: Operation not permitted

Temporary breakpoint 2, main (argc=1, argv=0x7ffc987788c8) at my_program.c:13
(gdb) print argc
$1 = 1
(gdb) next
```

Indeed, we have skipped over the troublesome function.

Let's check that it works when we do include an argument.  Note the use of the `--args` flag, which tells gdb that everything following is part of the program input.  Also, we can set a breakpoint by providing the function name directly.

```
gdb -q -tui --args ./my_program test
Reading symbols from ./my_program...
(gdb) break printString
Breakpoint 1 at 0x40115c: file my_program.c, line 8.
(gdb) start
Starting program: /workspace/gdb-valgrind-demo/my_program test
warning: Error disabling address space randomization: Operation not permitted

Temporary breakpoint 2, main (argc=2, argv=0x7ffea87e5928) at my_program.c:13
(gdb) continue
Continuing.
argc = 2
argv[0] = /workspace/gdb-valgrind-demo/my_program
Hello World

Breakpoint 1, printString (string=0x7ffea87e5c2b "test") at my_program.c:8
(gdb) print string
$1 = 0x7ffea87e5c2b "test"
```

This time, we entered the function and the string pointer points to the string "test", as we expect.

## Scenario-1

### A bug

This program is a bit more complicated and has some more difficult bugs.  Checkout `scenario-1` and run it.  We'll use MCIT as our group name, and start out with a short trip of three km.

```
make scenario-1
./roadtrip
Roadtrip!
Group name? MCIT
How many kilometres should we go?
3
We've gone 0 km
        Still in the parking lot...

We've gone 0 km
        Still in the parking lot...

We've gone 0 km
        Still in the parking lot...

We've gone 0 km
        Still in the parking lot...

We've g^C
```

Erm.... well that's a problem.  We can't seem to get out of the parking lot, even though we said we wanted to take a 3 km trip.  This seems like an infinity loop.

Now, I understand most people here would just open the code and look for the issue, but I'd like you to pretend for a moment that we're inside a large codebase where we can't just insert print statements everywhere for debugging purposes.  This is why builds compile with the `-g` flag for development and without it for production builds.

Let's start gdb and just let it run.  When we hit the infinite loop, we can send an interrupt signal and see where the loop appears to be stuck.  If you've looked at the code, you could also set a break point at a likely location.

```
gdb -q -tui ./roadtrip
Reading symbols from ./roadtrip...
(gdb) start
Temporary breakpoint 1 at 0x40159f: file roadtrip.c, line 104.
Starting program: /workspace/gdb-valgrind-demo/roadtrip
warning: Error disabling address space randomization: Operation not permitted

Temporary breakpoint 1, main (argc=1, argv=0x7ffed5416bb8) at roadtrip.c:104
(gdb) continue 
Continuing.

Roadtrip!
Group name? MCIT
How many kilometres should we go?
3
We've gone 0 km
        Still in the parking lot...

We've gone 0 km
        Still in the parking lot...

We've g^C
Program received signal SIGINT, Interrupt.
0x00007f3a16fc0077 in __GI___libc_write (fd=1, buf=0x22a82a0, nbytes=45)
    at ../sysdeps/unix/sysv/linux/write.c:26
../sysdeps/unix/sysv/linux/write.c: No such file or directory.
(gdb) 
```

Your results may vary depending on where you stopped, but in one run, I was inside printf and the stack trace had a lot of library function calls.  So pick the frame that has the last one with your program code.

```
(gdb) where
#0  0x00007f3a16fc0077 in __GI___libc_write (fd=1, buf=0x22a82a0, nbytes=45)
    at ../sysdeps/unix/sysv/linux/write.c:26
#1  0x00007f3a16f40e8d in _IO_new_file_write (
    f=0x7f3a1709f6a0 <_IO_2_1_stdout_>, data=0x22a82a0, n=45)
    at fileops.c:1176
#2  0x00007f3a16f42951 in new_do_write (to_do=45,
    data=0x22a82a0 "We've gone 0 km\n\tStill in the parking lot...\n",
    fp=0x7f3a1709f6a0 <_IO_2_1_stdout_>) at libioP.h:948
#3  _IO_new_do_write (to_do=45,
    data=0x22a82a0 "We've gone 0 km\n\tStill in the parking lot...\n",
    fp=0x7f3a1709f6a0 <_IO_2_1_stdout_>) at fileops.c:426
#4  _IO_new_do_write (fp=0x7f3a1709f6a0 <_IO_2_1_stdout_>,
    data=0x22a82a0 "We've gone 0 km\n\tStill in the parking lot...\n",
    to_do=45) at fileops.c:423
#5  0x00007f3a16f416b5 in _IO_new_file_xsputn (n=45, data=<optimized out>,
    f=<optimized out>) at libioP.h:948
#6  _IO_new_file_xsputn (f=0x7f3a1709f6a0 <_IO_2_1_stdout_>,
    data=<optimized out>, n=45) at fileops.c:1197
#7  0x00007f3a16f28972 in __vfprintf_internal (
    s=0x7f3a1709f6a0 <_IO_2_1_stdout_>,
    format=0x402054 "We've gone 0 km\n\tStill in the parking lot...\n",
    ap=ap@entry=0x7ffed5416970, mode_flags=mode_flags@entry=0)
#8  0x00007f3a16f13d3f in __printf (format=<optimized out>) at printf.c:33
#9  0x00000000004011fa in travel () at roadtrip.c:28
#10 0x0000000000401617 in main (argc=1, argv=0x7ffed5416bb8) at roadtrip.c:121
(gdb) frame 9
#9  0x00000000004011fa in travel () at roadtrip.c:28
```

It looks like our loop iterator is `i`.  So we can set a watchpoint and see when `i` is updated while stepping through the program

```
(gdb) watch i
Hardware watchpoint 2: i
(gdb) next
(gdb) next
(gdb) next
(gdb) next
```

You may have some errors where gdb says No such file or directory.  That is okay because (1) you don't need to debug standard library functions and (2) most likely the bug is in your code, not the standard libraries.

Walking through the program, it looks like we go through the loop but never update `i`!  Let's add `i++;` (already done in scenario-1.1) at line 75, hopefully this resolves the bug.

```
make scenario-1.1
./roadtrip

Roadtrip!
Group name? MCIT
How many kilometres should we go?
3
You entered: 1 km
Let's gooooooo!!!!!

We've gone 0 km
        Still in the parking lot...

Travel complete!

Summary:
Group name: MCIT
Bye bye!
```

Guess we're not done.

### Another bug

Based on the behaviour, it looks like there are two places we could start at.  The first is the query for the travel distance, where we entered 3, but later it prints the value 1.  Then the program says we've gone 0 km, but at a minimum we'd expect to have gone 1 km.

A likely place to start would be at line 19 which is where we call scanf to get the user input.  Note that the prompts appear in the gdb terminal panel.  When `(gdb)` is displayed, this means we're talking to gdb; otherwise, the input will go to the program.

```
gdb -q -tui ./roadtrip
(gdb) break 19
Breakpoint 1 at 0x401189: file roadtrip.c, line 19.
(gdb) start
Temporary breakpoint 2 at 0x4015af: file roadtrip.c, line 104.
Starting program: /workspace/gdb-valgrind-demo/roadtrip
warning: Error disabling address space randomization: Operation not permitted

Temporary breakpoint 2, main (argc=1, argv=0x7ffdb4220e78) at roadtrip.c:104
(gdb) continue
Continuing.

Roadtrip!
Group name? MCIT
How many kilometres should we go?

Breakpoint 1, travel () at roadtrip.c:19
(gdb) 
```

The breakpoint is just before scanf, so we stopped before the program asked for user input.  When we do `next`, that is when to enter `3`.

```
(gdb) next
3
(gdb)
```

Now we've given the input.  Let's check if it was stored correctly.

```
(gdb) p input
$1 = 1
```

Well that is definitely not correct.  What could be the cause here?  We stored the user input from scanf into the variable `input`, right?  Let's check the manual pages to see how this function works, just in case.

```
(gdb) quit
A debugging session is active.

        Inferior 1 [process 29888] will be killed.

Quit anyway? (y or n) y
man scanf # this pulls of the manual pages for scanf.
# press q to quit
```

We can read them online too.  Scrolling through, we can check that we used the right arguments and format specifies.  Possibly we used the return value incorrectly.  The return value for `scanf` is how many of the input items were successfully matched and assigned.  So `scanf` is returning 1 because it matched the one input.  The fix here is to remove the return value assignment!

Let's make that change (done in scenario-1.2) and run the program again.

```
make scenario-1.2
./roadtrip

Roadtrip!
Group name? MCIT
How many kilometres should we go?
3
You entered: 3 km
Let's gooooooo!!!!!

We've gone 0 km
        Still in the parking lot...

We've gone 821780880 km!
        Made it to Jupiter!

We've gone 821780881 km!
        Made it to Jupiter!

Travel complete!

Summary:
Group name: MCIT
Bye bye!
```

I think we overshot.

### Still more bugs?

While the program is reporting our entry correctly, now it seems like we're taking a field trip on The Magic Schoolbus

This specific issue could be harder to debug with gdb, so now is the time to introduce Valgrind's Memcheck memory checker.

The trick with Valgrind is to not panic at all the error messages.  Behind the scenes, valgrind is keeping track of all the memory and it will report issues as they occur during execution.  So my suggestion is to look at the first one or two reports and work out what is happening.  Reason being is that if there are a tonne of reports, many of them might be duplicate issues (e.g. if the issue is inside a loop), or due to the same function called multiple times.

We'll be running a full leak check and since we compiled with `-g`, we'll ask Valgrind to track the origins of any issues as well.

```
valgrind --leak-check=full --track-origins=yes ./roadtrip
==32765== Memcheck, a memory error detector
==32765== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==32765== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
==32765== Command: ./roadtrip
==32765== 

Roadtrip!
Group name? MCIT
How many kilometres should we go?
3
You entered: 3 km
Let's gooooooo!!!!!

We've gone 0 km
        Still in the parking lot...

==32765== Conditional jump or move depends on uninitialised value(s)
==32765==    at 0x48C9958: __vfprintf_internal (vfprintf-internal.c:1687)
==32765==    by 0x48B3D3E: printf (printf.c:33)
==32765==    by 0x40120D: travel (roadtrip.c:32)
==32765==    by 0x401626: main (roadtrip.c:121)
==32765==  Uninitialised value was created by a stack allocation
==32765==    at 0x401170: travel (roadtrip.c:16)
==32765== 
==32765== Use of uninitialised value of size 8
==32765==    at 0x48AD69B: _itoa_word (_itoa.c:179)
==32765==    by 0x48C9574: __vfprintf_internal (vfprintf-internal.c:1687)
==32765==    by 0x48B3D3E: printf (printf.c:33)
==32765==    by 0x40120D: travel (roadtrip.c:32)
==32765==    by 0x401626: main (roadtrip.c:121)
==32765==  Uninitialised value was created by a stack allocation
==32765==    at 0x401170: travel (roadtrip.c:16)
```

I think that is enough to go on for now.  The `==32765==` is letting you know which process is being debugged.  This can be useful if you're attempting to debug a child process.

The first section is just a regular startup message and copywrite information.

Then Valgrind runs behind the scenes and lets your program execute normally, where it prompts for the group name and distance.

The first loops appears okay, but then Valgrind reports that "Conditional jump or move depends on uninitialised value(s)".  What could this mean?  Let's read the stack track and origin report.

The last roadtrip function that was called was in line 32, and the origin of the uninitialised value was created on the stack in line 16.  So the culprit is the variable `distanceTravelled`, which we can see was never initialised.  What this means is that most likely there is junk data in this memory location, which is being read into the `printf` function.  The Conditional Jump depends on this value, which could be any if/then statement that relies on this value.  Since the variable is uninitialised, the behaviour of the program may be different depending on that value happens to be in memory.  Indeed, if you run this program multiple times, the distance travelled changes on each run.

Always initialise variables.  Never assume that memory "starts out as `\0`".

Also, compiling with the `-Wall` flag probably would have caught this issue.  So always compile with "Warnings, all" and try to clean them up.  Most of the time the compile checks will catch a lot of subtle bugs.

Let's fix the change (in scenario-1.3) and rerun.

```
make scenario-1.3
./roadtrip

Roadtrip!
Group name? MCIT
How many kilometres should we go?
3
You entered: 3 km
Let's gooooooo!!!!!

We've gone 0 km
        Still in the parking lot...

We've gone 1 km!
        That can't be right...

We've gone 2 km!
        That can't be right...

Travel complete!

Summary:
Group name: MCIT
Bye bye!
```

Hopefully we're done now.

[//]: # (Actually there is one more bug, but it is difficult to find.  It does not crash this program but causes other unexpected behaviour.  Can you find it?  It will show up in Scenario-2 though)

### Scenario-2

We're going to start with the same framework, but instead of the `travel` function, we'll work on actually preparing the roadtrip.  We have two new functions, `setupTrip` to initialize a struct and `setupCheck` to verify that we did the setup correctly.

### A different bug

Pull the code first.  Then run it.  Let's go to Philly!

```
make scenario-2
./roadtrip
Roadtrip!
Group name? MCIT
How many travellers? 8
Where are we going? (No spaces please) Philly
8 travellers to Philly! Allons y!

Summary:
Group name: MCIT
Destination: Philly
Travellers: 8
Bye bye!
```

Wow, we actually ran a program that doesn't have any bugs.

Or does it?

Let's take the same group to Okinawa, Japan (fun fact, I was born there).

```
./roadtrip

Roadtrip!
Group name? MCIT
How many travellers? 8
Where are we going? (No spaces please) Okinawa,Japan
1634754890 travellers to Okinawa,Japan! Allons y!
Too many people! Road trip cancelled!
```

Don't think we can fit a billion people in our car.  What could be happening?  Let's run this through valgrind.

```
valgrind --leak-check=full --track-origins=yes ./roadtrip
==18232== Memcheck, a memory error detector
==18232== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==18232== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
==18232== Command: ./roadtrip
==18232== 

Roadtrip!
Group name? MCIT
How many travellers? 8
Where are we going? (No spaces please) Okinawa,Japan
==18232== Invalid write of size 1
==18232==    at 0x483F0AC: strcpy (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==18232==    by 0x40150F: setupTrip (roadtrip.c:88)
==18232==    by 0x40163A: main (roadtrip.c:119)
==18232==  Address 0x4a4748c is 0 bytes after a block of size 12 alloc'd
==18232==    at 0x483B7F3: malloc (in /usr/lib/x86_64-linux-gnu/valgrind/vgpreload_memcheck-amd64-linux.so)
==18232==    by 0x4015DC: main (roadtrip.c:106)
```

This is just the first report from Valgrind.  Looks like we we have an invalid write.  The size is 1 byte and Valgrind says that it is 0 bytes after a block of 12 that was allocated by `malloc` in main line 106. 

```
Setup* setup = malloc(sizeof(Setup));
```

This malloc'd space has room for the integer and also the string.  But in line 88, we're copying a buffer of 1000 into malloc'd space of 8.  This spills over into the space for the integer.

```
destination[0]
destination[1]
...
destination[DEST_LEN - 1]
travellers // extra bytes go here
```

We'd need to have some kind of check to ensure that the length of the buffer doesn't exceed the space we've allocated on the heap.  We leave it as an exercise for the group to determine how to best handle this.

### A final bug?

Since we had some issues with the buffers, let's try a long string for the group name.

```
./roadtrip

Roadtrip!
Group name? 123456789
How many travellers? 6
Segmentation fault
```

This has the same issue as the previous one, but it causes a segmentation fault because it is overwriting the pointer to setup instead of the value in a reference.

```
name[0]
name[1]
...
name[NAME_LEN - 1]
setup
```

What sorts of fix would we need for these two issues?

## Conclusion

Hopefully these examples have been useful to show how useful gdb and Valgrind are.
