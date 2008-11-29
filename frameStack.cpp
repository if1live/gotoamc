#include <iostream>
#include <string.h>
#include "frameStack.h"

FrameStack::FrameStack( int _capacity )
{
	capacity = _capacity;
	m_top = -1;
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

		//for ( int i = 0 ; i < capacity ; i++ )
		//	newArray[i] = ppArray[i];
		memcpy(newArray, ppArray, sizeof(Frame*)*capacity);
		
		delete[] ppArray;
		ppArray = newArray;
		capacity = capacity * 2;
	}

	ppArray[++m_top] = _pFrame;
}

Frame* FrameStack::pop()
{
	if ( isEmpty() == true )
		throw "stack pop error: stack is empty";
	
	return ppArray[m_top--];
}

bool FrameStack::isEmpty()
{
	if ( m_top == -1 )
		return true;
	else
		return false;
}

bool FrameStack::isFull()
{
	if ( m_top == capacity )
		return true;
	else
		return false;
}

Frame* FrameStack::top()
{
	return ppArray[m_top];
}
