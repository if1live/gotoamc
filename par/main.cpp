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
bool validateArg(int argc, char *argv[]);

pthread_t thrs[16];	//maxminal thread
int threadNum;	//how many thread to use?

int main(int argc, char *argv[])
{
	//validating argument
	if(validateArg(argc, argv) == false)
		exit(EXIT_FAILURE);

	Context *context = Context::instance();	//create global data

	VideoIO *videoIO = new VideoIO();
	Frame2TextFrame *frame2TextFrame = new Frame2TextFrame();
	TextFrame2PPM *textFrame2PPM = new TextFrame2PPM();

	//initialize ffmpeg codec
	videoIO->init(argc, argv);

	//create empty frame
	textFrame2PPM->createEmptyFrame();

	//create thread
	//first thread is videoIO's thread
	//this program needs more than 3 thread.
	pthread_create(&thrs[0], NULL, videoIOThr, (void *)videoIO);

	//other thread
	for(int i = 1 ; i < threadNum ; i++)
	{
		if(i % 2 == 1)
			pthread_create(&thrs[i], NULL, frame2TextFrameThr, (void *)frame2TextFrame);
		else
			pthread_create(&thrs[i], NULL, textFrame2PPMThr, (void *)textFrame2PPM);
	}

	//join thread
	//step1. join frame2textFrame
	for(int i = 1 ; i < threadNum ; i = i+2)
		pthread_join(thrs[i], NULL);
	//step2. join textFram2PPM
	for(int i = 2 ; i < threadNum ; i = i+2)
		pthread_join(thrs[i], NULL);
	//step3. joint videoIO
	fprintf(stderr, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
	pthread_join(thrs[0], NULL);
	fprintf(stderr, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");

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
		//start main!
		textFrame2PPM->main();
	} catch(const char *msg) {
		fprintf(stderr, "[TextFrame2PPM thr] Except : %s\n", msg);
	}
	return NULL;
}


bool validateArg(int argc, char *argv[])
{
	//validation commandline argument
	if(argc != 4)
	{
		printf("Usage : %s <input video> <output video> <thread num, 3~16>\n", argv[0]);
		return false;
	}
	
	//else
	threadNum = atoi(argv[3]);

	if(threadNum < 3 || threadNum > 16)
	{
		printf("Error : not valid thread number, %d\n", threadNum);
		return false;
	}
	return true;
}
