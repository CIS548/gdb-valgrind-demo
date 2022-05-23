<style type="text/css">.rendered-markdown{font-size:14px} .rendered-markdown>*:first-child{margin-top:0!important} .rendered-markdown>*:last-child{margin-bottom:0!important} .rendered-markdown a{text-decoration:underline;color:#b75246} .rendered-markdown a:hover{color:#f36050} .rendered-markdown h1, .rendered-markdown h2, .rendered-markdown h3, .rendered-markdown h4, .rendered-markdown h5, .rendered-markdown h6{margin:24px 0 10px;padding:0;font-weight:bold;-webkit-font-smoothing:antialiased;cursor:text;position:relative} .rendered-markdown h1 tt, .rendered-markdown h1 code, .rendered-markdown h2 tt, .rendered-markdown h2 code, .rendered-markdown h3 tt, .rendered-markdown h3 code, .rendered-markdown h4 tt, .rendered-markdown h4 code, .rendered-markdown h5 tt, .rendered-markdown h5 code, .rendered-markdown h6 tt, .rendered-markdown h6 code{font-size:inherit} .rendered-markdown h1{font-size:28px;color:#000} .rendered-markdown h2{font-size:22px;border-bottom:1px solid #ccc;color:#000} .rendered-markdown h3{font-size:18px} .rendered-markdown h4{font-size:16px} .rendered-markdown h5{font-size:14px} .rendered-markdown h6{color:#777;font-size:14px} .rendered-markdown p, .rendered-markdown blockquote, .rendered-markdown ul, .rendered-markdown ol, .rendered-markdown dl, .rendered-markdown table, .rendered-markdown pre{margin:15px 0} .rendered-markdown hr{border:0 none;color:#ccc;height:4px;padding:0} .rendered-markdown>h2:first-child, .rendered-markdown>h1:first-child, .rendered-markdown>h1:first-child+h2, .rendered-markdown>h3:first-child, .rendered-markdown>h4:first-child, .rendered-markdown>h5:first-child, .rendered-markdown>h6:first-child{margin-top:0;padding-top:0} .rendered-markdown a:first-child h1, .rendered-markdown a:first-child h2, .rendered-markdown a:first-child h3, .rendered-markdown a:first-child h4, .rendered-markdown a:first-child h5, .rendered-markdown a:first-child h6{margin-top:0;padding-top:0} .rendered-markdown h1+p, .rendered-markdown h2+p, .rendered-markdown h3+p, .rendered-markdown h4+p, .rendered-markdown h5+p, .rendered-markdown h6+p{margin-top:0} .rendered-markdown ul, .rendered-markdown ol{padding-left:30px} .rendered-markdown ul li>:first-child, .rendered-markdown ul li ul:first-of-type, .rendered-markdown ol li>:first-child, .rendered-markdown ol li ul:first-of-type{margin-top:0} .rendered-markdown ul ul, .rendered-markdown ul ol, .rendered-markdown ol ol, .rendered-markdown ol ul{margin-bottom:0} .rendered-markdown dl{padding:0} .rendered-markdown dl dt{font-size:14px;font-weight:bold;font-style:italic;padding:0;margin:15px 0 5px} .rendered-markdown dl dt:first-child{padding:0} .rendered-markdown dl dt>:first-child{margin-top:0} .rendered-markdown dl dt>:last-child{margin-bottom:0} .rendered-markdown dl dd{margin:0 0 15px;padding:0 15px} .rendered-markdown dl dd>:first-child{margin-top:0} .rendered-markdown dl dd>:last-child{margin-bottom:0} .rendered-markdown blockquote{border-left:4px solid #DDD;padding:0 15px;color:#777} .rendered-markdown blockquote>:first-child{margin-top:0} .rendered-markdown blockquote>:last-child{margin-bottom:0} .rendered-markdown table th{font-weight:bold} .rendered-markdown table th, .rendered-markdown table td{border:1px solid #ccc;padding:6px 13px} .rendered-markdown table tr{border-top:1px solid #ccc;background-color:#fff} .rendered-markdown table tr:nth-child(2n){background-color:#f8f8f8} .rendered-markdown img{max-width:100%;-moz-box-sizing:border-box;box-sizing:border-box} .rendered-markdown code, .rendered-markdown tt{margin:0 2px;padding:0 5px;border:1px solid #eaeaea;background-color:#f8f8f8;border-radius:3px} .rendered-markdown code{white-space:nowrap} .rendered-markdown pre>code{margin:0;padding:0;white-space:pre;border:0;background:transparent} .rendered-markdown .highlight pre, .rendered-markdown pre{background-color:#f8f8f8;border:1px solid #ccc;font-size:13px;line-height:19px;overflow:auto;padding:6px 10px;border-radius:3px} .rendered-markdown pre code, .rendered-markdown pre tt{margin:0;padding:0;background-color:transparent;border:0}</style>
<div class="rendered-markdown"><h1>gdb-valgrind-tutorial</h1>
<p>This is a set of buggy programs to demonstrate how gdb and valgrind are super duper useful.  This recitation has been/will be recorded, but we are providing the source code and walkthrough notes.</p>
<h2>Author</h2>
<p><a href="https://github.com/alesparza">Antonio Esparza</a>, a 595 TA (but mostly a debugger)</p>
<h2>Introduction</h2>
<p>gdb is a great tool for debugging, but it does require some time exploring the basic capabilities to get used to it.  I do highly recommend running in the text user interface mode</p>
<p>Valgrind is a set of utilities that analyze code for errors.  We will be focusing on the Memcheck tool for this course.  Memcheck allows us to inspect memory usage to find bug related to memory management, such as uninitialised variables, invalid read/write errors, and memory leaks</p>
<h2>How this works</h2>
<p>You can follow along in this recitation by loading the appropriate commits with <code>git checkout scenario-&lt;num&gt;</code>.  Each scenario features a program with bugs</p>
<p>We'll start with scenario-0, which is an introduction to gdb, followed by scenario-1 which is more complicated and introduces valgrind, followed by a more examples in scenario-2.</p>
<h2>Scenario-0</h2>
<p>Load the code for scenario-0 and compile it</p>
<pre><code>git checkout scenario-0
clang my_program.c -o my_program
</code></pre>
<p>You can also run <code>make-scenario-0</code> which will do all the required steps for you</p>
<pre><code>make scenario-0 # automagically checkout the correct commit and compile
</code></pre>
<p>Let's try running the program and see what happens.</p>
<pre><code>./my_program
argc = 1
argv[0] = ./my_program
Hello World
Segmentation fault
</code></pre>
<p>Yikes, we just got started and already have a segmentation fault.  Let's start up gdb and walk through the program.</p>
<pre><code>gdb ./my_program
GNU gdb (Ubuntu 9.2-0ubuntu1~20.04.1) 9.2
Copyright (C) 2020 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later &lt;http://gnu.org/licenses/gpl.html&gt;
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
&lt;http://www.gnu.org/software/gdb/bugs/&gt;.
Find the GDB manual and other documentation resources online at:
    &lt;http://www.gnu.org/software/gdb/documentation/&gt;.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from ./my_program...
(gdb) 
</code></pre>
<p>That's kind of busy.  I don't even know what's going on.  Here is the first trick: telling gdb to be quiet.  First, exit gdb:</p>
<pre><code>(gdb) quit
</code></pre>
<p>Then start gdb in quiet mode.</p>
<pre><code>gdb -q ./my_program # -q is the 'quiet' flag
Reading symbols from ./my_program...
(gdb) 
</code></pre>
<p>This is great, but it's all in the command line and we have to keep switching windows to look at the code.  Let's put it all into one place by using the Text Interface Mode.</p>
<pre><code>(gdb) quit
gdb -q -tui ./my_program # -tui enables the Text User Interface
Reading symbols from ./my_program...
(No debugging symbols found in ./my_program)
(gdb) 
</code></pre>
<p>Cool, now we can see our source code as we debug&hellip; except it says there is No Source Available in the top panel.  Most likely this means that we did not use the <code>-g</code> flag when compiling.  I suggest always compiling with debugging symbols enabled because we'll almost certainly need to debug at some point.</p>
<p>We can go into the makefile and add the <code>-g</code> flag, or we can checkout a commit that already does this for us.  Again, the makefile will do this for us automagically.</p>
<pre><code>(gdb) q
make scenario-0.1
</code></pre>
<p>Now when we run gdb, we'll see the source code!</p>
<pre><code>gdb -q -tui ./my_program
Reading symbols from ./my_program...
(gdb)
</code></pre>
<p>Let's start the program with <code>start</code>.</p>
<pre><code>(gdb) start
Temporary breakpoint 1 at 0x4011af: file my_program.c, line 13.
Starting program: /workspace/gdb-valgrind-demo/my_program
warning: Error disabling address space randomization: Operation not permitted

Temporary breakpoint 1, main (argc=1, argv=0x7ffc3c7b6298) at my_program.c:13
(gdb) 
</code></pre>
<p>We just started the program and we're already getting errors!?  Not exactly.  The warning here is about disabling address space randomization.  As a security feature (take 551 for more details), gdb will attempt to disable address space randomization, meaning that it will try to load the program into the same memory location for each debugging session.  gdb is reporting that it was unable to do so (which means your memory addresses may differ).</p>
<p>This is not going to be an issue for any of the projects we're working on, so let's walk through the program with <code>next</code> and try to find the segmentation fault.</p>
<pre><code>(gdb) next
(gdb) next
(gdb) next
(gdb) next  

Program received signal SIGSEGV, Segmentation fault.
0x0000000000401175 in printString (string=0x0) at my_program.c:9
</code></pre>
<p>Okay, gdb is now reporting that the program received a SEGSEGV (illegal memory access signal) due to a segmentation fault.  gdb has paused the program but if we attempt to continue walking through the program, the signal will pass to the program and the process will terminate.</p>
<p>We can see that the segmentation fault appears to be at line 9, which is in the <code>printString</code> function.</p>
<p>At this point, you may notice that the display has gotten a little corrupted.  This happens when your program writes to stdout.  Easily fixed with the <code>refresh</code> command</p>
<pre><code>(gdb) refresh)
</code></pre>
<p>What we can do now is inspect the variables and try to determine what happened in the program.  Since we're trying to print a character, we could start by printing <code>string[0]</code></p>
<pre><code>(gdb) print string[0]
Cannot access memory at address 0x0
</code></pre>
<p>Hmmm&hellip; so we seem to have a NULL pointer, and dereferencing it is the cause of the segmentation fault.  We can check if string is indeed a NULL pointer:</p>
<pre><code>(gdb) print string
$1 = 0x0
</code></pre>
<p>Yep, that's a problem.</p>
<p>Let's check the stack trace and go back to the main function</p>
<pre><code>(gdb) where
#0  0x0000000000401175 in printString (string=0x0) at my_program.c:9
#1  0x00000000004011f3 in main (argc=1, argv=0x7ffc3c7b6298)
    at my_program.c:16
