#include <cassert>
#include "queue.h"
#include "textFrame2PPM.h"
#include "context.h"
#include "frameHeap.h"

TextFrame2PPM::TextFrame2PPM()
{
	ratio = 4;	//resize factor	
	textBuffer = NULL;
	//open font ppm file : BEGIN
	char *buffer = new char[32];
	
	for(int i = 0 ; i < MAX_OF_FONTS ; i++)
	{
		fonts[i] = new Frame();	//frame of font image
		
		snprintf(buffer, 31, "../res/%d.ppm", i);
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
//	createEmptyFrame();

	//set pointer of this
	pContext->setTextFrame2PPM(this);

	isFirstRun = true;
}

void TextFrame2PPM::createEmptyFrame(void)
{
	//DEBUG
	printf("[TextFrame2PPM] create Empty Frame....");

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

	int width;
	int height;
	for(int i = 0 ; i < 10 ; i++)
	{
		width = textFrame->getTextWidth() / ratio;
		height = textFrame->getTextHeight() / ratio;

		//create frame
		Frame *frame = new Frame();
		frame->setBlankFrame(WIDTH_OF_FONTS * width, HEIGHT_OF_FONTS * height);
		pUnusedOutputFrameStack->push(frame);
	}

	textBuffer = new unsigned char [height * width];
	memset(textBuffer, 0, height * width * sizeof(unsigned char));

	//DEBUG
	printf("complete\n");
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
	if(isFirstRun == true)
	{
		createEmptyFrame();
		isFirstRun = false;
	}


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

	//DEBUG
	printf("[TextFrame2PPM] #%d convert...", textFrame->getId());

	int height = textFrame->getTextHeight();
	int width = textFrame->getTextWidth();
	unsigned char *buffer = textFrame->getText();
	//	char* testChar = " abc  bca  cab "
	
	//resize text frame(buffer)
	int pos = 0;
	for(int y = 0 ; y < height ; y++)
	{
		for(int x = 0 ; x < width ; x++)
		{
			if(y % ratio == 0 && x % ratio == 0)
			{
				textBuffer[pos] = *(buffer + y*width + x);
				pos++;
			}
		}
	}
	height = height / ratio;
	width = width / ratio;


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
					//int rgb = fonts[buffer[i*width + j]]->getRGB(q, p);
					int rgb = fonts[textBuffer[i*width + j]]->getRGB(q, p);
					unsigned char value = 0x000000ff & rgb;
					
					outputFrame->setGrey( q + j*WIDTH_OF_FONTS, p + i*HEIGHT_OF_FONTS, value );
				}
			}
		}
	}

	//save frame to data structure
	int id = textFrame->getId();
	outputFrame->setId(id);

	pUnusedTextFrameStack->push(textFrame);
	pOutputFrameHeap->push(outputFrame);	//save converted frame

	//DEBUG
	printf("complete\n");
}

TextFrame2PPM::~TextFrame2PPM()
{
	//free font image
	for ( int i = 0 ; i < MAX_OF_FONTS ; i++ )
		delete fonts[i];

	delete textBuffer;
}


