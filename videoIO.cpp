#include <cstdlib>
#include <stdio.h>
#include "constNum.h"
#include "videoIO.h"
#include "frame.h"
#include "frameHeap.h"
#include "frameQueue.h"
#include "frameStack.h"

VideoIO::VideoIO()
{
	frameIndex = 0;

	//register all formats and codec
	av_register_all();

	//allocate video frame...use like buffer
	//type : YUV420P
	pInputFrame = avcodec_alloc_frame();
	pOutputFrame = NULL;

	continueReadFrame = true;

	//set pointer to null
	pInputFrameQueue = NULL;
	pOutputFrameHeap = NULL;
	pUnusedInputFrameStack = NULL;
	pUnusedOutputFrameStack = NULL;

	pInputImgConvertCtx = NULL;

	//set variable
	outSize = -1;
	pOutBuffer = NULL;
	outBufferSize = -1;
	readComplete = false;
}

VideoIO::~VideoIO()
{
	//delete data struct
	if(pInputFrameQueue != NULL)
	{
		while(pInputFrameQueue->isEmpty() == false)
		{
			Frame *frame = pInputFrameQueue->front();
			delete frame;
			pInputFrameQueue->pop();
		}
		delete pInputFrameQueue;
	}

	if(pOutputFrameHeap != NULL)
	{
		while(pOutputFrameHeap->isEmpty() == false)
		{
			Frame *frame = pOutputFrameHeap->top();
			delete frame;
			pOutputFrameHeap->pop();
		}
		delete pOutputFrameHeap;
	}

	if(pUnusedInputFrameStack != NULL)
	{
		while(pUnusedInputFrameStack->isEmpty() == false)
		{
			Frame *frame = pUnusedInputFrameStack->top();
			delete frame;
			pUnusedInputFrameStack->pop();
		}
		delete pUnusedInputFrameStack;
	}

	if(pUnusedOutputFrameStack != NULL)
	{
		while(pUnusedOutputFrameStack->isEmpty() == false)
		{
			Frame *frame = pUnusedOutputFrameStack->top();
			delete frame;
			pUnusedOutputFrameStack->pop();
		}
		delete pUnusedOutputFrameStack;
	}
	
	//delete img convert
	if(pInputImgConvertCtx != NULL)
	{
		sws_freeContext(pInputImgConvertCtx);
	}

	//close output video
	if(outSize != -1)
	{
		//get the delayed frames
		for(int i = getFrameIndex() ; outSize ; i++)
		{
			fflush(stdout);
			outSize = avcodec_encode_video(pOutputCodecCtx, pOutBuffer, outBufferSize, NULL);
			printf("write frame %3d (size=%5d)\n", i, outSize);
			fwrite(pOutBuffer, 1, outSize, pOutputFile);
		}
		
		//add sequence end code to have a real mpeg file
		pOutBuffer[0] = 0x00;
		pOutBuffer[1] = 0x00;
		pOutBuffer[2] = 0x01;
		pOutBuffer[3] = 0xb7;
		fwrite(pOutBuffer, 1, 4, pOutputFile);
	}

	//free the rgb image
	av_free(pInputFrame);

	free(pOutputFrame->data[0]);
	av_free(pOutputFrame);

	//close the codec
	avcodec_close(pOutputCodecCtx);
	avcodec_close(pInputCodecCtx);

	av_free(pOutputCodecCtx);
	
	//close input video
	av_close_input_file(pFormatCtx);

	free(pOutBuffer);

	fclose(pOutputFile);		//output video
}

int VideoIO::main(int argc, char *argv[])
{
	if(validateArg(argc, argv) == false)
		exit(EXIT_FAILURE);;	//quit program

	//open input codec & video
	char *inputFilename = argv[1];
	bool openInputCodecReturn = openInputCodec(inputFilename);
	if(openInputCodecReturn == false)
		exit(EXIT_FAILURE);

	//open output codec & video
	char *outputFilename = argv[2];
	openOutputCodec(outputFilename, pInputCodecCtx->width, pInputCodecCtx->height);

	pUnusedInputFrameStack = new FrameStack(FRAME_LIMIT);
	for(int i = 0 ; i < FRAME_LIMIT ; i++)
	{
		//assign appropriate parts of buffer to image planes in pFrameRGB
		//note that pFrameRGB is an AVFrame, bug AVFrame is a superset of AVPicture
		//type : RGB24
		Frame *pInputFrame = new Frame(pInputCodecCtx, PIX_FMT_RGB24);
		pUnusedInputFrameStack->push(pInputFrame);
	}
	pInputFrameQueue = new FrameQueue(FRAME_LIMIT);
	
	pUnusedOutputFrameStack = new FrameStack(FRAME_LIMIT);
	
	pOutputFrameHeap = new FrameHeap(FRAME_LIMIT);

	//read frame

	for(int i = 0 ; i < FRAME_LIMIT ; i++)
		readFrame();

	for(int i = 0 ; i < FRAME_LIMIT ; i++)
	{
		Frame *frame;
		try
		{
			frame = pInputFrameQueue->front();
			pInputFrameQueue->pop();
			pOutputFrameHeap->push(frame);
		}
		catch(const char *msg)
		{
			fprintf(stderr, "[Except]%s", msg);
		}
	}

	for(int i = 0 ; i < FRAME_LIMIT ; i++)
	{
		writeFrame();
	}
	
	return 0;
}

