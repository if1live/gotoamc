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
	frameLimit = 40;
	convertingRange = 800;
	pOutputFrameHeap = new FrameHeap(frameLimit*2);
	pInputFrameQueue = new Queue<Frame *>(frameLimit*2);
	pUnusedInputFrameStack = new Stack<Frame *>(frameLimit*2);
	pUnusedOutputFrameStack = new Stack<Frame *>(frameLimit*2);
	pTextFrameQueue = new Queue<TextFrame *>(frameLimit*2);
	pUnusedTextFrameStack = new Stack<TextFrame *>(frameLimit*2);

	pVideoIO = NULL;
	pTextFrame2PPM = NULL;
	pFrame2TextFrame = NULL;
	width = -1;
	height = -1;
}

Context::~Context()
{
	if(pOutputFrameHeap != NULL)
	{
		fprintf(stderr, "[Context]OutputFrameHeap delete...");
		//	assert(pOutputFrameHeap->isEmpty() == true && "OutputFrameHeap is not empty!");
		while(pOutputFrameHeap->isEmpty() == false)
		{
			//write remain frame
			pVideoIO->writeFrame();
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

int Context::getWidth(void)
{
	assert(this->width > 1 && "width is below 0");

	return this->width;
}

int Context::getHeight(void)
{
	assert(this->width > 1 && "height is below 0");

	return this->height;
}

void Context::setWidth(int _width)
{
	this->width = _width;
}


void Context::setHeight(int _height)
{
	this->height = _height;
}


int Context::getConvertingRange(void)
{
	return convertingRange;
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
