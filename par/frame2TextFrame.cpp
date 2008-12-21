#include "frame2TextFrame.h"
#include <aalib.h>

Frame2TextFrame::Frame2TextFrame()
{
	endFlag = false;
	//how many frame calculated? and required lock
	index = 0;
	pthread_mutex_init(&indexLock, NULL);
	pthread_mutex_init(&lock1, NULL);
	pthread_mutex_init(&lock2, NULL);

	// get context
	pContext = Context::instance();
	
	// get input frame queue
	pInputFrameQueue = pContext->getInputFrameQueue();
	
	// get unused text frame stack
	pUnusedTextFrameStack = pContext->getUnusedTextFrameStack();

	// get text frame queue
	pTextFrameQueue = pContext->getTextFrameQueue();	

	pUnusedInputFrameStack = pContext->getUnusedInputFrameStack();

	//create text frame
	int limit = pContext->getFrameLimit();
	for(int i = 0 ; i < limit ; i++)
	{
		TextFrame* pTextFrame;
		pTextFrame = new TextFrame();
		pUnusedTextFrameStack->push(pTextFrame);
	}
}

void Frame2TextFrame::incIndex(void)
{
	pthread_mutex_lock(&indexLock);
	index++;
	pthread_mutex_unlock(&indexLock);
}

Frame2TextFrame::~Frame2TextFrame()
{
	pthread_mutex_destroy(&lock1);
	pthread_mutex_destroy(&lock2);
	pthread_mutex_destroy(&indexLock);
}

void Frame2TextFrame::main(void)
{
	int range = pContext->getConvertingRange();
	while(index < range)
	{
		convertFrame();
		incIndex();
	}

	endFlag = true;
	fprintf(stderr, "[Frame2TextFrame] Exit <<<<<<<<<<<<<<<<<<\n");
}

void Frame2TextFrame::convertFrame(void)
{
	bool readComplete = false;
	while(readComplete == false && endFlag == false)
	{
		Frame *frame = NULL;
		//locked area - START
		pthread_mutex_lock(&lock1);
		if(pInputFrameQueue->isEmpty() == false)
		{
			frame = pInputFrameQueue->pop();
		}
		pthread_mutex_unlock(&lock1);
		//locked area - END
		
		if(frame != NULL)
		{
			convertFrame(frame);
			pUnusedInputFrameStack->push(frame);
			readComplete = true;
		}
	}
}

void Frame2TextFrame::convertFrame(Frame* _pFrame)
{
	//get unused text frame from stack
	TextFrame *pTextFrame = NULL;
	while(pTextFrame == NULL)
	{
		//locked area - start
		pthread_mutex_lock(&lock2);
		
		if(pUnusedTextFrameStack->isEmpty() == false)
		{
			pTextFrame = pUnusedTextFrameStack->pop();
		}

		pthread_mutex_unlock(&lock2);
		//locked area - end
	}

	///DEBUG

	aa_hardware_params hParams;	// hardware params for aalib
	aa_context *c;	// aa context
//	aa_recommendhidisplay("teste1");

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

	pTextFrame->setText((const char*)aa_text(c));	// save converted text
	aa_close(c);	// close used aa context

	// push to the text frame queue
	int id = _pFrame->getId();
	pTextFrame->setId(id);
	pTextFrameQueue->push(pTextFrame);
	
	fprintf(stderr, "[Frame2TextFrame] to ascii art : #%d\n", _pFrame->getId());
}