(gdb) frame 1
#1  0x00000000004011f3 in main (argc=1, argv=0x7ffc3c7b6298)
    at my_program.c:16
(gdb) print argv[1]
$2 = 0x0
</code></pre>
<p>It appears that this program was expecting an argument, but failed to provide any checks in case it was not provided.  I think we can close gdb at this point.  Let's go and update the code to include some kind of check, which we can do by checking out scenario-0.2.</p>
<pre><code>(gdb) quit
A debugging session is active.

        Inferior 1 [process 12147] will be killed.

Quit anyway? (y or n) y
make scenario-0.2
</code></pre>
<p>While we could just test the program, let's use gdb to watch the program execute.  We'll set a breakpoint on line 17 with the <code>break</code> before the conditional block executes, allow the program to run up to the breakpoint with <code>continue</code>.  Note that to step-into a function, we use the <code>step</code> command.</p>
<pre><code>gdb -q -tui ./my_program
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
</code></pre>
<p>Indeed, we have skipped over the troublesome function.</p>
<p>Let's check that it works when we do include an argument.  Note the use of the <code>--args</code> flag, which tells gdb that everything following is part of the program input.  Also, we can set a breakpoint by providing the function name directly.</p>
<pre><code>gdb -q -tui --args ./my_program test
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
</code></pre>
<p>This time, we entered the function and the string pointer points to the string &ldquo;test&rdquo;, as we expect.</p>
<h2>Scenario-1</h2>
<h3>A bug</h3>
<p>This program is a bit more complicated and has some more difficult bugs.  Checkout <code>scenario-1</code> and run it.  We'll use MCIT as our group name, and start out with a short trip of three km.</p>
<pre><code>make scenario-1
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
</code></pre>
<p>Erm&hellip;. well that's a problem.  We can't seem to get out of the parking lot, even though we said we wanted to take a 3 km trip.  This seems like an infinity loop.</p>
<p>Now, I understand most people here would just open the code and look for the issue, but I'd like you to pretend for a moment that we're inside a large codebase where we can't just insert print statements everywhere for debugging purposes.  This is why builds compile with the <code>-g</code> flag for development and without it for production builds.</p>
<p>Let's start gdb and just let it run.  When we hit the infinite loop, we can send an interrupt signal and see where the loop appears to be stuck.  If you've looked at the code, you could also set a break point at a likely location.</p>
<pre><code>gdb -q -tui ./roadtrip
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
</code></pre>
<p>Your results may vary depending on where you stopped, but in one run, I was inside printf and the stack trace had a lot of library function calls.  So pick the frame that has the last one with your program code.</p>
<pre><code>(gdb) where
#0  0x00007f3a16fc0077 in __GI___libc_write (fd=1, buf=0x22a82a0, nbytes=45)
    at ../sysdeps/unix/sysv/linux/write.c:26
