#pragma once
#include "frame.h"
#include <aalib.h>

class FrameQueue;
class FrameStack;

class TextFrame
{
public:
	//ctor
	TextFrame(Frame* _pFrame);	// create text-converted frame
	int getWidth(void);
	int getHeight(void);
	int getTextWidth(void);
	int getTextHeight(void);
	unsigned char* getText(void);	// get text!
	void print(void);	// for debugging purpose

	void setInputFrameQueue(FrameQueue *_ptr);
	void setUnusedInputFrameStack(FrameStack *_ptr);
	
	int main(void);	//entry point

	//dtor
	~TextFrame();
private:
	aa_hardware_params hParams;	// hardware params for aalib
	aa_context *c;	// aa context
	unsigned char *aadata;	// converted data
	int width;	//width of original frame
	int height;	//height of original frame
	int textWidth;		// aa width
	int textHeight;		// aa height

	//communicate with VideoIO
	FrameQueue *pInputFrameQueue;
	FrameStack *pUnusedInputFrameStack;

	//TODO : communicate with (textFrame(ascii) -> frame(image))
	//
};
