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
	pthread_mutex_init(&mutex, NULL);
}

FrameHeap::~FrameHeap()
{
	if(ppHeap != 0)
	{
		delete[] ppHeap;
		ppHeap = NULL;
	}
	pthread_mutex_destroy(&mutex);
}

bool FrameHeap::isEmpty(void)
{
	fprintf( stderr, "Checking Heap is Empty\n" );	//for debugging
    pthread_mutex_lock( &mutex );
	if(heapSize == 0)
	{
		pthread_mutex_unlock( &mutex );
		fprintf( stderr, "Heap is Empty!\n" );		//for debugging
		return true;
	}
	else
	{
		pthread_mutex_unlock( &mutex );
		fprintf( stderr, "Heap is not Empty!\n" );	//for debugging
		return false;
	}
}


bool FrameHeap::isFull(void)
{
	fprintf( stderr, "Checking Heap is Full\n" );	//for debugging
	pthread_mutex_lock( &mutex );
	if(heapSize == capacity - 1)
	{
		pthread_mutex_unlock( &mutex );
		fprintf( stderr, "Heap is Full!\n" );		//for debugging
		return true;
	}
	else
	{
		pthread_mutex_unlock( &mutex );
		fprintf( stderr, "Heap is not Full!\n" );	//for debugging
		return false;
	}
}


Frame* FrameHeap::top(void)
{
	fprintf( stderr, "Starting Get Top of Heap\n" );//for debugging
	pthread_mutex_lock( &mutex );
	Frame* temp = ppHeap[1];
	pthread_mutex_unlock( &mutex );
	fprintf( stderr, "Get Top of Heap\n" ); 		//for debugging
	return temp;
}

void FrameHeap::push(Frame *_pFrame)
{
	fprintf( stderr, "Starting PUSH to Heap\n" ); 	//for debugging
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
	fprintf( stderr, "Ended PUSH to Heap\n" ); 		//for debugging
}

Frame *FrameHeap::pop(void)
{
	fprintf( stderr, "Starting POP from Heap\n" ); 	//for debugging
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
	fprintf( stderr, "Ended POP from Heap\n" );		//for debugging

	return returnValue;
}