#1  0x00007f3a16f40e8d in _IO_new_file_write (
    f=0x7f3a1709f6a0 &lt;_IO_2_1_stdout_&gt;, data=0x22a82a0, n=45)
    at fileops.c:1176
#2  0x00007f3a16f42951 in new_do_write (to_do=45,
    data=0x22a82a0 "We've gone 0 km\n\tStill in the parking lot...\n",
    fp=0x7f3a1709f6a0 &lt;_IO_2_1_stdout_&gt;) at libioP.h:948
#3  _IO_new_do_write (to_do=45,
    data=0x22a82a0 "We've gone 0 km\n\tStill in the parking lot...\n",
    fp=0x7f3a1709f6a0 &lt;_IO_2_1_stdout_&gt;) at fileops.c:426
#4  _IO_new_do_write (fp=0x7f3a1709f6a0 &lt;_IO_2_1_stdout_&gt;,
    data=0x22a82a0 "We've gone 0 km\n\tStill in the parking lot...\n",
    to_do=45) at fileops.c:423
#5  0x00007f3a16f416b5 in _IO_new_file_xsputn (n=45, data=&lt;optimized out&gt;,
    f=&lt;optimized out&gt;) at libioP.h:948
#6  _IO_new_file_xsputn (f=0x7f3a1709f6a0 &lt;_IO_2_1_stdout_&gt;,
    data=&lt;optimized out&gt;, n=45) at fileops.c:1197
