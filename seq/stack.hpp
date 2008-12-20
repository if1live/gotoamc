#include <iostream>
#include <string.h>
#include "stack.h"


template <class T>
Stack<T>::Stack( int _capacity )
{
	capacity = _capacity;
	m_top = -1;
	pArray = new T[capacity];
}

template <class T>
Stack<T>::~Stack(void)
{
	delete[] pArray;
}


template <class T>
void Stack<T>::push(T &_item )
{
	if ( isFull() == true )
	{
		T* newArray = new T[capacity * 2];

		//for ( int i = 0 ; i < capacity ; i++ )
		//	newArray[i] = ppArray[i];
		memcpy(newArray, pArray, sizeof(T)*capacity);
		
		delete[] pArray;
		pArray = newArray;
		capacity = capacity * 2;
	}

	pArray[++m_top] = _item;
}

template <class T>
T Stack<T>::pop()
{
	if ( isEmpty() == true )
		throw "stack pop error: stack is empty";
	
	return pArray[m_top--];
}


template <class T>
bool Stack<T>::isEmpty()
{
	if ( m_top == -1 )
		return true;
	else
		return false;
}


template <class T>
bool Stack<T>::isFull()
{
	if ( m_top == capacity )
		return true;
	else
		return false;
}


template <class T>
T &Stack<T>::top()
{
	return pArray[m_top];
}
