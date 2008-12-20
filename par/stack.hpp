#include <iostream>
#include <string.h>
#include "stack.h"


template <class T>
Stack<T>::Stack( int _capacity )
{
	capacity = _capacity;
	m_top = -1;
	pArray = new T[capacity];
	mutex = PTHEAD_MUTEX_INITIALIZER;
}

template <class T>
Stack<T>::~Stack(void)
{
	delete[] pArray;
}


template <class T>
void Stack<T>::push(T &_item )
{
	pthread_mutex_lock( &mutex );
	if ( m_top == capacity )
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
	pthread_mutex_unlock( &mutex );
}

template <class T>
T Stack<T>::pop()
{
	pthread_mutex_lock( &mutex );
	if ( m_top == -1 )
		throw "stack pop error: stack is empty";
	
	T temp = pArray[m_top--];
	pthread_mutex_unlock( &mutex );
	return temp;
}


template <class T>
bool Stack<T>::isEmpty()
{
	pthread_mutex_lock( &mutex );
	if ( m_top == -1 )
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


template <class T>
bool Stack<T>::isFull()
{
	pthread_mutex_lock( &mutex );
	if ( m_top == capacity )
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


template <class T>
T &Stack<T>::top()
{
	pthread_mutex_lock( &mutex );
	T& temp = pArray[m_top];
	pthread_mutex_unlock( &mutex );
	return temp;
}
