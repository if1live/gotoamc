#pragma once

class Frame;

class FrameQueue
{
public:
	//ctor
	FrameQueue(int _capacity = 10);

	//dtor
	~FrameQueue();

	bool isEmpty(void);
	bool isFull(void);
	Frame *front(void);
	Frame *rear(void);
	void push(Frame *_pFrame);
	Frame *pop(void);

private:
	Frame **ppQueue;
	int capacity;
	int mFront;
	int mRear;
};
