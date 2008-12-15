#pragma once
#include "frame.h"
#include "context.h"
#include "queue.h"
#include "stack.h"
#include "textFrame.h"
#define RMASK 0xff0000
#define GMASK 0x00ff00
#define BMASK 0x0000ff

class Frame2TextFrame
{
public:
	Frame2TextFrame();
	~Frame2TextFrame();
	void main(void); // thread ops
	void conversionLoop(void);	//routine to get items from queue and convert it till queue is empty
	void convertFrame(Frame* _pFrame);	// main routine for single frameconversion

private:
	Context* pContext;
	Queue<Frame *>* pInputFrameQueue;
	Stack<TextFrame *>* pUnusedTextFrameStack;
	Queue<TextFrame *>* pTextFrameQueue;
};

