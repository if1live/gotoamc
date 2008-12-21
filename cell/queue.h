#pragma once

template <class T>
class Queue
{
public:
	//ctor
	Queue(int _capacity = 10);

	//dtor
	~Queue();

	bool isEmpty(void);
	bool isFull(void);
	T& front(void);
	T& rear(void);
	void push(T &_item);
	T pop(void);

private:
	T *pQueue;
	int capacity;
	int mFront;
	int mRear;
};

#include "queue.hpp"
