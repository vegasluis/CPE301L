
#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

template <class myType>
class priorityQueue
{
public:
	priorityQueue(int i = 1000);
	~priorityQueue();
	int entries() const;
	//function returns the total count of elements in the heap

	void insert(const myType, const int);
	//function inserts an entry into the binary heap
	//If the count of the heap entries exceed the heap
	//size, the heap expands by calling the resize()function
	//the heap is maintained via the reheapUp(), the count is 
	//updated accordingly 

	bool deleteMax (myType &, int &);
	//function removes the max entry from the heap
	//,the heap properties are maintained by the reheapDown()
	//function, the count is updated. If the heap is empty, return
	//fale otherwise return true

	bool isEmpty() const;
	//function returns true if there are no elements in the heap
	// and false otherwise

	void printHeap () const;
	//function prints the indexent heap in level order with a 
	//blank line between each level
	

private:

	struct heapNode
		{
			int priority;
			myType name;
		};
	int count;
	int heapSize;
	heapNode *myHeap;

	void reheapUp(int);
	//fucntion recursively ensures the heap order is properly 
	//maintained

	void reheapDown(int);
	//fucntion recursively ensures the heap order is properly 
	//maintained

	void buildHeap();
	//function should update the heap to apply the heap properties
	
	void resize();
	//function creates a new heap array twice the size of the 
	//existing heap, copies all entries from the indexent heap
	//into the new heap, and delete the old heap.The heapSize 
	//is updated accordingly 
};
///**************************************************
///**************************************************
///**************************************************

template <class myType>
priorityQueue<myType>::priorityQueue(int i)
{
	int check = i; // i mustb e at least 1000
	if (check < 1000)
		check = 1000;
	myHeap = new heapNode [check];
	heapSize = check;
	count = 0;

}
///**************************************************
template <class myType>
priorityQueue<myType>::~priorityQueue()
{
	delete [] myHeap;
}
///**************************************************
template <class myType>
int priorityQueue<myType>::entries() const
{
return count;
}
///**************************************************

template <class myType>
void priorityQueue<myType>::buildHeap() // insert will ensure that order property is correct; only need to check min property
{
	int startPos = count/2; //starting point

	for(int i= startPos; i > 0; i--) // must check all nodes
		{
			reheapDown(i);
			reheapUp(i);
		}
}
///**************************************************
template <class myType>
void priorityQueue<myType>::insert(const myType elem, const int prior)
{
	if (count+1 >= heapSize-1) // check if in bounds
	{
		resize();
	}
	count++;
	
	myHeap[count].priority = prior; //swap
	myHeap[count].name = elem;
	reheapUp(count);
	
}

///**************************************************
template <class myType>
bool priorityQueue<myType>::deleteMax(myType & elem, int & prior)
{
	if (isEmpty()) // no elements return false
		return false; 
	else
		{
	
	prior= myHeap[1].priority; // return top values
	elem = myHeap[1].name; 
	heapNode tmp;
	tmp = myHeap[count]; // grab last node
	myHeap[1] = tmp; 	
	count--;		//lower count
	reheapDown(1); // place lowest priority on top so it reheaps entire tree
	return true;
		}
}
///**************************************************
template <class myType>
void priorityQueue<myType>::printHeap() const
{
	int spacing = 0; //counter
	int pseudoCount = 0; // count up to
	for (int i = 1; i < count+1; i++) // print all values up to count from index 1 -> count
	{
		if (spacing != pseudoCount) // initial is just one
		{
			cout << myHeap[i].name << "\t" << myHeap[i].priority << endl;
			spacing++;
		}
		else 
		{
			cout << myHeap[i].name << "\t" << myHeap[i].priority << endl;
			cout << endl;
			cout << endl;
				spacing = 1;
			if (pseudoCount == 0)
				pseudoCount = 2;
			else
				pseudoCount = pseudoCount*2; // each level can have 2^n elements
		}
	}
	
}
///**************************************************
template <class myType>
void priorityQueue<myType>::reheapUp(int index)
{
	heapNode tmp;
	int parent;
	if (index >1) // if out of bounds dont check
	{
		parent = index /2; //parent index 
		if(myHeap[parent].priority < myHeap[index].priority) //check priority
		{
		 tmp = myHeap[index]; //swap
		myHeap[index] = myHeap[parent];
		myHeap[parent] = tmp;
		}
		reheapUp(parent);
	}
}
///**************************************************
template <class myType>
void priorityQueue<myType>::reheapDown(int index)
{
	heapNode tmp;
	int parent;
	int Lindex;
	int Rindex;
	int maxIndex;

	parent = index; //parent index 
		Lindex = index*2; //left child index
		Rindex =(index*2)+1; //right child index

	if (Lindex > count && Rindex > count ) //if child is greater than count then out of bounds
		return; //return if unused node

	if(Rindex > count)
		maxIndex=Lindex;
		
	else
	{
		
		if (myHeap[Lindex].priority >= myHeap[Rindex].priority) //check which has priority
			maxIndex = Lindex; //left has priority	
		else
			maxIndex = Rindex; //right has priority 
	}

	if(myHeap[index].priority < myHeap[maxIndex].priority) // check parent priority
	{	
		tmp = myHeap[maxIndex];
		myHeap[maxIndex] = myHeap[parent];
		myHeap[parent] = tmp;

		reheapDown(maxIndex);
	}
	
}
///**************************************************
template <class myType>
void priorityQueue<myType>::resize()
{
	heapSize = heapSize*2;
	heapNode *heapSizeRe = new heapNode[heapSize];
	heapNode *deleteHeap = myHeap;
	myHeap = heapSizeRe;
	
	
	for (int i = 1; i < count+1; i++) //copy values
	{
		myHeap[i] = deleteHeap[i];

	}
	
	delete [] deleteHeap;
		
}
///**************************************************
template <class myType>
bool priorityQueue<myType> :: isEmpty() const
{
	if (count == 0) // no count so nothing is in array
	{
		return true;
	}
	else 
		return false;
}

#endif
