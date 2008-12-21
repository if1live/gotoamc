#pragma once

#include "queue.h"
#include "stack.h"

class Frame;
class FrameHeap;
class VideoIO;

class Context	//save global variable, define,....
{
public:
	static Context *instance(void);

	//dtor
	~Context();

	//getter and setter
	FrameHeap *getOutputFrameHeap(void);
	Queue<Frame *> *getInputFrameQueue(void);
	Stack<Frame *> *getUnusedInputFrameStack(void);
	Stack<Frame *> *getUnusedOutputFrameStack(void);

	VideoIO *getVideoIO(void);

	void setVideoIO(VideoIO *_ptr);

	int getFrameLimit(void);
private:
	//ctor
	Context();

	static Context *context;

	FrameHeap *pOutputFrameHeap;
	Queue<Frame *> *pInputFrameQueue;
	Stack<Frame *> *pUnusedInputFrameStack;
	Stack<Frame *> *pUnusedOutputFrameStack;

	VideoIO *pVideoIO;

	int frameLimit;
};

