#include "context.h"
#include "frameHeap.h"
#include "frameQueue.h"
#include "frameStack.h"
#include "frame.h"

Context::Context()
	:pOutputFrameHeap(NULL),
	 pInputFrameQueue(NULL),
	 pUnusedInputFrameStack(NULL),
	 pUnusedOutputFrameStack(NULL),
	 frameLimit(30)
{
}

Context::~Context()
{
	if(pOutputFrameHeap != NULL)
	{
		delete pOutputFrameHeap;
		pOutputFrameHeap = NULL;
	}

	if(pInputFrameQueue != NULL)
	{
		delete pInputFrameQueue;
		pInputFrameQueue = NULL;
	}

	if(pUnusedInputFrameStack != NULL)
	{
		delete pUnusedInputFrameStack;
		pUnusedInputFrameStack = NULL;
	}
	
	if(pUnusedOutputFrameStack != NULL)
	{
		delete pUnusedOutputFrameStack != NULL;
		pUnusedOutputFrameStack = NULL;
	}
}

Context *Context::context = NULL;

Context *Context::instance(void)
{
	if(context != NULL)
		context = new Context();

	return context;
}


FrameHeap *Context::getOutputFrameHeap(void)
{
	if(pOutputFrameHeap != NULL)
		pOutputFrameHeap = new FrameHeap[frameLimit];
	return pOutputFrameHeap;
}

FrameHeap *Context::getInputFrameQueue(void)
{
	if(pInputFrameQueue != NULL)
		pInputFrameQueue = new FrameQueue[frameLimit];
	return pInputFrameQueue;
}

FrameStack *Context::getUnusedInputFrameStacK(void)
{
	if(pUnusedInputFrameStack != NULL)
		pUnusedInputFrameStack = new FrameStack[frameLimit];

	return pUnusedInputFrameStack;
}

FrameStack *Context::getUnusedOutputFrameStack(void)
{
	if(pUnusedOutputFrameStack != NULL)
		pUnusedOutputFrameStack = new FrameStack[frameLimit];

	return pUnusedOutputFrameStack;
}
