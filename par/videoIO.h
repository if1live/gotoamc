#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <avcodec.h>
#include <avformat.h>
#include <stdio.h>


#ifdef __cplusplus
}
#endif

class Frame;
class FrameHeap;
class Context;

template <class T> class Queue;
template <class T> class Stack;

class VideoIO
{
public:
	//ctor
	VideoIO();

	//dtor
	~VideoIO();

	int main(void);	//thread's function
	bool readFrame(void);	//read a frame from video then save to pFrame
	bool writeFrame(void);	//write a frame
	bool saveFrame(Frame *_pFrame);	
	bool isReadingComplete(void);
	bool init(int argc, char *argv[]);	//initialize codec
	void requestToWrite(int count);	//request to this class(videoIO) to write frame
private:
	//member function
	int getFrameIndex(void);
	void incFrameIndex(void);

	bool validateArg(int argc, char *argv[]);

	bool openInputCodec(void);
	bool openOutputCodec(int _width, int _height);

	char *inputFilename;
	char *outputFilename;

	void RGB24ToYUV420P(AVFrame *_src, int _width, int _height);
	void YUV420PToRGB24(AVFrame *_dst, int _width, int _height);

	//member variable
	AVFormatContext *pFormatCtx;
	
	AVCodec *pInputCodec;
	AVCodec *pOutputCodec;
	
	AVCodecContext *pInputCodecCtx;
	AVCodecContext *pOutputCodecCtx;

	AVPacket packet;

	int frameIndex;	//id of frame
	int videoStream;
	int frameFinished;

	Queue<Frame *> *pInputFrameQueue;	//save read frame from video
	FrameHeap *pOutputFrameHeap;	//save writing frame to video

	Stack<Frame *> *pUnusedInputFrameStack;	//available frame(use this then push to pInputQueue)
	Stack<Frame *> *pUnusedOutputFrameStack;	//available frame(use pOutputHeap then push to this)

	Context *pContext;
	AVFrame *pInputFrame;
	AVFrame *pOutputFrame;

	FILE *pOutputFile;

	bool continueReadFrame;	//true : read frame, false : write frame
	uint8_t *pOutBuffer;
	int outBufferSize;
	int outSize;

	bool readComplete;
	int writingRequested;
};

