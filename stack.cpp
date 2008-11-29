#include <iostream>
#include "stack.h"

stack::stack(void)
{
	for ( int i = 0 ; i < MAX ; i++ )
		array[i] = NULL;
}

stack::~stack(void)
{
}

void stack::push( int input )
{
	if ( isFull() == true )
		return NULL;
	else
	{
		for ( int i = MAX - 1 ; i > 0 ; i-- )
			array[i] = array[i-1];

		array[0] = input;
	}
}

int stack::pop()
{
	if ( isEmpty() == true )
		return NULL;
	else
	{
		temp = array[0];

		for ( int i = 0 ; i < MAX - 1 ; i++ )
			array[i] = array[i+1];
		
		array[MAX] = NULL;

		return temp;
	}
}

bool stack::isEmpty()
{
	if ( array[0] == NULL )
		return true;
	else
		return else;
}

bool stack::isFull()
{
	if ( array[MAX - 1] != NULL )
		return true;
	else
		return else;
}

int* stack::getTop()
{
	return &array[0];
}