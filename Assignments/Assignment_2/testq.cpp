/**
   Project: Implementation of a Queue in C++.
   Programmer: Karim Naqvi
   Course: enel452
   Description: test code
*/

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <vector>

#include "queue.h"

/**
   Compare the given queue to the given array of data elements.  Return
   true if they are all equal.
 */
bool testQueueEquality( Queue & q, std::vector<Data> const & array)
{
    if (q.size() != array.size())
	return false;

    for (unsigned i = 0; i < q.size(); ++i)
    {
        Data d(0,0);
	q.remove(&d);
	if ( !d.equals(array[i]) )
	    return false;
	q.insert(d);
    }
    return true;
}

/*
self implemented assignment test code 
*/
void assignmentTest()
{
    // Test 1: Insertion at the tail of the queue
    std::cout << "\nTest 1: Insertions at tail\n";
    Queue q1;
    q1.insert(Data(1, 2));   // Insert at tail
    q1.insert(Data(3, 4));   // Insert at tail
    q1.insert(Data(5, 6));   // Insert at tail
    q1.print();  // Expected output: 0:(1,2) 1:(3,4) 2:(5,6)

    // Test 2: Insertion at the head of a new queue
    std::cout << "\nTest 2: Insertion at the head\n";
    Queue q2;
    q2.insert(Data(1, 2));   // Insert at tail
    q2.insert(Data(3, 4));   // Insert at tail
    q2.insert(Data(5, 6));   // Insert at tail
    q2.insert(Data(-1, -2), 0);  // Insert at head
    q2.print();  // Expected output: 0:(-1,-2) 1:(1,2) 2:(3,4) 3:(5,6)

    // Test 3: Insertion at the tail of a new queue using position = size()
    std::cout << "\nTest 3: Insertion at the tail using position = size()\n";
    Queue q3;
    q3.insert(Data(1, 2));   // Insert at tail
    q3.insert(Data(3, 4));   // Insert at tail
    q3.insert(Data(5, 6));   // Insert at tail
    q3.insert(Data(7, 8), q3.size());  // Insert at tail using position = size()
    q3.print();  // Expected output: 0:(1,2) 1:(3,4) 2:(5,6) 3:(7,8)

    // Test 4: Insertion in the middle of a new queue
    std::cout << "\nTest 4: Insertion in the middle (position 2)\n";
    Queue q4;
    q4.insert(Data(1, 2));   // Insert at tail
    q4.insert(Data(3, 4));   // Insert at tail
    q4.insert(Data(5, 6));   // Insert at tail
    q4.insert(Data(-3, -4), 2);  // Insert in the middle at position 2
    q4.print();  // Expected output: 0:(1,2) 1:(3,4) 2:(-3,-4) 3:(5,6)

    // Test 5: Testing invalid insertion (position > size()) on a new queue
    std::cout << "\nTest 5: Invalid insertion (position out of range)\n";
    Queue q5;
    q5.insert(Data(1, 2));   // Insert at tail
    q5.insert(Data(3, 4));   // Insert at tail
    try
    {
        q5.insert(Data(9, 10), 5);  // Invalid position
    }
    catch (const std::out_of_range& e)
    {
        std::cerr << e.what() << "\n";  // Catch the out_of_range exception and print the message
    }
    q5.print();  // Expected output: 0:(1,2) 1:(3,4)

    // Test 6: Insert multiple elements at the head
    std::cout << "\nTest 6: Insert multiple elements at the head\n";
    Queue q6;
    q6.insert(Data(1, 2));  // Insert at tail
    q6.insert(Data(-1, -2), 0);  // Insert at head
    q6.insert(Data(-3, -4), 0);  // Insert at head
    q6.insert(Data(-5, -6), 0);  // Insert at head
    q6.print();  // Expected output: 0:(-5,-6) 1:(-3,-4) 2:(-1,-2) 3:(1,2)

    // Test 7: Insert multiple elements in the middle
    std::cout << "\nTest 7: Insert multiple elements in the middle\n";
    Queue q7;
    q7.insert(Data(1, 2));  // Insert at tail
    q7.insert(Data(3, 4));  // Insert at tail
    q7.insert(Data(7, 8), 1);  // Insert in the middle at position 1
    q7.insert(Data(9, 10), 2);  // Insert in the middle at position 2
    q7.print();  // Expected output: 0:(1,2) 1:(7,8) 2:(9,10) 3:(3,4)

    // Test 8: Combination of insert and remove operations
    std::cout << "\nTest 8: Combination of insert and remove operations\n";
    Queue q8;
    q8.insert(Data(1, 2));  // Insert at tail
    q8.insert(Data(3, 4));  // Insert at tail
    q8.insert(Data(5, 6));  // Insert at tail
    Data temp;
    q8.remove(&temp);  // Remove head (1,2)
    q8.insert(Data(7, 8), 1);  // Insert in the middle at position 1
    q8.remove(&temp);  // Remove head (3,4)
    q8.insert(Data(9, 10));  // Insert at tail
    q8.print();  // Expected output: 0:(5,6) 1:(7,8) 2:(9,10)
}





int main()
{
    using namespace std;

    cout << "Testing queue.\n";
    Queue q1;

    q1.insert(Data(1,1));
    q1.insert(Data(3,3));
    q1.insert(Data(5,5));

    q1.print();

    vector<Data> dataVec;
    dataVec.push_back(Data(1,1));
    dataVec.push_back(Data(3,3));
    dataVec.push_back(Data(5,5));

    assert(testQueueEquality(q1, dataVec));

    Data d44(4, 4);
    bool found = q1.search(d44);
    assert(found == false);


    q1.insert(d44);  // now is (1,1),(3,3),(5,5),(4,4)
    found = q1.search(d44);
    assert(found == true);

    // now queue is(1,1),(3,3),(5,5),(4,4) and 
    // dataVec has (1,1),(3,3),(5,5).  Not equal
    assert(testQueueEquality(q1, dataVec) == false);

    Data temp;
    q1.remove(&temp);  // now q1 is (3,3),(5,5),(4,4)

    Data temp2(1,1);
    assert(temp.equals(temp2));  // (1,1) == (1,1)

    Data temp3(6,6);
    found = q1.search(temp3);
    assert(found == false);

    // assignmentTest function call to test the queue operations
    assignmentTest();
}
