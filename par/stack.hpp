#include <iostream>
#include <string.h>


template <class T>
Stack<T>::Stack( int _capacity )
{
	capacity = _capacity;
	m_top = -1;
	pArray = new T[capacity];
	pthread_mutex_init(&mutex, NULL);
}

template <class T>
Stack<T>::~Stack(void)
{
	delete[] pArray;
	pthread_mutex_destroy(&mutex);
}


template <class T>
void Stack<T>::push(T &_item )
{
	fprintf( stderr, "Starting PUSH to Stack\n" );	//for debugging
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
	fprintf( stderr, "Ended PUSH to Stack\n" );		//for debugging
}

template <class T>
T Stack<T>::pop()
{
	fprintf( stderr, "Starting POP from Stack\n" );	//for debugging
	pthread_mutex_lock( &mutex );
	if ( m_top == -1 )
		throw "stack pop error: stack is empty";
	
	T temp = pArray[m_top--];
	pthread_mutex_unlock( &mutex );
	fprintf( stderr, "Ended POP from Stack\n" );	//for debugging
	return temp;
}


template <class T>
bool Stack<T>::isEmpty()
{
	fprintf( stderr, "Checking Stack is Empty\n" );	//for debugging
	pthread_mutex_lock( &mutex );
	if ( m_top == -1 )
	{
		pthread_mutex_unlock( &mutex );
		fprintf( stderr, "Stack is Empty!\n" );		//for debugging
		return true;
	}
	else
	{
		pthread_mutex_unlock( &mutex );
		fprintf( stderr, "Stack is not Empty!\n" );	//for debugging
		return false;
	}
}


template <class T>
bool Stack<T>::isFull()
{
	fprintf( stderr, "Checking Stack is Full\n" );	//for debugging
	pthread_mutex_lock( &mutex );
	if ( m_top == capacity )
	{
		pthread_mutex_unlock( &mutex );
		fprintf( stderr, "Stack is Full!\n" );		//for debugging
		return true;
	}
	else
	{
		pthread_mutex_unlock( &mutex );
		fprintf( stderr, "Stack is not Full!\n" );	//for debugging
		return false;
	}
}


template <class T>
T &Stack<T>::top()
{
	fprintf( stderr, "Starting Get Top of Stack\n" );//for debugging
	pthread_mutex_lock( &mutex );
	T& temp = pArray[m_top];
	pthread_mutex_unlock( &mutex );
	fprintf( stderr, "Get Top of Stack\n" );		//for debugging
	return temp;
}
