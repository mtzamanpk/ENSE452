Description

This project implements a queue using a singly-linked list in C++. 
Each element stores a pair of integers (x, y). The queue supports 
standard FIFO behavior with insertions at the tail and removals from 
the head.

Additions
insert(Data d, unsigned position): Allows insertion at any position 
in the queue. Valid positions range from 0 to size(). 
Throws a std::out_of_range exception if the position is invalid.

Unit Tests
assignmentTest(): Tests various queue operations:
1. Insertion at the tail.
2. Insertion at the head.
3. Insertion at the tail using position = size().
4. Insertion in the middle (position 2).
5. Invalid insertion (position out of range).
6. Insert multiple elements at the head.
7. Insert multiple elements in the middle.
8. Combination of insert and remove operations.

How to Run
step 1: make 
step 2: ./testq
