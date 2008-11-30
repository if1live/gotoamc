#pragma once
#include "frame.h"
#include <aalib.h>

class TextFrame
{
public:
	//ctor
	TextFrame(Frame* _pFrame);	// create text-converted frame
	int getWidth(void);
	int getHeight(void);
	int getTextWidth(void);
	int getTextHeight(void);
	void print(void);	// for debugging purpose

	//dtor
	~TextFrame();
private:
	aa_hardware_params hParams;	// hardware params for aalib
	aa_context *context;	// aa context
	unsigned char *aadata;	// converted data
	int width;	//width of original frame
	int height;	//height of original frame
	int textWidth;		// aa width
	int textHeight;		// aa height
};
