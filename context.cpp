#include <stdio.h>
#include "context.h"
#include "frameHeap.h"
#include "frameQueue.h"
#include "frameStack.h"
#include "textFrameStack.h"
#include "textFrameQueue.h"
#include "frame.h"
#include "textFrame.h"

Context::Context()
{
	frameLimit = 30;
	pOutputFrameHeap = new FrameHeap(frameLimit);
	pInputFrameQueue = new FrameQueue(frameLimit);
	pUnusedInputFrameStack = new FrameStack(frameLimit);
	pUnusedOutputFrameStack = new FrameStack(frameLimit);
	pTextFrameQueue = new TextFrameQueue(frameLimit);
	pUnusedTextFrameStack = new TextFrameStack(frameLimit);

	pVideoIO = NULL;
	pTextFrame2PPM = NULL;
	pFrame2TextFrame = NULL;
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
			TextFrame *textFrame = pTextFrameQueue->front();
			delete textFrame;
			pTextFrameQueue->pop();
		}
		delete pTextFrameQueue;
		pTextFrameQueue = NULL;
	}

	if(pUnusedTextFrameStack != NULL)
	{
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
		delete pVideoIO;
	}


	if(pTextFrame2PPM != NULL)
	{
		delete pTextFrame2PPM;
	}

	if(pFrame2TextFrame != NULL)
	{
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


VideoIO *Context::getVideoIO(void)
{
	while(pVideoIO == NULL)
	{
		;	//wait 
	}
	return pVideoIO;
}


TextFrame2PPM *Context::getTextFrame2PPM(void)
{
	while(pTextFrame2PPM == NULL)
	{
		;	//wait
	}
	return pTextFrame2PPM;
}


Frame2TextFrame *Context::getFrame2TextFrame(void)
{
	while(pFrame2TextFrame == NULL)
	{
		;	//wait
	}
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
