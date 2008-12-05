#pragma once

#include "queue.h"
#include "stack.h"

class Frame;
class TextFrame;
class FrameHeap;
class VideoIO;
class TextFrame2PPM;
class Frame2TextFrame;


class Context	//save global variable, define,....
{
public:
	static Context *instance(void);

	//dtor
	~Context();

	//getter and setter
	FrameHeap *getOutputFrameHeap(void);
	Queue<Frame *> *getInputFrameQueue(void);
	Stack<Frame *> *getUnusedInputFrameStack(void);
	Stack<Frame *> *getUnusedOutputFrameStack(void);
	Queue<TextFrame *> *getTextFrameQueue(void);
	Stack<TextFrame *> *getUnusedTextFrameStack(void);

	VideoIO *getVideoIO(void);
	TextFrame2PPM *getTextFrame2PPM(void);
	Frame2TextFrame *getFrame2TextFrame(void);

	void setVideoIO(VideoIO *_ptr);
	void setTextFrame2PPM(TextFrame2PPM *_ptr);
	void setFrame2TextFrame(Frame2TextFrame *_ptr);

	int getFrameLimit(void);
private:
	//ctor
	Context();

	static Context *context;

	FrameHeap *pOutputFrameHeap;
	Queue<Frame *> *pInputFrameQueue;
	Stack<Frame *> *pUnusedInputFrameStack;
	Stack<Frame *> *pUnusedOutputFrameStack;
	Queue<TextFrame *> *pTextFrameQueue;
	Stack<TextFrame *> *pUnusedTextFrameStack;

	VideoIO *pVideoIO;
	TextFrame2PPM *pTextFrame2PPM;
	Frame2TextFrame *pFrame2TextFrame;

	int frameLimit;
};

