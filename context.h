#pragma once

class FrameHeap;
class FrameQueue;
class FrameStack;
class TextFrameStack;
class TextFrameQueue;

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
	TextFrameQueue *getTextFrameQueue(void);
	TextFrameStack *getUnusedTextFrameStack(void);

	int getFrameLimit(void);
private:
	//ctor
	Context();

	static Context *context;

	FrameHeap *pOutputFrameHeap;
	FrameQueue *pInputFrameQueue;
	FrameStack *pUnusedInputFrameStack;
	FrameStack *pUnusedOutputFrameStack;
	TextFrameQueue *pTextFrameQueue;
	TextFrameStack *getUnusedTextFrameStack;

	int frameLimit;
};
