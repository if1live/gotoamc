#pragma once

class FrameHeap;
class FrameQueue;
class FrameStack;
class TextFrameStack;
class TextFrameQueue;
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
	FrameQueue *getInputFrameQueue(void);
	FrameStack *getUnusedInputFrameStack(void);
	FrameStack *getUnusedOutputFrameStack(void);
	TextFrameQueue *getTextFrameQueue(void);
	TextFrameStack *getUnusedTextFrameStack(void);

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
	FrameQueue *pInputFrameQueue;
	FrameStack *pUnusedInputFrameStack;
	FrameStack *pUnusedOutputFrameStack;
	TextFrameQueue *pTextFrameQueue;
	TextFrameStack *pUnusedTextFrameStack;

	VideoIO *pVideoIO;
	TextFrame2PPM *pTextFrame2PPM;
	Frame2TextFrame *pFrame2TextFrame;

	int frameLimit;
};
