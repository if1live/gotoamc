#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <avcodec.h>
#include <avformat.h>
#include <swscale.h>
#include <stdio.h>

#ifdef __cplusplus
}
#endif

class Frame;
class FrameHeap;
class FrameQueue;
class FrameStack;

class VideoIO
{
public:
	//ctor
	VideoIO();

	//dtor
	~VideoIO();

	int main(int argc, char *argv[]);	//thread's function
	bool readFrame(void);	//read a frame from video then save to pFrame
	bool writeFrame(void);	//write a frame
	bool saveFrame(Frame *_pFrame, int _width, int _height, int _id);
	
	
private:
	//member function
	int getFrameIndex(void);
	void incFrameIndex(void);

	bool validateArg(int argc, char *argv[]);

	bool openInputCodec(char *_filename);
	bool openOutputCodec(char *_filename, int _width, int _height);

	void RGB24ToYUV420P(AVFrame *_src, int _width, int _height);

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

	struct SwsContext *pInputImgConvertCtx;

	///TODO : change data struct
	FrameHeap *pInputFrameQueue;	//save read frame from video
	FrameHeap *pOutputFrameHeap;	//save writing frame to video

	///TODO : change to stack
	FrameHeap *pUnusedInputFrameStack;	//available frame(use this then push to pInputQueue)
	FrameHeap *pUnusedOutputFrameStack;	//available frame(use pOutputHeap then push to this)

	//this is test code
	AVFrame *pInputFrame;
	AVFrame *pOutputFrame;

	FILE *pOutputFile;

	bool continueReadFrame;	//true : read frame, false : write frame
	uint8_t *pOutBuffer;
	int outBufferSize;
	int outSize;

	bool readComplete;
};

