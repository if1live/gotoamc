#include <stdio.h>
#include "context.h"
#include "frameHeap.h"
#include "frameQueue.h"
#include "frameStack.h"
#include "textFrameStack.h"
#include "textFrameQueue.h"
#include "frame.h"

Context::Context()
{
	frameLimit = 30;
	pOutputFrameHeap = new FrameHeap(frameLimit);
	pInputFrameQueue = new FrameQueue(frameLimit);
	pUnusedInputFrameStack = new FrameStack(frameLimit);
	pUnusedOutputFrameStack = new FrameStack(frameLimit);
	pTextFrameQueue = new TextFrameQueue(frameLimit);
	pUnusedTextFrameStack = new TextFrameStack(frameLimit);
}

Context::~Context()
{
	if(pOutputFrameHeap != NULL)
	{
		while(pOutputFrameHeap->isEmpty() == false)
		{
			Frame *frame = pOutputFrameHeap->top();
			delete frame;
			pOutputFrameHeap->pop();
		}
		delete pOutputFrameHeap;
		pOutputFrameHeap = NULL;
	}

	if(pInputFrameQueue != NULL)
	{
		while(pInputFrameQueue->isEmpty() == false)
		{
			Frame *frame = pInputFrameQueue->front();
			delete frame;
			pInputFrameQueue->pop();
		}
		delete pInputFrameQueue;
		pInputFrameQueue = NULL;
	}

	if(pUnusedInputFrameStack != NULL)
	{
		while(pUnusedInputFrameStack->isEmpty() == false)
		{
			Frame *frame = pUnusedInputFrameStack->top();
			delete frame;
			pUnusedInputFrameStack->pop();
		}
		delete pUnusedInputFrameStack;
		pUnusedInputFrameStack = NULL;
	}
	
	if(pUnusedOutputFrameStack != NULL)
	{
		while(pUnusedOutputFrameStack->isEmpty() == false)
		{
			Frame *frame = pUnusedOutputFrameStack->top();
			delete frame;
			pUnusedOutputFrameStack->pop();
		}
		delete pUnusedOutputFrameStack;
		pUnusedOutputFrameStack = NULL;
	}
	
	if(pTextFrameQueue != NULL)
	{
		while(pTextFrameQueue->isEmpty() == false)
		{
			TextFrame *frame = pTextFrameQueue->front();
			delete frame;
			pTextFrameQueue->pop();
		}
		delete pTextFrameQueue;
		pTextFrameQueue = NULL;
	}

	if(pUnusedTextFrameStack != NULL)
	{
		while(pUnusedTextFrameStack->isEmpty() == false)
		{
			TextFrame *frame = pUnusedTextFrameStack->top();
			delete frame;
			pUnuesdTextFrameStack->pop();
		}
		delete pUnusedTextFrameStack;
		pUnusedTextFrameStack = NULL;
	}
}


Context *Context::context = NULL;

Context *Context::instance(void)
{
	if(context == NULL)
		context = new Context();

	return context;
}


FrameHeap *Context::getOutputFrameHeap(void)
{
	return pOutputFrameHeap;
}

FrameQueue *Context::getInputFrameQueue(void)
{
	return pInputFrameQueue;
}

FrameStack *Context::getUnusedInputFrameStack(void)
{
	return pUnusedInputFrameStack;
}

FrameStack *Context::getUnusedOutputFrameStack(void)
{
	return pUnusedOutputFrameStack;
}

int Context::getFrameLimit(void)
{
	return frameLimit;
}

TextFrameQueue *Context::getTextFrameQueue(void)
{
	return pTextFrameQueue;
}

TextFrameStack *Context::getUnusedTextFrameStack(void)
{
	return pUnusedTextFrameStack;
}
