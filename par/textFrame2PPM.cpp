#include <cassert>
#include "videoIO.h"
#include "queue.h"
#include "textFrame2PPM.h"
#include "context.h"
#include "frameHeap.h"

TextFrame2PPM::TextFrame2PPM()
{
	endFlag = false;
	index = 0;
	pthread_mutex_init(&indexLock, NULL);
	pthread_mutex_init(&lock1, NULL);
	pthread_mutex_init(&lock2, NULL);

	outputFrameCount = 0;
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
}

void TextFrame2PPM::createEmptyFrame(void)
{
	//DEBUG
	printf("[TextFrame2PPM] create Empty Frame....");

	//create unused output frame
	int frameLimit = pContext->getFrameLimit();
	int width = pContext->getWidth() / 2 / ratio;
	int height = pContext->getHeight() / 2 / ratio;
	for(int i = 0 ; i < frameLimit*2 ; i++)
	{
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
	int range = pContext->getConvertingRange();
	while(index < range)
	{
		convert();
		incIndex();
	}
	
	VideoIO *videoIO = pContext->getVideoIO();
	videoIO->requestToWrite(-1);

	fprintf(stderr, "[TextFrame2PPM] Exit <<<<<<<<<<<<<<<\n");
	endFlag = true;
	return 0;
}

void TextFrame2PPM::convert()
{
	//request to write frame
	int limit = pContext->getFrameLimit();
	if(outputFrameCount >= limit)
	{
		VideoIO *videoIO = pContext->getVideoIO();
		videoIO->requestToWrite(outputFrameCount);
		outputFrameCount = 0;
	}

	//get Text frame from pTextFrameQueue then convert to image, save it to pOutputFrameHeap
	TextFrame *textFrame = NULL;
	while(textFrame == NULL)
	{
		//fprintf(stderr, "%d ", outputFrameCount);
		//locked area - start
		pthread_mutex_lock(&lock1);
		if(pTextFrameQueue->isEmpty() == false)
		{
			textFrame = pTextFrameQueue->pop();
		}
		pthread_mutex_unlock(&lock1);
		//locked area - end
		//
		if(endFlag == true)
			return;
	}
	//	pTextFrameQueue->pop();	//get text frame : success

	//DEBUG

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

	//get unused frame
	Frame *outputFrame = NULL;
	while(outputFrame == NULL)
	{
		//locked area - start
		pthread_mutex_lock(&lock2);
		if(pUnusedOutputFrameStack->isEmpty() == false)
		{
			outputFrame = pUnusedOutputFrameStack->pop();
		}
		pthread_mutex_unlock(&lock2);
		//locked area - end
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

	delete buffer;
	textFrame->setText(NULL);

	pUnusedTextFrameStack->push(textFrame);
	pOutputFrameHeap->push(outputFrame);	//save converted frame
	outputFrameCount++;

	//DEBUG
	fprintf(stderr, "[TextFrame2PPM] #%d convert\n", textFrame->getId());
}

TextFrame2PPM::~TextFrame2PPM()
{
	//free font image
	for ( int i = 0 ; i < MAX_OF_FONTS ; i++ )
		delete fonts[i];

	delete textBuffer;

	//delete mutex
	pthread_mutex_destroy(&indexLock);
	pthread_mutex_destroy(&lock1);
	pthread_mutex_destroy(&lock2);
}

void TextFrame2PPM::incIndex(void)
{
	pthread_mutex_lock(&indexLock);
	index++;
	pthread_mutex_unlock(&indexLock);
}


