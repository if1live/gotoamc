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
	mutex = PTHREAD_MUTEX_INITIALIZER;
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
    pthread_mutex_lock( &mutex );
	if(heapSize == 0)
	{
		pthread_mutex_unlock( &mutex );
   		return true;
	}
	else
	{
		pthread_mutex_unlock( &mutex );
		return false;
	}
}


bool FrameHeap::isFull(void)
{
	pthread_mutex_lock( &mutex );
	if(heapSize == capacity - 1)
	{
		pthread_mutex_unlock( &mutex );
		return true;
	}
	else
	{
		pthread_mutex_unlock( &mutex );
		return false;
	}
}


Frame* FrameHeap::top(void)
{
	pthread_mutex_lock( &mutex );
	Frame* temp = ppHeap[1];
	pthread_mutex_unlock( &mutex );
	
	return temp;
}

void FrameHeap::push(Frame *_pFrame)
{
	pthread_mutex_lock( &mutex );
	//insert _frame into min heap
	if ( heapSize == capacity - 1 )
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
	pthread_mutex_unlock( &mutex );
}

Frame *FrameHeap::pop(void)
{
	pthread_mutex_lock( &mutex );
	//save return value
	Frame *returnValue = ppHeap[1];
	
	//delete min element
	if ( heapSize == 0 )
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
	pthread_mutex_unlock( &mutex );

	return returnValue;
}

