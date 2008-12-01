#include <cassert>
#include "textFrame2PPM.h"
#include "context.h"
#include "frameStack.h"
#include "frameHeap.h"
#include "textFrameStack.h"
#include "textFrameQueue.h"

TextFrame2PPM::TextFrame2PPM()
{
	//open font ppm file : BEGIN
	for(int i = 0 ; i < 128 ; i++)
	{
		fonts[i] = new Frame();	//frame of font image
		
		char *buffer = new char[32];
		snprintf(buffer, sizeof(buffer), "src/%d.ppm", i);
		fonts[i]->openP3PPM(buffer);
		delete[] buffer;
	}
/*	for (int i = 97 ; i < 100 ; i++ )
    {
		fonts[i] = new Frame();
		
		char* buff = new char[32];
		snprintf( buff, sizeof( buff ), "%d.ppm", i );
		fonts[i]->openP3PPM( buff );
		delete[] buff;
    }
	
	fonts[32] = new Frame();
	
	char* buff = new char[32];
	snprintf( buff, sizeof( buff ), "%d.ppm", 32 );
	fonts[32]->openP3PPM( buff );
	delete[] buff;
*/
	//open font ppm file : END
	
	//get data structure point
	pContext = Context::instance();
	pTextFrameQueue = pContext->getTextFrameQueue();
	pUnusedTextFrameStack = pContext->getUnusedTextFrameStack();
	pOutputFrameHeap = pContext->getOutputFrameHeap();
	pUnusedOutputFrameStack = pContext0>getUnusedOutputFrameStack();
}

int TextFrame2PPM::main(void)
{
	//entry point
	if(pTextFrameQueue->isEmpty() == false)
	{
		convert();
	}

	return NULL;
}

void TextFrame2PPM::convert()
{
	//get Text frame from pTextFrameQueue then convert to image, save it to pOutputFrameHeap
	TextFrame *textFrame = NULL;
	while(textFrame == NULL)
	{
		if(pTextFrameQueue->isEmpty() == false)
		{
			textFrame = pTextFrameQueue->top();
			pTextFrameQueue->pop();	//get text frame : success
		}
	}

	int height = textFrame->getHeight();	//3
	int width = textFrame->getWidth();	//5
//	char* testChar = " abc  bca  cab ";

	Frame *outputFrame = NULL;
	while(outputFrame == NULL)
	{
		if(pUnusedOutputFrameQueue->isEmpty() == false)
		{
			outputFrame = pUnusedOutputFrameQueue->front();
			pUnusedOutputFrameQueue->pop();
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
					int rgb = fonts[testChar[i*width + j]]->getRGB(q, p);
					unsigned char value = 0x000000ff & rgb;
					outputFrame->setGrey( q, p, value );
				}
			}
		}
	}
	
	pUnusedTextFrameStack->push(textFrame);
	pOutputFrameHeap->push(outputFrame);	//save converted frame
//	outputFrame.saveP3PPM( "output.ppm" );
}

TextFrame2PPM::~TextFrame2PPM()
{
	//free font image
	for ( int i = 97 ; i < 100 ; i++ )
		delete fonts[i];
	delete fonts[32]; 
}


