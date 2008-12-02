#include "textFrame.h"
#include <iostream>
#define RMASK 0xff0000
#define GMASK 0x00ff00
#define BMASK 0x0000ff 

TextFrame::TextFrame(Frame* _pFrame)
{
	this->originalFramePointer = _pFrame;
	
	// get width and height
	width = _pFrame->getWidth();
	height = _pFrame->getHeight();

//	printf("iw:%d ih:%d\n", width, height);	
	// set hardware params. for aalib
	aa_defparams.minwidth = this->width/2;
	aa_defparams.minheight = this->height/2;
	aa_defparams.maxwidth = this->width/2 - (this->width/2)%100 + 100;
	aa_defparams.maxheight = this->width/2 - (this->width/2)%100 + 100;

	// init context
	c = aa_init(&mem_d, &aa_defparams, NULL);
	
	if(c == NULL)
		throw "Failed to initialize aalib";

	// get RGB values from frame and put pixels with greyscale-converted value
	for(int i=0; i<width; i++)
	{
		for(int j=0; j<height; j++)
		{
			int rgbValue = _pFrame->getRGB(i,j);
		//	printf("i:%d j:%d r:%d g:%d b:%d grs:%d\n", i,j,(rgbValue&RMASK)>>16, (rgbValue&GMASK)>>8, (rgbValue&BMASK), (int)((((rgbValue&RMASK) >> 16)*0.30)+(((rgbValue&GMASK) >> 8)*0.59) + ((rgbValue&BMASK)*0.11)));
			aa_putpixel(c, i, j,(unsigned char)((((rgbValue&RMASK) >> 16)*0.30)+(((rgbValue&GMASK) >> 8)*0.59) + ((rgbValue&BMASK)*0.11)));
		}
	}
	// render into ascii art
	textWidth = aa_scrwidth(c);	// text width
	textHeight = aa_scrheight(c);	// text height
	aa_render(c, &aa_defrenderparams, 0, 0, textWidth, textHeight);
	aa_flush(c);
	
	aadata = aa_text(c);
}

TextFrame::~TextFrame()
{
	if(c != NULL)
	{
		aa_close(c);
		c = NULL;
	}
	aadata = NULL;
}

int TextFrame::getWidth(void)
{
	return this->width;
}

int TextFrame::getHeight(void)
{
	return this->height;
}

int TextFrame::getTextWidth(void)
{
	return this->textWidth;
}

int TextFrame::getTextHeight(void)
{
	return this->textHeight;
}

unsigned char* TextFrame::getText(void)
{
	return this->aadata;
}

Frame* getFramePointer(void)
{
	return this->originalFramePointer;
}

void TextFrame::print(void) // for debugging purpose
{
	std::cout << aadata << std::endl;
	std::cout << "TextWidth: " << textWidth << "TextHeight: " << textHeight << std::endl;
}

int TextFrame::main(void)
{
	///TODO : entry point
	return 0;
}
