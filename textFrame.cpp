#define ENABLE_ANSI	// set this to enable ANSI extension
#include "textFrame.h"
#include <iostream>
#define RMASK 0xff0000
#define GMASK 0x00ff00
#define BMASK 0x0000ff 

TextFrame::TextFrame(Frame* _pFrame)
{
	// get width and height
	width = _pFrame->getWidth();
	height = _pFrame->getHeight();
	
	// set hardware params. for aalib
	/*hParams.font = NULL; //default value

	hParams.supported = AA_EXTENDED;

	hParams.minwidth = width;
	hParams.minheight = height;
	hParams.maxwidth = width;
	hParams.maxheight = height;
	hParams.width = width;
	hParams.height = height;
	hParams.recwidth = width;
	hParams.recheight = height;
	*/

	aa_savedata asv;
	asv.name = "aaout";
	asv.format = &aa_text_format;
	asv.file = NULL;

	// init context
	context = aa_init(&mem_d, &aa_defparams, &asv);
	
	if(context == NULL)
		throw "Failed to initialize aalib";

	// get RGB values from frame and put pixels with greyscale-converted value
	for(int i=0; i<width; i++)
	{
		for(int j=0; j<height; j++)
		{
			int rgbValue = _pFrame->getRGB(i,j);
			printf("i:%d j:%d r:%d g:%d b:%d grs:%d\n", i,j,(rgbValue&RMASK)>>16, (rgbValue&GMASK)>>8, (rgbValue&BMASK), (int)((((rgbValue&RMASK) >> 16)*0.30)+(((rgbValue&GMASK) >> 8)*0.59) + ((rgbValue&BMASK)*0.11)));
			aa_putpixel(context, i, j,(unsigned char)((((rgbValue&RMASK) >> 16)*0.30)+(((rgbValue&GMASK) >> 8)*0.59) + ((rgbValue&BMASK)*0.11)));
		}
	}
	printf("##width %d height %d\n", width, height);
	
	// render into ascii art
	textWidth = aa_scrwidth(context);	// text width
	textHeight = aa_scrheight(context);	// text height
	
	printf("##POINT BEFORE RENDER: w %d h %d\n", textWidth, textHeight);
	aa_render(context, &aa_defrenderparams, 0, 0, textWidth, textHeight);
	printf("###RENDER DONE\n");
	aa_flush(context);

	aadata = aa_text(context);
}

TextFrame::~TextFrame()
{
	if(context != NULL)
	{
		aa_close(context);
		//context = NULL;
	}
	//aadata = NULL;
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

void TextFrame::print(void) // for debugging purpose
{
	std::cout << aadata << std::endl;
	std::cout << "TextWidth: " << textWidth << "TextHeight: " << textHeight << std::endl;
}
