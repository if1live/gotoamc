#pragma once

class Frame;

class FrameStack
{
public:
	FrameStack( int _capacity = 10 );
	~FrameStack(void);

	void push( Frame* _pFrame );
	Frame* pop();
	bool isEmpty();
	bool isFull();
	Frame* top();

private:
	Frame** ppArray;
	int capacity;
	int m_top;
};
