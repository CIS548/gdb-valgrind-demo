scenario-0: clean
	git checkout scenario-0
	clang my_program.c -o my_program

scenario-0.1: clean
	git checkout scenario-0.1
	clang -g my_program.c -o my_program

scenario-0.2: clean
	git checkout scenario-0.2
	clang -g my_program.c -o my_program

scenario-1: clean
	git checkout scenario-1
	clang -g roadtrip.c -o roadtrip

scenario-1.1: clean
	git checkout scenario-1.1
	clang -g roadtrip.c -o roadtrip

scenario-1.2: clean
	git checkout scenario-1.2
	clang -g roadtrip.c -o roadtrip

scenario-1.3: clean
	git checkout scenario-1.3
	clang -g roadtrip.c -o roadtrip

scenario-2: clean
	git checkout scenario-2
	clang -g roadtrip.c -o roadtrip

clean:
	git restore .
	rm -f roadtrip my_program

