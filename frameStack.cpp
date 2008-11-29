#include <iostream>
#include "stack.h"

stack::stack( int _capacity )
{
	capacity = _capacity;
	top = -1;
	ppArray = new Frame*[capacity];
}

stack::~stack(void)
{
	delete[] ppArray;
}

void stack::push( Frame* _pFrame )
{
	if ( isFull() == true )
	{
		Frame** newArray = new Frame*[capacity * 2];

		for ( i = 0 ; i < capacity ; i++ )
			newArray[i] = ppArray[i];
		
		free(ppArray);
		ppArray = newArray;
		capacity = capacity * 2;
	}

	ppArray[++top] = _pFrame;
}

Frame* stack::pop()
{
	if ( isEmpty() == true )
		return NULL;
	
	return ppArray[top--];
}

bool stack::isEmpty()
{
	if ( top == -1 )
		return true;
	else
		return else;
}

bool stack::isFull()
{
	if ( top == capacity )
		return true;
	else
		return else;
}

Frame* stack::getTop()
{
	return &ppArray[top];
}