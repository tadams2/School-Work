# School-Work
### CMSC 421 - Principles of Operating Systems (Fall 15)

[Homework 2](http://www.csee.umbc.edu/~jtang/archives/cs421.f15/homework/hw2.html)

hw2.c - The parent process will first create 4 child processess with the fork() syscall. The parent will then ask the user what they would like to do with the child processess. The options are 'Kill a Child', 'Signal a Child', 'Reap a Child', and 'Kill and Reap all Children'

Compiled with - 
```bash 
gcc --std=c99 -Wall -O2 -o hw2 hw2.c
```

------------------------------------------
[Homework 3](http://www.csee.umbc.edu/~jtang/archives/cs421.f15/homework/hw3.html)

hw3.c - The program simulates stock trades, the parameters for the program are given by an input file which has the number of traders (threads), the number of stock funds, the number of transactions, the initial amounts in each fund, and finally where the funds get moved to from where. The program implements a scheduler that decides which trader handles each transaction.

Compiled with - 
```bash
gcc --std=c99 -Wall -O2 -pthread -o hw3 hw3.c
```

------------------------------------------
[Project 1](http://www.csee.umbc.edu/~jtang/archives/cs421.f15/homework/proj1.html)

rotX.c - The program emulates a caesar cipher as a miscellaneous character device where the encryption key is written to /dev/rotX, the phrase to encrypt is mmap()'d to /dev/rotX and the phrase to tell the device to encrypt is written to /dev/rotXctl the phrase is any string starting with "go"

rotX-test.c - Many unit tests which covers everything from a happy path to writting to the device with thousands of threads simultaneously


------------------------------------------
[Project 2](http://www.csee.umbc.edu/~jtang/archives/cs421.f15/homework/proj2.html)

scrubber.c - This program creates an internet filter as a miscellaneous character device registered in /dev/scrubber. Words written to /dev/scrubber are stored in a Linux Kernel Linked List and the character device will filter payloads written over telnet

scrubber-test.c - Many unit tests which cover everything from happy path tests to many edge cases like writing phrases over telnet that are smaller than the words in the list to make sure we do not segfault


------------------------------------------------------------------------------------------------

### CMSC 471 - Artificial Intelligence (Spring 16)

Project 1

search.py - This python(3.3) program implements a Depth First Search, Breadth First Search, and a Uniform Cost Search. The program builds a graph from an input file as a dictionary of dictionaries in the form {parent : {child : weight } }. This program has the following usage: 
```bash
python search.py <inputfile> <outputfile> <startNode> <goalNode> <searchType (DFS, BFS, UCS)>
```

Project 3
This project was an effort to get us to use any classifer we wanted to identify some simple 32x32 images. There were very little rules for this project and we had to get the highest % accuracy we could. I ended up with nearly a 93% accuracy using a Support Vector Machine. Of the files supplied one is for training and the other is the tool to interact with trained classifer.

Project 4

Clustering.py - This project was to write a K-means classifer. For this we had to also show a graph of our results (sample output image supplied with the input as well). For this we needed to use numpy since using standard python arrays would destroy any sense of efficency, numpy also allows us to use some very useful linear algebra functions.
to run this program do the following
```bash
python clustering.py <number of clusters> <inputfile>
```

### CMSC 491 - Hadoop Based Distributed Computing (Spring 16)

[Homework 2] (https://www.csee.umbc.edu/~shadam1/491s16/hw2.html)
First part of the semester long project. In this part we develop a Kafa producer / consumer combo and store the digested tweets into HDFS for further processing in the future. Avro was used serialize the data we get from tweets and then public it to the Kafka topic, then subsequently consume it. This project can be built using maven with the supplied xml file

[Homework 3] (https://www.csee.umbc.edu/~shadam1/491s16/hw3.html)
Second part of the project. In this part we had to develop pig scripts to batch process data along with the help of cron jobs to schedule the processing. we had to develop pig scripts to do the following: Extract user information, 100 most chatty users, most popular users, top 100 hashtags, and reverse index of twitter words to tweet ID. All of these jobs were executed hourly and the results stored back into HDFS

