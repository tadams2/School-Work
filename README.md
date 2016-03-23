# School-Work
CMSC 421 - Principles of Operating Systems (Fall 15)

hw2.c - The parent process will first create 4 child processess with the fork() syscall. The parent will then ask the user what they would like to do with the child processess. The options are 'Kill a Child', 'Signal a Child', 'Reap a Child', and 'Kill and Reap all Children'

Compiled with - gcc --std=c99 -Wall -O2 -o hw2 hw2.c

Project Page - http://www.csee.umbc.edu/~jtang/archives/cs421.f15/homework/hw2.html
------------------------------------------

hw3.c - The program simulates stock trades, the parameters for the program are given by an input file which has the number of traders (threads), the number of stock funds, the number of transactions, the initial amounts in each fund, and finally where the funds get moved to from where. The program implements a scheduler that decides which trader handles each transaction.

Compiled with - gcc --std=c99 -Wall -O2 -pthread -o hw3 hw3.c

Project Page - http://www.csee.umbc.edu/~jtang/archives/cs421.f15/homework/hw3.html
------------------------------------------

rotX.c - The program emulates a caesar cipher as a miscellaneous character device where the encryption key is written to /dev/rotX, the phrase to encrypt is mmap()'d to /dev/rotX and the phrase to tell the device to encrypt is written to /dev/rotXctl the phrase is any string starting with "go"

rotX-test.c - Many unit tests which covers everything from a happy path to writting to the device with thousands of threads simultaneously

Project Page - http://www.csee.umbc.edu/~jtang/archives/cs421.f15/homework/proj1.html
------------------------------------------

scrubber.c - This program creates an internet filter as a miscellaneous character device registered in /dev/scrubber. Words written to /dev/scrubber are stored in a Linux Kernel Linked List and the character device will filter payloads written over telnet

scrubber-test.c - Many unit tests which cover everything from happy path tests to many edge cases like writing phrases over telnet that are smaller than the words in the list to make sure we do not segfault

Project Page - http://www.csee.umbc.edu/~jtang/archives/cs421.f15/homework/proj2.html
------------------------------------------------------------------------------------------------

CMSC 471 - Artificial Intelligence (Spring 16)

search.py - This python(3.3) program implements a Depth First Search, Breadth First Search, and a Uniform Cost Search. The program builds a graph from an input file as a dictionary of dictionaries in the form {parent : {child : weight } }. 
The correct usage of this program is: 
                                     'python search.py <inputfile> <outputfile> <start node> <goal node> <search type (DFS, BFS, UCS)>
