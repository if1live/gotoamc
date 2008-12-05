#include <stdio.h>
#include <cassert>
#include "context.h"
#include "frameHeap.h"
#include "frame.h"
#include "textFrame.h"
#include "textFrame2PPM.h"
#include "videoIO.h"
#include "frame2TextFrame.h"

Context::Context()
{
	frameLimit = 10;
	pOutputFrameHeap = new FrameHeap(frameLimit);
	pInputFrameQueue = new Queue<Frame *>(frameLimit);
	pUnusedInputFrameStack = new Stack<Frame *>(frameLimit);
	pUnusedOutputFrameStack = new Stack<Frame *>(frameLimit);
	pTextFrameQueue = new Queue<TextFrame *>(frameLimit);
	pUnusedTextFrameStack = new Stack<TextFrame *>(frameLimit);

	pVideoIO = NULL;
	pTextFrame2PPM = NULL;
	pFrame2TextFrame = NULL;
}

Context::~Context()
{
	if(pOutputFrameHeap != NULL)
	{
		fprintf(stderr, "[Context]OutputFrameHeap delete\n");
		assert(pOutputFrameHeap->isEmpty() == true && "OutputFrameHeap is not empty!");
		delete pOutputFrameHeap;
		pOutputFrameHeap = NULL;
	}

	if(pInputFrameQueue != NULL)
	{
		fprintf(stderr, "[Context]InputFrameQueue delete\n");
		assert(pInputFrameQueue->isEmpty() == true && "InputFrameQueue is not empty!");
		delete pInputFrameQueue;
		pInputFrameQueue = NULL;
	}

	if(pUnusedInputFrameStack != NULL)
	{
		fprintf(stderr, "[Context]UnusedInputFrameStack delete\n");
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
		fprintf(stderr, "[Context]UnusedOutputFrameStack delete\n");
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
		fprintf(stderr, "[Context]TextFrameQueue delete\n");
		assert(pTextFrameQueue->isEmpty() == true && "TextFrameQueue is not empty!");
		delete pTextFrameQueue;
		pTextFrameQueue = NULL;
	}

	if(pUnusedTextFrameStack != NULL)
	{
		fprintf(stderr, "[Context]UnusedTextFrameStack delete\n");
		while(pUnusedTextFrameStack->isEmpty() == false)
		{
			TextFrame *textFrame = pUnusedTextFrameStack->top();
			delete textFrame;
			pUnusedTextFrameStack->pop();
		}
		delete pUnusedTextFrameStack;
		pUnusedTextFrameStack = NULL;
	}

	if(pVideoIO != NULL)
	{
		fprintf(stderr, "[Context]VideoIO delete\n");
		delete pVideoIO;
	}


	if(pTextFrame2PPM != NULL)
	{
		fprintf(stderr, "[Context]TextFrame2PPM delete\n");
		delete pTextFrame2PPM;
	}

	if(pFrame2TextFrame != NULL)
	{
		fprintf(stderr, "[Context]Frame2TextFrame delete\n");
		delete pFrame2TextFrame;
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

Queue<TextFrame *> *Context::getTextFrameQueue(void)
{
	return pTextFrameQueue;
}

Stack<TextFrame *> *Context::getUnusedTextFrameStack(void)
{
	return pUnusedTextFrameStack;
}


VideoIO *Context::getVideoIO(void)
{
	assert(pVideoIO != NULL && "pVideoIO is NULL!");
	return pVideoIO;
}


TextFrame2PPM *Context::getTextFrame2PPM(void)
{
	assert(pTextFrame2PPM != NULL && "pTextFrame2PPM is NULL");
	return pTextFrame2PPM;
}


Frame2TextFrame *Context::getFrame2TextFrame(void)
{
	assert(pFrame2TextFrame != NULL && "pFrame2TexteFrame is NULL");
	return pFrame2TextFrame;
}


void Context::setVideoIO(VideoIO *_ptr)
{
	pVideoIO = _ptr;
}


void Context::setTextFrame2PPM(TextFrame2PPM *_ptr)
{
	pTextFrame2PPM = _ptr;
}


void Context::setFrame2TextFrame(Frame2TextFrame *_ptr)
{
	pFrame2TextFrame = _ptr;
}
