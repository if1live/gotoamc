#pragma once

class TextFrame;

class TextFrameQueue
{
public:
	//ctor
	TextFrameQueue(int _capacity = 10);

	//dtor
	~TextFrameQueue();

	bool isEmpty(void);
	bool isFull(void);
	TextFrame *front(void);
	TextFrame *rear(void);
	void push(TextFrame *_pTextFrame);
	TextFrame *pop(void);

private:
	TextFrame **ppQueue;
	int capacity;
	int mFront;
	int mRear;
};
