#pragma once

class FrameHeap;
class FrameQueue;
class FrameStack;

class Context	//save global variable, define,....
{
public:
	static Context *instance(void);

	//dtor
	~Context();
	

	//getter and setter
	FrameHeap *getOutputFrameHeap(void);
	FrameQueue *getInputFrameQueue(void);
	FrameStack *getUnusedInputFrameStack(void);
	FrameStack *getUnusedOutputFrameStack(void);

	int getFrameLimit(void);
private:
	//ctor
	Context();

	static Context *context;

	FrameHeap *pOutputFrameHeap;
	FrameQueue *pInputFrameQueue;
	FrameStack *pUnusedInputFrameStack;
	FrameStack *pUnusedOutputFrameStack;

	int frameLimit;
};
