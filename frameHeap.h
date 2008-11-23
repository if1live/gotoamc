#pragma once

class Frame;

class FrameHeap
{
public:
	//ctor
	FrameHeap(int _capacity = 10);

	//dtor
	~FrameHeap();

	bool isEmpty(void);
	bool isFull(void);
	Frame *top(void);
	void push(Frame *_pFrame);
	Frame *pop(void);

private:
	Frame **ppHeap;
	int capacity;
	int heapSize;
};

