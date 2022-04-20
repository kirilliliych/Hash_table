# Hash table
----
## Hash table optimizations
----
Now we have written hash table. As it is a data structure, it would be a nice idea to implement it as efficiently as possible.
Therefore, there is a question: can we rewrite some code in order to speed our hash table up? Let us try.
It is obvious that the main applicant to be reorganized is find function. So what we are going to do is to call it in large quantities. 
In our case the words that will be searched for will be from english-latin dictionary and the place where we will search them is "War and Peace" made by L. Tolstoy.
Find will be used for 100 times for each word. Callgrind (which is a plugin for valgrind) will gather data about experiment and its result will be processed by KCachegrind utility
