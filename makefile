all:
	g++ -g -O0 -Wall hash_table.cpp list.cpp hash_funcs.cpp -o hash_table 
	valgrind --tool=callgrind --dump-instr=yes --collect-jumps=yes ./hash_table
