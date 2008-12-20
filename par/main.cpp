#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "videoIO.h"
#include "textFrame2PPM.h"
#include "frame2TextFrame.h"
#include "context.h"

void *videoIOThr(void *arg);
void *frame2TextFrameThr(void *arg);
void *textFrame2PPMThr(void *arg);

pthread_t thrs[3];

int main(int argc, char *argv[])
{
	Context *context = Context::instance();	//create global data

	VideoIO *videoIO = new VideoIO();
	Frame2TextFrame *frame2TextFrame = new Frame2TextFrame();
	TextFrame2PPM *textFrame2PPM = new TextFrame2PPM();

	//initialize ffmpeg codec
	videoIO->init(argc, argv);

	//create thread
	pthread_create(&thrs[0], NULL, videoIOThr, (void *)videoIO);
//	sleep(5);
	pthread_create(&thrs[1], NULL, frame2TextFrameThr, (void *)frame2TextFrame);
//	sleep(5);
	pthread_create(&thrs[2], NULL, textFrame2PPMThr, (void *)textFrame2PPM);

	//join thread
	for(int i = 2 ; i >= 0 ; i--)
		pthread_join(thrs[i], NULL);


	//	while(videoIO->isReadingComplete() == false)
	//	for(int i = 0 ; i < 30 ; i++)
	//	{
	//		try
	//		{
	//			videoIO->readFrame();
	//			frame2TextFrame->convertFrame();
	//			textFrame2PPM->convert();
	//			videoIO->writeFrame();
	//		}
	//		catch(const char *msg)
	//		{
	//			fprintf(stderr, "Except : %s\n", msg);
	//			delete context;
	//			return 1;
	//		}
	//	}
	delete context;	//delete context data

	return 0;
}


void *videoIOThr(void *arg)
{
	try {
		VideoIO *videoIO = (VideoIO *) arg;
		videoIO->main();
	} catch(const char *msg) {
		fprintf(stderr, "[VideoIO thr] Except : %s\n", msg);
	}
	return NULL;
}

void *frame2TextFrameThr(void *arg)
{
	try {
		Frame2TextFrame *frame2TextFrame = (Frame2TextFrame *) arg;
		frame2TextFrame->main();
	} catch(const char *msg) {
		fprintf(stderr, "[Frame2TextFrame thr] Except : %s\n", msg);
	}
	return NULL;
}

void *textFrame2PPMThr(void *arg)
{
	try {
		TextFrame2PPM *textFrame2PPM = (TextFrame2PPM *) arg;
		//create empty frame
		textFrame2PPM->createEmptyFrame();
		//start main!
		textFrame2PPM->main();
	} catch(const char *msg) {
		fprintf(stderr, "[TextFrame2PPM thr] Except : %s\n", msg);
	}
	return NULL;
}


