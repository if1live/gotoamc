#include "context.h"
#include "frameQueue.h"
#include "textFrameQueue.h"
#include "textFrameStack.h"

class FrameQueue;
class TextFrameQueue;
class TextFrameStack;

void Frame2TextFrame::main(void)
{
	Context* context;
	FrameQueue* frameQueue;
	TextFrameStack* unusedTextFrameStack;
	TextFrameQueue* textFrameQueue;

	Frame* frame;
	TextFrame* textFrame;

	context = TextFrameQueue::instance();

	// get frame queue
	frameQueue = context->getInputFrameQueue();
	
	// get unused text frame stack
	unusedTextFrameStack = context->getUnusedTextFrameStack();
	
	// get frame
	frame = frameQueue->pop();
	
	// get text frame
}
