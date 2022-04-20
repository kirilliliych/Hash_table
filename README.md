# Hash table
----
## Hash table optimizations
----
Now we have written hash table. As it is a data structure, it would be a nice idea to implement it as efficiently as possible.
Therefore, there is a question: can we rewrite some code in order to speed our hash table up? Let us try.

It is obvious that the main applicant to be reorganized is find function. So what we are going to do is to call it in large quantities. 
In our case the words that will be searched for will be from english-latin dictionary and the place where we will search them is 
"War and Peace" made by L. Tolstoy.
Find will be used for 100 times for each word. Callgrind (which is a plugin for valgrind) will gather data about experiment and its result 
will be processed by KCachegrind utility in graph form. Pay attention that functions that do not refer to hash table will not be optimized because
we consider them as a user data we do not have access to.

Here you can see the configuration used to compile and launch the program:
```
all:
	nasm -f elf64 vstrcmpeqb
	g++ -c -g -O0 -Wall -mavx2 hash_table.cpp list.cpp hash_funcs.cpp -no-pie
	g++ hash_table.o list.o hash_funcs.o vstrcmpeqb.o -o hash_table
	valgrind --tool=callgrind --dump-instr=yes --collect-jumps=yes ./hash_table
```

### 1) No optimization

![ALT](Optimization/1_nooptimization.png)

