#pragma once
#include <pthread.h>

template <class T>
class Stack
{
public:
	Stack( int _capacity = 10 );
	~Stack(void);

	void push( T &_item );
	T pop();
	bool isEmpty();
	bool isFull();
	T &top();

private:
	T* pArray;
	int capacity;
	int m_top;
	ptread_mutex_t mutex;
};

#include "stack.hpp"

