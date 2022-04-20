all:
	nasm -f elf64 vstrcmpeqb
	g++ -c -g -O0 -Wall -mavx2 hash_table.cpp list.cpp hash_funcs.cpp -no-pie
	g++ hash_table.o list.o hash_funcs.o vstrcmpeqb.o -o hash_table
	valgrind --tool=callgrind --dump-instr=yes --collect-jumps=yes ./hash_table
	
	
