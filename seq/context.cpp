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
	frameLimit = 8;
	pOutputFrameHeap = new FrameHeap();
	pInputFrameQueue = new Queue<Frame *>();
	pUnusedInputFrameStack = new Stack<Frame *>();
	pUnusedOutputFrameStack = new Stack<Frame *>();
	pTextFrameQueue = new Queue<TextFrame *>();
	pUnusedTextFrameStack = new Stack<TextFrame *>();

	pVideoIO = NULL;
	pTextFrame2PPM = NULL;
	pFrame2TextFrame = NULL;
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
	
	if(pTextFrameQueue != NULL)
	{
		fprintf(stderr, "[Context]TextFrameQueue delete...");
		//assert(pTextFrameQueue->isEmpty() == true && "TextFrameQueue is not empty!");
		while(pTextFrameQueue->isEmpty() == false)
		{
			TextFrame *textFrame = pTextFrameQueue->front();
			delete textFrame;
			pTextFrameQueue->pop();
		}
		delete pTextFrameQueue;
		pTextFrameQueue = NULL;

		fprintf(stderr, "complete\n");
	}

	if(pUnusedTextFrameStack != NULL)
	{
		fprintf(stderr, "[Context]UnusedTextFrameStack delete...");
		while(pUnusedTextFrameStack->isEmpty() == false)
		{
			TextFrame *textFrame = pUnusedTextFrameStack->top();
			delete textFrame;
			pUnusedTextFrameStack->pop();
		}
		delete pUnusedTextFrameStack;
		pUnusedTextFrameStack = NULL;

		fprintf(stderr, "complete\n");
	}

	if(pVideoIO != NULL)
	{
		fprintf(stderr, "[Context]VideoIO delete...");
		delete pVideoIO;
		fprintf(stderr, "complete\n");
	}


	if(pTextFrame2PPM != NULL)
	{
		fprintf(stderr, "[Context]TextFrame2PPM delete...");
		delete pTextFrame2PPM;
		fprintf(stderr, "complete\n");
	}

	if(pFrame2TextFrame != NULL)
	{
		fprintf(stderr, "[Context]Frame2TextFrame delete...");
		delete pFrame2TextFrame;
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
