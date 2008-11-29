#include <iostream>
#include <stdlib.h>
#include "frameStack.h"

FrameStack::FrameStack( int _capacity )
{
	capacity = _capacity;
	top = -1;
	ppArray = new Frame*[capacity];
}

FrameStack::~FrameStack(void)
{
	delete[] ppArray;
}

void FrameStack::push( Frame* _pFrame )
{
	if ( isFull() == true )
	{
		Frame** newArray = new Frame*[capacity * 2];

		for ( int i = 0 ; i < capacity ; i++ )
			newArray[i] = ppArray[i];
		
		free(ppArray);
		ppArray = newArray;
		capacity = capacity * 2;
	}

	ppArray[++top] = _pFrame;
}

Frame* FrameStack::pop()
{
	if ( isEmpty() == true )
		return NULL;
	
	return ppArray[top--];
}

bool FrameStack::isEmpty()
{
	if ( top == -1 )
		return true;
	else
		return false;
}

bool FrameStack::isFull()
{
	if ( top == capacity )
		return true;
	else
		return false;
}

Frame* FrameStack::getTop()
{
	return ppArray[top];
}
