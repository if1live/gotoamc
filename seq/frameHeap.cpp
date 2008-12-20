#include <cstring>
#include "frameHeap.h"
#include "frame.h"

FrameHeap::FrameHeap(int _capacity)
{
	if(_capacity < 1)
		throw "capacity must be >= 1";
	
	capacity = _capacity + 1;
	heapSize = 0;
	ppHeap = new Frame* [capacity + 1];
}

FrameHeap::~FrameHeap()
{
	if(ppHeap != 0)
	{
		delete[] ppHeap;
		ppHeap = NULL;
	}
}

bool FrameHeap::isEmpty(void)
{
	if(heapSize == 0)
		return true;
	else
		return false;
}


bool FrameHeap::isFull(void)
{
	if(heapSize == capacity - 1)
		return true;
	else
		return false;
}


Frame* FrameHeap::top(void)
{
	return ppHeap[1];
}

void FrameHeap::push(Frame *_pFrame)
{
	//insert _frame into min heap
	if(isFull() == true)
	{
		//double the capacity
		Frame **tmp = new Frame* [capacity*2 + 1];
		memcpy(tmp, ppHeap, sizeof(Frame*) * capacity + 1);

		delete[] ppHeap;
		ppHeap = tmp;
		
		capacity = capacity * 2;
	}

	int currentNode = ++heapSize;
	while(currentNode != 1 && *(ppHeap[currentNode/2]) >= *(_pFrame))
	{
		//bubble up
		ppHeap[currentNode] = ppHeap[currentNode/2];	//move parent down
		currentNode = currentNode / 2;
	}
	ppHeap[currentNode] = _pFrame;
}

Frame *FrameHeap::pop(void)
{
	//save return value
	Frame *returnValue = ppHeap[1];

	//delete min element
	if(isEmpty() == true)
		throw "heap is empty";

	//remove last element from heap
	Frame *lastE = ppHeap[heapSize--];

	//trickle down
	int currentNode = 1;	//root
	int child = 2;	//a child of currentNode
	while(child <= heapSize)
	{
		//set child to smaller child of currentNode
		if (child < heapSize)
		{
			if(*(ppHeap[child]) >= *(ppHeap[child+1]))
				child = child+1;
		}

		//can we put lastE in currentNode?
		if(*lastE < *(ppHeap[child]))
		{
			break;	//yes
		}

		//no
		ppHeap[currentNode] = ppHeap[child];
		currentNode = child;
		child = child * 2;
	}
	ppHeap[currentNode] = lastE;

	return returnValue;
}