#7  0x00007f3a16f28972 in __vfprintf_internal (
    s=0x7f3a1709f6a0 &lt;_IO_2_1_stdout_&gt;,
    format=0x402054 "We've gone 0 km\n\tStill in the parking lot...\n",
    ap=ap@entry=0x7ffed5416970, mode_flags=mode_flags@entry=0)
#8  0x00007f3a16f13d3f in __printf (format=&lt;optimized out&gt;) at printf.c:33
#9  0x00000000004011fa in travel () at roadtrip.c:28
#10 0x0000000000401617 in main (argc=1, argv=0x7ffed5416bb8) at roadtrip.c:121
(gdb) frame 9
#9  0x00000000004011fa in travel () at roadtrip.c:28
</code></pre>
<p>It looks like our loop iterator is <code>i</code>.  So we can set a watchpoint and see when <code>i</code> is updated while stepping through the program</p>
<pre><code>(gdb) watch i
Hardware watchpoint 2: i
(gdb) next
(gdb) next
(gdb) next
(gdb) next
</code></pre>
<p>You may have some errors where gdb says No such file or directory.  That is okay because (1) you don't need to debug standard library functions and (2) most likely the bug is in your code, not the standard libraries.</p>
<p>Walking through the program, it looks like we go through the loop but never update <code>i</code>!  Let's add <code>i++;</code> (already done in scenario-1.1) at line 75, hopefully this resolves the bug.</p>
<pre><code>make scenario-1.1
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
</code></pre>
<p>Guess we're not done.</p>
<h3>Another bug</h3>
<p>Based on the behaviour, it looks like there are two places we could start at.  The first is the query for the travel distance, where we entered 3, but later it prints the value 1.  Then the program says we've gone 0 km, but at a minimum we'd expect to have gone 1 km.</p>
<p>A likely place to start would be at line 19 which is where we call scanf to get the user input.  Note that the prompts appear in the gdb terminal panel.  When <code>(gdb)</code> is displayed, this means we're talking to gdb; otherwise, the input will go to the program.</p>
<pre><code>gdb -q -tui ./roadtrip
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
</code></pre>
<p>The breakpoint is just before scanf, so we stopped before the program asked for user input.  When we do <code>next</code>, that is when to enter <code>3</code>.</p>
<pre><code>(gdb) next
3
(gdb)
</code></pre>
<p>Now we've given the input.  Let's check if it was stored correctly.</p>
<pre><code>(gdb) p input
$1 = 1
</code></pre>
<p>Well that is definitely not correct.  What could be the cause here?  We stored the user input from scanf into the variable <code>input</code>, right?  Let's check the manual pages to see how this function works, just in case.</p>
<pre><code>(gdb) quit
A debugging session is active.

        Inferior 1 [process 29888] will be killed.