bool VideoIO::writeFrame(void)
{
	int w = pOutputCodecCtx->width;
	int h = pOutputCodecCtx->height;
	
	//get frame
	Frame *frame;
	try
	{
		frame = pOutputFrameHeap->top();
		pOutputFrameHeap->pop();
	}
	catch(const char *msg)
	{
		fprintf(stderr, "[Except]%s\n", msg);
		return false;
	}

	//convert RGB24->YUV402P
	RGB24ToYUV420P(frame->getFrame(), w, h);

	//write frame
	outSize = avcodec_encode_video(pOutputCodecCtx, pOutBuffer, outBufferSize, pOutputFrame);
	printf("id#%d, size : %d\n", frame->getId(), outSize);
	fwrite(pOutBuffer, 1, outSize, pOutputFile);

	//saveFrame(frame, w, h, frame->getId());
	
	//push frame to unused output frame stack
	pUnusedOutputFrameStack->push(frame);

	return true;
}

int VideoIO::getFrameIndex(void)
{
	return frameIndex;
}

void VideoIO::incFrameIndex(void)
{
	frameIndex++;
}

bool VideoIO::validateArg(int argc, char *argv[])
{
	//validating commandline argument
	if(argc != 3)
	{
		printf("Usage : %s <input video> <output video>\n", argv[0]);
		return false;
	}
	//else
	return true;	
}


//return value
//true : success
//false : fail
bool VideoIO::openInputCodec(char *_filename)
{
	//open video file
	if(av_open_input_file(&pFormatCtx, _filename, NULL, 0, NULL) != 0)
	{
		fprintf(stderr, "couldn't open file\n");
		return false;	//couldn't open file
	}

	//retrieve stream information
	if(av_find_stream_info(pFormatCtx) < 0)
	{
		fprintf(stderr, "couldn't find stream information\n");
		return false;
	}

	//dump information about file onto standard error
	dump_format(pFormatCtx, 0, _filename, 0);

	//find the first video stream
	videoStream = -1;
	for(unsigned int i = 0 ; i < pFormatCtx->nb_streams ; i++)
	{
		if(pFormatCtx->streams[i]->codec->codec_type == CODEC_TYPE_VIDEO)
		{
			videoStream = i;
			break;
		}
	}
	if(videoStream == -1)
	{
		fprintf(stderr, "didn't find a video stream");
		return false;
	}

	//get a pointer to the reading codec context for the video stream
	pInputCodecCtx = pFormatCtx->streams[videoStream]->codec;

	//find the decoder for the video stream(reaing video)
	pInputCodec = avcodec_find_decoder(pInputCodecCtx->codec_id);
	if(pInputCodec == NULL)
	{
		fprintf(stderr, "Unsupported coded!\n");
		return false;	//codec not found
	}

	//open codec
	if(avcodec_open(pInputCodecCtx, pInputCodec) < 0)
	{
		fprintf(stderr, "could not open codec\n");
		return false;
	}

	//success
	return true;
}

//read a frame
//return value
//true : reading a frame success
//false : reading a frame failed
bool VideoIO::readFrame(void)
{
	if(continueReadFrame == false)
		return false;	//do not read frame1

	//else...continue read frame from video
	bool readSuccess = false;

	while(readSuccess != true && readComplete == false)
	{
		if(av_read_frame(pFormatCtx, &packet) >= 0)
		{
			//is this a packet from the video stream?
			if(packet.stream_index == videoStream)
			{
				//decode video frame
				avcodec_decode_video(pInputCodecCtx, pInputFrame, &frameFinished,
									 packet.data, packet.size);
				
				//did we get a frame?
				if(frameFinished)
				{
					int w = pInputCodecCtx->width;
					int h = pInputCodecCtx->height;
					
					if(pInputImgConvertCtx == NULL)
					{
						//prepare to convert from YUV402P to RGB24
						int pix_fmt = pInputCodecCtx->pix_fmt;
						int dst_pix_fmt = PIX_FMT_RGB24;
						
						pInputImgConvertCtx = sws_getContext(w, h, pix_fmt, 
															 w, h, dst_pix_fmt, SWS_BICUBIC,
															 NULL, NULL, NULL);
						if(pInputImgConvertCtx == NULL)
						{
							fprintf(stderr, "Can't use Sws Convert!!\n");
							exit(EXIT_FAILURE);
						}
					}
					
					//get unused frame
					Frame *frame = NULL;
					try
					{
						frame = pUnusedInputFrameStack->top();
						pUnusedInputFrameStack->pop();
					}
					catch(const char *msg)
					{
						fprintf(stderr, "[Except]%s\n", msg);
					}

					//convert YUV420P->RGB24
					sws_scale(pInputImgConvertCtx, pInputFrame->data, pInputFrame->linesize, 0, h,
							  frame->getFrame()->data, frame->getFrame()->linesize);
					
					frame->setId(frameIndex);
					
					fprintf(stderr, "read frame#%d\n", frame->getId());
					
					//complete read a frame, then push to queue
					pInputFrameQueue->push(frame);
					
					incFrameIndex();
					
					readSuccess = true;
				}
				else
				{
					readSuccess = true;
					readComplete = true;
				}
			}
			//free the packet that was allocated by av_read_frame
			av_free_packet(&packet);
		}
	}
	return true;
}

