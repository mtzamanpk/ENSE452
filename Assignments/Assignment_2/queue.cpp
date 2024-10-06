/**
   Project: Implementation of a Queue in C++.
   Programmer: Karim Naqvi
   Course: enel452
*/

#include "queue.h"
#include <iostream>
#include <cstdlib>              // for exit

using namespace std;

Queue::Queue()
{
    head = 0;
    tail = 0;
    nelements = 0;
    verbose = false;
}

Queue::~Queue()
{
    for (QElement* qe = head; qe != 0;)
    {
	QElement* temp = qe;
	qe = qe->next;
	delete(temp);
    }
}

void Queue::remove(Data* d)
{
    if (size() > 0)
    {
        QElement* qe = head;
        head = head->next;
        nelements--;
        *d = qe->data;
	delete qe;
    }
    else
    {
        cerr << "Error: Queue is empty.\n";
        exit(1);
    }
}

void Queue::insert(Data d)
{
    if (verbose) std::cout << "insert(d)\n";
    QElement* el = new QElement(d);
    if (size() > 0)
    {
        tail->next = el;
    }
    else
    {
        head = el;
    }
    tail = el;
    nelements++;
}

/*
Assignment Code Implementation
Name: Muhammad Zaman 200449177
Assignment: 2
*/

void Queue::insert(Data d, unsigned position)
{
    // Check if the given position is within the valid range [0, size()].
    // If the position is out of bounds, print an error message and exit with code 3.
    if (position > size())
    {
        throw std::out_of_range("insert: range error.");
        return;
    }

    // Create a new QElement to store the provided data.
    QElement* newElement = new QElement(d);

    // If the position is 0, we are inserting at the head of the queue.
    if (position == 0)
    {
        // Set the new element's next pointer to the current head.
        newElement->next = head;
        // Update the head to point to the new element.
        head = newElement;

        // If the queue was previously empty, the tail also needs to point to the new head.
        if (size() == 0)
        {
            tail = head;
        }
    }
    // If the position is equal to the size of the queue, insert the element at the tail.
    else if (position == size())
    {
        // If the tail is not null, link the current tail to the new element.
        if (tail != nullptr)
        {
            tail->next = newElement;
            tail = newElement; // Update the tail to the new element.
        }
        // If the queue is empty (tail is null), both head and tail need to point to the new element.
        else
        {
            head = tail = newElement;
        }
    }
    // Otherwise, we are inserting the element in the middle of the queue.
    else
    {
        // Traverse the queue to find the element just before the insertion point.
        QElement* prev = head;
        for (unsigned i = 0; i < position - 1; ++i)
        {
            prev = prev->next;  // Move to the next element until the correct position is found.
        }
        // Link the new element to the next element in the queue.
        newElement->next = prev->next;
        // Link the previous element to the new element, placing it in the correct position.
        prev->next = newElement;
    }
    // Increase the element count since we have successfully inserted a new element.
    nelements++;
}

/* END OF ASSIGNMENT CODE */

bool Queue::search(Data otherData) const
{
    QElement* insideEl = head;
    for (int i = 0; i < nelements; i++)
    {
        if (insideEl->data.equals(otherData))
            return true;
        insideEl = insideEl->next;
    }
    return false;
}

void Queue::print() const
{
    QElement* qe = head;
    if (size() > 0)
    {
        for (unsigned i = 0; i < size(); i++)
        {
            cout << i << ":(" << qe->data.x << "," << qe->data.y << ") ";
            qe = qe->next;
        }
    }
    cout << "\n";
}

unsigned Queue::size() const
{
    return nelements;
}