Quit anyway? (y or n) y
man scanf # this pulls of the manual pages for scanf.
# press q to quit
</code></pre>
<p>We can read them online too.  Scrolling through, we can check that we used the right arguments and format specifies.  Possibly we used the return value incorrectly.  The return value for <code>scanf</code> is how many of the input items were successfully matched and assigned.  So <code>scanf</code> is returning 1 because it matched the one input.  The fix here is to remove the return value assignment!</p>
<p>Let's make that change (done in scenario-1.2) and run the program again.</p>
<pre><code>make scenario-1.2
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
</code></pre>
<p>I think we overshot.</p>
<h3>Still more bugs?</h3>
<p>While the program is reporting our entry correctly, now it seems like we're taking a field trip on The Magic Schoolbus</p>
<p>This specific issue could be harder to debug with gdb, so now is the time to introduce Valgrind's Memcheck memory checker.</p>
<p>The trick with Valgrind is to not panic at all the error messages.  Behind the scenes, valgrind is keeping track of all the memory and it will report issues as they occur during execution.  So my suggestion is to look at the first one or two reports and work out what is happening.  Reason being is that if there are a tonne of reports, many of them might be duplicate issues (e.g. if the issue is inside a loop), or due to the same function called multiple times.</p>
<p>We'll be running a full leak check and since we compiled with <code>-g</code>, we'll ask Valgrind to track the origins of any issues as well.</p>
<pre><code>valgrind --leak-check=full --track-origins=yes ./roadtrip
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
</code></pre>
<p>I think that is enough to go on for now.  The <code>==32765==</code> is letting you know which process is being debugged.  This can be useful if you're attempting to debug a child process.</p>
<p>The first section is just a regular startup message and copywrite information.</p>
<p>Then Valgrind runs behind the scenes and lets your program execute normally, where it prompts for the group name and distance.</p>
<p>The first loops appears okay, but then Valgrind reports that &ldquo;Conditional jump or move depends on uninitialised value(s)&ldquo;.  What could this mean?  Let's read the stack track and origin report.</p>
<p>The last roadtrip function that was called was in line 32, and the origin of the uninitialised value was created on the stack in line 16.  So the culprit is the variable <code>distanceTravelled</code>, which we can see was never initialised.  What this means is that most likely there is junk data in this memory location, which is being read into the <code>printf</code> function.  The Conditional Jump depends on this value, which could be any if/then statement that relies on this value.  Since the variable is uninitialised, the behaviour of the program may be different depending on that value happens to be in memory.  Indeed, if you run this program multiple times, the distance travelled changes on each run.</p>
<p>Always initialise variables.  Never assume that memory &ldquo;starts out as <code>\0</code>&ldquo;.</p>
<p>Also, compiling with the <code>-Wall</code> flag probably would have caught this issue.  So always compile with &ldquo;Warnings, all&rdquo; and try to clean them up.  Most of the time the compile checks will catch a lot of subtle bugs.</p>
<p>Let's fix the change (in scenario-1.3) and rerun.</p>
<pre><code>make scenario-1.3
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
</code></pre>
<p>Hopefully we're done now.</p>
<h3>Scenario-2</h3>
<p>We're going to start with the same framework, but instead of the <code>travel</code> function, we'll work on actually preparing the roadtrip.  We have two new functions, <code>setupTrip</code> to initialize a struct and <code>setupCheck</code> to verify that we did the setup correctly.</p>
<h3>A different bug</h3>
<p>Pull the code first.  Then run it.  Let's go to Philly!</p>
<pre><code>make scenario-2
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
</code></pre>
<p>Wow, we actually ran a program that doesn't have any bugs.</p>
<p>Or does it?</p>
<p>Let's take the same group to Okinawa, Japan (fun fact, I was born there).</p>
<pre><code>./roadtrip

