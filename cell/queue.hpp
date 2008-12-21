#include <string.h>	// for memcpy
#include "queue.h"

template <class T>
Queue<T>::Queue(int _capacity)
{
	if(_capacity < 1)
		throw "capacity must be >= 1";
	
	capacity = _capacity;
	mFront = mRear = 0;
	pQueue = new T[capacity];
}

template <class T>
Queue<T>::~Queue<T>()
{
	if(pQueue != 0)
	{
		delete[] pQueue;
		pQueue = NULL;
	}
}

template <class T>
bool Queue<T>::isEmpty(void)
{
	return (mFront == mRear);
}


template <class T>
bool Queue<T>::isFull(void)
{
	return (((mRear+1) % capacity) == mFront);
}


template <class T>
T &Queue<T>::front(void)
{
	if(isEmpty()) throw "Queue is empty: No front element";
	return pQueue[(mFront+1) % capacity];
}

template <class T>
T &Queue<T>::rear(void)
{
	if(isEmpty()) throw "Queue is empty: No rear element";
	return pQueue[mRear];
}


template <class T>
void Queue<T>::push(T &_item)
{
	if(isFull())	// full? then double the capacity
	{
		// DS textbook p.146
		T *newQueue = new T[2*capacity];
		// copy from queue to newQueue
		int start = (mFront + 1) % capacity;
		
		if(start<2)
		{
			// no wrap around
			// copy(queue+start, queue+start+capacity-1, newQueue);
			memcpy(newQueue, pQueue, sizeof(T)*capacity);
		}
		else
		{
			// queue wraps around
			// copy(queue+start, queue+capacity, newQueue);
			// copy(queue, queue+rear+1, newQueue+capacity-start);
			memcpy(newQueue, pQueue, sizeof(T)*(capacity-start+1));
			memcpy(newQueue+capacity-start, pQueue, sizeof(T)*(mRear+2));
		}
		
		// switch to newQueue
		mRear = mFront + capacity - 1;
		capacity = capacity * 2;
		delete[] pQueue;
		pQueue = newQueue;
	}

	mRear = (mRear+1) % capacity;
	pQueue[mRear] = _item;
}


template <class T>
T Queue<T>::pop()
{
	if(isEmpty()) throw "Queue is empty";
	mFront = (mFront + 1)%capacity;

	return pQueue[mFront];
}

