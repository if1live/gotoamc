#include "frame2TextFrame.h"
#include <aalib.h>

Frame2TextFrame::Frame2TextFrame()
{
	// get context
	pContext = Context::instance();
	
	// get input frame queue
	pInputFrameQueue = pContext->getInputFrameQueue();
	
	// get unused text frame stack
	pUnusedTextFrameStack = pContext->getUnusedTextFrameStack();

	// get text frame queue
	pTextFrameQueue = pContext->getTextFrameQueue();	
}

Frame2TextFrame::~Frame2TextFrame()
{
	
}

void Frame2TextFrame::main(void)
{
	conversionLoop();
	// TODO: run this with threads
}

void Frame2TextFrame::conversionLoop(void)
{
	Frame* pFrame;
	while(!pInputFrameQueue->isEmpty())
	{
		pFrame = pInputFrameQueue->pop();
		convertFrame(pFrame);	
	}
}

void Frame2TextFrame::convertFrame(void)
{
	Frame *frame = NULL;
	frame = pInputFrameQueue->pop();
	convertFrame(frame);
}

void Frame2TextFrame::convertFrame(Frame* _pFrame)
{
	TextFrame* pTextFrame;
	if(pUnusedTextFrameStack->isEmpty())
		pTextFrame = new TextFrame();
	else
		pTextFrame = pUnusedTextFrameStack->pop();

	aa_hardware_params hParams;	// hardware params for aalib
	
	aa_context *c;	// aa context

	//register font
//	aa_registerfont(aa_font8);

	// get width and height and save to TextFrame
	int width = _pFrame->getWidth();
	int height = _pFrame->getHeight();
	pTextFrame->setWidth(width);
	pTextFrame->setHeight(height);

	// set hardware params. for aalib
	hParams = aa_defparams;
//	hParams.minwidth = width/2;
//	hParams.minheight = height/2;
//	hParams.maxwidth = width/2 - (width/2)%100 + 100;
//	hParams.maxheight = width/2 - (width/2)%100 + 100;
//	hParams.minwidth = width / 2;
//	hParams.minheight = height / 2;
	hParams.width = width / 2;
	hParams.height = height / 2;
//	hParams.font = &aa_fontline;

	// copy mem_d to prevent collision
	aa_driver aa_md;
	aa_md = mem_d;

	// copy aa_defrenderparams to prevent collision
	aa_renderparams aa_rp;
	aa_rp = aa_defrenderparams;	
	
	// init context
	c = aa_init(&aa_md, &hParams, NULL);
	
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
	int textWidth = aa_scrwidth(c);
	int textHeight = aa_scrheight(c);
	pTextFrame->setTextWidth(textWidth);	// save text width
	pTextFrame->setTextHeight(textHeight);	// save text height
	aa_render(c, &aa_rp, 0, 0, textWidth, textHeight);
	aa_flush(c);

	pTextFrame->setText(aa_text(c));	// save converted text
	aa_close(c);	// close used aa context

	// push to the text frame queue
	int id = _pFrame->getId();
	pTextFrame->setId(id);
	pTextFrameQueue->push(pTextFrame);
}
