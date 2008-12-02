#include <iostream>
#include <string.h>
#include "textFrameStack.h"

TextFrameStack::TextFrameStack( int _capacity )
{
	capacity = _capacity;
	m_top = -1;
	ppArray = new TextFrame*[capacity];
}

TextFrameStack::~TextFrameStack(void)
{
	delete[] ppArray;
}

void TextFrameStack::push( TextFrame* _pTextFrame )
{
	if ( isFull() == true )
	{
		TextFrame** newArray = new TextFrame*[capacity * 2];

		//for ( int i = 0 ; i < capacity ; i++ )
		//	newArray[i] = ppArray[i];
		memcpy(newArray, ppArray, sizeof(TextFrame*)*capacity);
		
		delete[] ppArray;
		ppArray = newArray;
		capacity = capacity * 2;
	}

	ppArray[++m_top] = _pTextFrame;
}

TextFrame* TextFrameStack::pop()
{
	if ( isEmpty() == true )
		throw "stack pop error: stack is empty";
	
	return ppArray[m_top--];
}

bool TextFrameStack::isEmpty()
{
	if ( m_top == -1 )
		return true;
	else
		return false;
}

bool TextFrameStack::isFull()
{
	if ( m_top == capacity )
		return true;
	else
		return false;
}

TextFrame* TextFrameStack::top()
{
	return ppArray[m_top];
}
