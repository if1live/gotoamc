#include "frame2TextFrame.h"
#include "context.h"
#include "textFrameStack.h"
#include "queue.h"

Frame2TextFrame::Frame2TextFrame()
{
	///TODO
}

Frame2TextFrame::~Frame2TextFrame()
{
	///TODO
}

void Frame2TextFrame::main(void)
{
	Context* context;
	Queue<Frame *>* frameQueue;
	TextFrameStack* unusedTextFrameStack;
	Queue<TextFrame *>* textFrameQueue;

	Frame* frame;
	TextFrame* textFrame;

	context = Context::instance();

	// get frame queue
	frameQueue = context->getInputFrameQueue();
	
	// get unused text frame stack
	unusedTextFrameStack = context->getUnusedTextFrameStack();
	
	// get frame
	frame = frameQueue->pop();
	
	// get text frame
}
