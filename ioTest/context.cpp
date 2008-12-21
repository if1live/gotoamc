#include <stdio.h>
#include <cassert>
#include "context.h"
#include "frameHeap.h"
#include "frame.h"
#include "videoIO.h"

Context::Context()
{
	frameLimit = 8;
	pOutputFrameHeap = new FrameHeap();
	pInputFrameQueue = new Queue<Frame *>();
	pUnusedInputFrameStack = new Stack<Frame *>();
	pUnusedOutputFrameStack = new Stack<Frame *>();

	pVideoIO = NULL;
}

Context::~Context()
{
	if(pOutputFrameHeap != NULL)
	{
		fprintf(stderr, "[Context]OutputFrameHeap delete...");
		//	assert(pOutputFrameHeap->isEmpty() == true && "OutputFrameHeap is not empty!");
		while(pOutputFrameHeap->isEmpty() == false)
		{
			Frame *frame = pOutputFrameHeap->top();
			delete frame;
			pOutputFrameHeap->pop();
		}
		delete pOutputFrameHeap;
		pOutputFrameHeap = NULL;

		fprintf(stderr, "complete\n");
	}

	if(pInputFrameQueue != NULL)
	{
		fprintf(stderr, "[Context]InputFrameQueue delete...");
		//assert(pInputFrameQueue->isEmpty() == true && "InputFrameQueue is not empty!");
		while(pInputFrameQueue->isEmpty() == false)
		{
			Frame *frame = pInputFrameQueue->front();
			delete frame;
			pInputFrameQueue->pop();
		}
		delete pInputFrameQueue;
		pInputFrameQueue = NULL;

		fprintf(stderr, "complete\n");
	}

	if(pUnusedInputFrameStack != NULL)
	{
		fprintf(stderr, "[Context]UnusedInputFrameStack delete...");
		while(pUnusedInputFrameStack->isEmpty() == false)
		{
			Frame *frame = pUnusedInputFrameStack->top();
			delete frame;
			pUnusedInputFrameStack->pop();
		}
		delete pUnusedInputFrameStack;
		pUnusedInputFrameStack = NULL;

		fprintf(stderr, "complete\n");
	}
	
	if(pUnusedOutputFrameStack != NULL)
	{
		fprintf(stderr, "[Context]UnusedOutputFrameStack delete...");
		while(pUnusedOutputFrameStack->isEmpty() == false)
		{
			Frame *frame = pUnusedOutputFrameStack->top();
			delete frame;
			pUnusedOutputFrameStack->pop();
		}
		delete pUnusedOutputFrameStack;
		pUnusedOutputFrameStack = NULL;

		fprintf(stderr, "complete\n");
	}
	
	if(pVideoIO != NULL)
	{
		fprintf(stderr, "[Context]VideoIO delete...");
		delete pVideoIO;
		fprintf(stderr, "complete\n");
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

Queue<Frame *> *Context::getInputFrameQueue(void)
{
	return pInputFrameQueue;
}

Stack<Frame *> *Context::getUnusedInputFrameStack(void)
{
	return pUnusedInputFrameStack;
}

Stack<Frame *> *Context::getUnusedOutputFrameStack(void)
{
	return pUnusedOutputFrameStack;
}

int Context::getFrameLimit(void)
{
	return frameLimit;
}

VideoIO *Context::getVideoIO(void)
{
	assert(pVideoIO != NULL && "pVideoIO is NULL!");
	return pVideoIO;
}


void Context::setVideoIO(VideoIO *_ptr)
{
	pVideoIO = _ptr;
}