bool VideoIO::openOutputCodec(char *_filename, int _width, int _height)
{
	pOutputFile = fopen(_filename, "wb");
	if(_filename == NULL)
	{
		fprintf(stderr, "could not open %s\n", _filename);
		exit(EXIT_FAILURE);
	}

	pOutputCodec = avcodec_find_encoder(CODEC_ID_MPEG2VIDEO);

	if(pOutputCodec == NULL)
	{
		fprintf(stderr, "could not found writing codec\n");
		exit(EXIT_FAILURE);
	}

	pOutputCodecCtx = avcodec_alloc_context();

	//configure variable
	//put sample parameters
	pOutputCodecCtx->bit_rate = 400000;
	//resulution must be a multiple of two
	pOutputCodecCtx->width = _width;
	pOutputCodecCtx->height = _height;
	//frame per second
	pOutputCodecCtx->time_base = (AVRational){1,25};
	pOutputCodecCtx->gop_size = 10;	//emit one intra frame every ten frames
//	pOutputCodecCtx->time_base = pInputCodecCtx->time_base;
//	pOutputCodecCtx->gop_size = pInputCodecCtx->gop_size;
	pOutputCodecCtx->max_b_frames = 1;
	pOutputCodecCtx->pix_fmt = PIX_FMT_YUV420P;

	//open it
	if(avcodec_open(pOutputCodecCtx, pOutputCodec) < 0)
	{
		fprintf(stderr, "could not open codec\n");
		exit(EXIT_FAILURE);
	}

	//prepare out buffer
	outSize = 0;
	outBufferSize = (_width) * (_height);
	pOutBuffer = (uint8_t *) malloc (sizeof(uint8_t) * outBufferSize);


	return true;
}


bool VideoIO::saveFrame(Frame *_pFrame)
{
	char filename[32];

	int id = _pFrame->getId();
	
	//open file
	sprintf(filename, "frame%02d.ppm", id);

	return _pFrame->saveP6PPM(filename);
}

void VideoIO::RGB24ToYUV420P(AVFrame *_src, int _width, int _height)
{
	//convert RGB24(_src) to YUV420(pOutputFrame...class member variable)
	int w = _width;
	int h = _height;
	int size = w * h;

	if(pOutputFrame == NULL)
	{
		pOutputFrame = avcodec_alloc_frame();
		
		uint8_t *buf = (uint8_t *) malloc (sizeof(uint8_t) * (size * 3) / 2);

		pOutputFrame->data[0] = buf;
		pOutputFrame->data[1] = pOutputFrame->data[0] + size;
		pOutputFrame->data[2] = pOutputFrame->data[1] + size / 4;

		pOutputFrame->linesize[0] = w;
		pOutputFrame->linesize[1] = w / 2;
		pOutputFrame->linesize[2] = w / 2;
	}
	
	//Y
	for(int y = 0 ; y < h ; y++)
	{
		for(int x = 0 ; x < w; x++)
		{
			uint8_t r = *(_src->data[0] + y * _src->linesize[0] + (3*x));
			uint8_t g = *(_src->data[0] + y * _src->linesize[0] + (3*x + 1));
			uint8_t b = *(_src->data[0] + y * _src->linesize[0] + (3*x + 2));
			
			uint8_t resultY  = (0.257 * r) + (0.504 * g) + (0.098 * b) + 16;
			
			pOutputFrame->data[0][y * pOutputFrame->linesize[0] + x] = resultY;
		}
	}
	
	//Cb, Cr
	for(int y = 0 ; y < h / 2 ; y++)
	{
		for(int x = 0 ; x < w / 2 ; x++)
		{
			uint8_t r = *(_src->data[0] + y*2*_src->linesize[0] + (3*2*x));
			uint8_t g = *(_src->data[0] + y*2*_src->linesize[0] + (3*2*x + 1));
			uint8_t b = *(_src->data[0] + y*2*_src->linesize[0] + (3*2*x + 2));
			
			uint8_t resultCr = (0.439 * r) - (0.368 * g) - (0.071 * b) + 128;
			uint8_t resultCb = -(0.148 * r) - (0.291 * g) + (0.439 * b) + 128;
			
			pOutputFrame->data[1][y * pOutputFrame->linesize[1] + x] = resultCr;
			pOutputFrame->data[2][y * pOutputFrame->linesize[2] + x] = resultCb;
		}
	}
}
