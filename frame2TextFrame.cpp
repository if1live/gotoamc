#include "frame2TextFrame.h"
#include "context.h"
#include "queue.h"
#include "stack.h"

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
	Stack<TextFrame *>* unusedTextFrameStack;
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
