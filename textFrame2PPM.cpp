#include <cassert>
#include "queue.h"
#include "textFrame2PPM.h"
#include "context.h"
#include "frameHeap.h"


TextFrame2PPM::TextFrame2PPM()
{
	//open font ppm file : BEGIN
	char *buffer = new char[32];
	
	for(int i = 0 ; i < MAX_OF_FONTS ; i++)
	{
		fonts[i] = new Frame();	//frame of font image
		
		snprintf(buffer, 31, "res/%d.ppm", i);
		fonts[i]->openPPM(buffer);
	}
	delete[] buffer;

	//open font ppm file : END
	
	//get data structure point
	pContext = Context::instance();

	pTextFrameQueue = pContext->getTextFrameQueue();
	pUnusedTextFrameStack = pContext->getUnusedTextFrameStack();

	pOutputFrameHeap = pContext->getOutputFrameHeap();
	pUnusedOutputFrameStack = pContext->getUnusedOutputFrameStack();
	//create unused output frame
	createEmptyFrame();

	//set pointer of this
	pContext->setTextFrame2PPM(this);
}

void TextFrame2PPM::createEmptyFrame(void)
{
	//get a text frame to get text width, text height
	TextFrame *textFrame = NULL;
	///TODO
	
	while(textFrame == NULL)
	{
		if(pTextFrameQueue->isEmpty() == false)
		{
			textFrame = pTextFrameQueue->front();
			break;
		}
	}

	int frameLimit = pContext->getFrameLimit();
	for(int i = 0 ; i < frameLimit ; i++)
	{
		int width = textFrame->getTextWidth();
		int height = textFrame->getTextHeight();

		//create frame
		Frame *frame = new Frame();
		frame->setBlankFrame(WIDTH_OF_FONTS * width, HEIGHT_OF_FONTS * height);
		pUnusedOutputFrameStack->push(frame);
	}
}

int TextFrame2PPM::main(void)
{
	//entry point
	  if(pTextFrameQueue->isEmpty() == false)
	  {
	convert();
	  }
	return 0;
}

void TextFrame2PPM::convert()
{
	//get Text frame from pTextFrameQueue then convert to image, save it to pOutputFrameHeap
	TextFrame *textFrame = NULL;
	while(textFrame == NULL)
	{
		if(pTextFrameQueue->isEmpty() == false)
		{
			textFrame = pTextFrameQueue->front();
			pTextFrameQueue->pop();	//get text frame : success
		}
	}

	int height = textFrame->getTextHeight();
	int width = textFrame->getTextWidth();
	unsigned char *buffer = textFrame->getText();
//	char* testChar = " abc  bca  cab ";

	Frame *outputFrame = NULL;
	while(outputFrame == NULL)
	{
		if(pUnusedOutputFrameStack->isEmpty() == false)
		{
			outputFrame = pUnusedOutputFrameStack->top();
			pUnusedOutputFrameStack->pop();
			break;	//get empty frame : success
		}
	}
//	Frame outputFrame;
//    outputFrame.setBlankFrame( WIDTH_OF_FONTS * width, HEIGHT_OF_FONTS * height );
	
	for ( int i = 0 ; i < height ; i++ )
	{
		for ( int p = 0 ; p < HEIGHT_OF_FONTS ; p++ )
		{
			for ( int j = 0 ; j < width ; j++ )
			{
				for ( int q = 0 ; q < WIDTH_OF_FONTS ; q++ )
				{
					int rgb = fonts[buffer[i*width + j]]->getRGB(q, p);
					unsigned char value = 0x000000ff & rgb;

					outputFrame->setGrey( q + j*WIDTH_OF_FONTS, p + i*HEIGHT_OF_FONTS, value );
				}
			}
		}
	}
	
	pUnusedTextFrameStack->push(textFrame);
	pOutputFrameHeap->push(outputFrame);	//save converted frame
}

TextFrame2PPM::~TextFrame2PPM()
{
	//free font image
	for ( int i = 0 ; i < MAX_OF_FONTS ; i++ )
		delete fonts[i];
}


