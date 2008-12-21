#include <stdio.h>
#include "videoIO.h"
#include "context.h"
#include "frame.h"
#include "queue.h"
#include "frameHeap.h"
#include "stack.h"

int main(int argc, char *argv[])
{
	Context *context = Context::instance();	//create global data

	VideoIO *videoIO = new VideoIO();

	videoIO->init(argc, argv);
	
	Queue<Frame *> *fromQueue = context->getInputFrameQueue();
	FrameHeap *toHeap = context->getOutputFrameHeap();
	Stack<Frame *> *unusedInputFrameStack = context->getUnusedInputFrameStack();
	Stack<Frame *> *unusedOutputFrameStack = context->getUnusedOutputFrameStack();

	for(int i = 0 ; i < context->getFrameLimit() ; i++)
	{
		try
		{
			videoIO->readFrame();
			//move frame
			Frame *frame = fromQueue->pop();
			toHeap->push(frame);

			videoIO->writeFrame();
			
			//move frame
			frame = unusedOutputFrameStack->pop();
			unusedInputFrameStack->push(frame);
		}
		catch(const char *msg)
		{
			fprintf(stderr, "Except : %s\n");
			delete context;
			return 1;
		}
	}
	delete context;	//delete context data

	return 0;
}
