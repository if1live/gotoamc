#include <string.h>	// for memcpy
#include "textFrame.h"
#include "textFrameQueue.h"

TextFrameQueue::TextFrameQueue(int _capacity)
{
	if(_capacity < 1)
		throw "capacity must be >= 1";
	
	capacity = _capacity;
	mFront = mRear = 0;
	ppQueue = new TextFrame* [capacity];
}

TextFrameQueue::~TextFrameQueue()
{
	if(ppQueue != 0)
	{
		delete[] ppQueue;
		ppQueue = NULL;
	}
}

bool TextFrameQueue::isEmpty(void)
{
	return (mFront == mRear);
}

bool TextFrameQueue::isFull(void)
{
	return (((mRear+1) % capacity) == mFront);
}

TextFrame* TextFrameQueue::front(void)
{
	if(isEmpty()) throw "Queue is empty: No front element";
	return ppQueue[(mFront+1) % capacity];
}

TextFrame* TextFrameQueue::rear(void)
{
	if(isEmpty()) throw "Queue is empty: No rear element";
	return ppQueue[mRear];
}

void TextFrameQueue::push(TextFrame *_pTextFrame)
{
	if(isFull())	// full? then double the capacity
	{
		// DS textbook p.146
		TextFrame** newQueue = new TextFrame*[2*capacity];
		// copy from queue to newQueue
		int start = (mFront + 1) % capacity;
		
		if(start<2)
		{
			// no wrap around
			// copy(queue+start, queue+start+capacity-1, newQueue);
			memcpy(newQueue, ppQueue, sizeof(TextFrame*)*capacity);
		}
		else
		{
			// queue wraps around
			// copy(queue+start, queue+capacity, newQueue);
			// copy(queue, queue+rear+1, newQueue+capacity-start);
			memcpy(newQueue, ppQueue, sizeof(Frame*)*(capacity-start+1));
			memcpy(newQueue+capacity-start, ppQueue, sizeof(TextFrame*)*(mRear+2));
		}
		
		// switch to newQueue
		mRear = mFront + capacity - 1;
		capacity = capacity * 2;
		delete[] ppQueue;
		ppQueue = newQueue;
	}

	mRear = (mRear+1) % capacity;
	ppQueue[mRear] = _pTextFrame;
}

TextFrame* TextFrameQueue::pop()
{
	if(isEmpty()) throw "Queue is empty";
	mFront = (mFront + 1)%capacity;

	return ppQueue[mFront];
}