Roadtrip!
Group name? MCIT
How many travellers? 8
Where are we going? (No spaces please) Okinawa,Japan
1634754890 travellers to Okinawa,Japan! Allons y!
Too many people! Road trip cancelled!
</code></pre>
<p>Don't think we can fit a billion people in our car.  What could be happening?  Let's run this through valgrind.</p>
<pre><code>valgrind --leak-check=full --track-origins=yes ./roadtrip
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
</code></pre>
<p>This is just the first report from Valgrind.  Looks like we we have an invalid write.  The size is 1 byte and Valgrind says that it is 0 bytes after a block of 12 that was allocated by <code>malloc</code> in main line 106.</p>
<pre><code>Setup* setup = malloc(sizeof(Setup));
</code></pre>
<p>This malloc'd space has room for the integer and also the string.  But in line 88, we're copying a buffer of 1000 into malloc'd space of 8.  This spills over into the space for the integer.</p>
<pre><code>destination[0]
destination[1]
...
destination[DEST_LEN - 1]
travellers // extra bytes go here
</code></pre>
<p>We'd need to have some kind of check to ensure that the length of the buffer doesn't exceed the space we've allocated on the heap.  We leave it as an exercise for the group to determine how to best handle this.</p>
<h3>A final bug?</h3>
<p>Since we had some issues with the buffers, let's try a long string for the group name.</p>
<pre><code>./roadtrip

Roadtrip!
Group name? 123456789
How many travellers? 6
Segmentation fault
</code></pre>
<p>This has the same issue as the previous one, but it causes a segmentation fault because it is overwriting the pointer to setup instead of the value in a reference.</p>
<pre><code>name[0]
name[1]
...
name[NAME_LEN - 1]
setup
</code></pre>
<p>What sorts of fix would we need for these two issues?</p>
<h2>Conclusion</h2>
<p>Hopefully these examples have been useful to show how useful gdb and Valgrind are.</p>
</div>