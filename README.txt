Resources:
https://man7.org/linux/man-pages/man2/sched_setaffinity.2.html

For compiling the first task:
have include -std-c99, it did not like my for loops?
gcc -o task1 task1.c -lpthread -std=c99

output from strings.txt:
=======================
abcdabsufsoababuosufba
ab
=======================
Substrings: 4

For the second task
gcc -o task2 task2.c -lpthread

output from message.txt:
=======================
hello i am a text file!
=======================
hello i am a text file!

DONE

For the third task
I edited the producerThread to have the following code:

