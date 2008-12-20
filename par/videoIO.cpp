#include <cstdlib>
#include <stdio.h>
#include "videoIO.h"
#include "frame.h"
#include "frameHeap.h"
#include "context.h"
#include "queue.h"
#include "stack.h"

VideoIO::VideoIO()
{
	writingRequested = 0;
	frameIndex = 0;
	inputFilename = NULL;
	outputFilename = NULL;

	//must be called before using avcodec lib
	avcodec_init();

	//register all formats and codec
	av_register_all();

	//allocate video frame...use like buffer
	//type : YUV420P
	pInputFrame = avcodec_alloc_frame();
	pOutputFrame = NULL;

	continueReadFrame = true;

	pContext = Context::instance();	//get Context class

	pUnusedInputFrameStack = pContext->getUnusedInputFrameStack();
	pInputFrameQueue = pContext->getInputFrameQueue();
	pUnusedOutputFrameStack = pContext->getUnusedOutputFrameStack();
	pOutputFrameHeap = pContext->getOutputFrameHeap();


	pInputCodecCtx = NULL;
	pInputCodec = NULL;
	pOutputCodecCtx = NULL;
	pOutputCodec = NULL;
	pFormatCtx = NULL;

	//set variable
	outSize = -1;
	pOutBuffer = NULL;
	outBufferSize = -1;
	readComplete = false;

	//set pointer of this
	pContext->setVideoIO(this);
}

void VideoIO::requestToWrite(int count)
{
	writingRequested = count;
}

VideoIO::~VideoIO()
{
	//close output video
	if(outSize != -1)
	{
		//get the delayed frames
		for(int i = getFrameIndex() ; outSize ; i++)
		{
			fflush(stdout);
			outSize = avcodec_encode_video(pOutputCodecCtx, pOutBuffer, outBufferSize, NULL);
			printf("[VideoIO] write frame : %3d (size=%5d)\n", i, outSize);
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
	if(pInputFrame != NULL)
		av_free(pInputFrame);

	if(pOutputFrame != NULL)
	{
		free(pOutputFrame->data[0]);
		av_free(pOutputFrame);
	}

	//close the codec
	if(pOutputCodecCtx != NULL)
	{
		avcodec_close(pOutputCodecCtx);
		av_free(pOutputCodecCtx);
		fclose(pOutputFile);		//output video
	}

	if(pInputCodecCtx != NULL)
	{
		avcodec_close(pInputCodecCtx);
	}
	
	//close input video
	if(pFormatCtx != NULL)
	{
		av_close_input_file(pFormatCtx);
	}
	
	if(pOutBuffer != NULL)
		free(pOutBuffer);
}

bool VideoIO::init(int argc, char *argv[])
{
	int frameLimit = pContext->getFrameLimit();
	
	if(validateArg(argc, argv) == false)
		exit(EXIT_FAILURE);	//quit program

	//open input codec & video
	inputFilename = argv[1];
	bool openInputCodecReturn = openInputCodec();
	if(openInputCodecReturn == false)
		exit(EXIT_FAILURE);

	//set video's height, width
	int width = pInputCodecCtx->width;
	int height = pInputCodecCtx->height;
	pContext->setHeight(height);
	pContext->setWidth(width);

	//set output file name;
	outputFilename = argv[2];
	openOutputCodec(width, height);

	//openOutputCodec(outputFilename, pInputCodecCtx->width, pInputCodecCtx->height);
/*
	for(int i = 0 ; i < frameLimit ; i++)
	{
		//assign appropriate parts of buffer to image planes in pFrameRGB
		//note that pFrameRGB is an AVFrame, bug AVFrame is a superset of AVPicture
		//type : RGB24
		Frame *pInputFrame = new Frame(pInputCodecCtx, PIX_FMT_RGB24);
		pUnusedInputFrameStack->push(pInputFrame);
	}
*/
	fprintf(stderr, "[video IO] codec initialize \n");

	return true;
}

int VideoIO::main(void)
{
	///TODO
	int range = pContext->getConvertingRange();
	int i = 0;
	while(i < range)
	{
		if(writingRequested != 0)
		{
			//write frame
			for(int i = 0 ; i < writingRequested ; i++)
			{
				writeFrame();
			}
			writingRequested = 0;
		}
		else
		{
			//read frame
			readFrame();
			i++;
		}
	}

	return 0;
}

bool VideoIO::writeFrame(void)
{
	/*
	if(pOutputCodecCtx == NULL)
	{
		//open output codec
		Frame *frame = pOutputFrameHeap->top();
		int width = frame->getWidth();
		int height = frame->getHeight();
		openOutputCodec(width, height);
	}
	*/
	int w = pOutputCodecCtx->width;
	int h = pOutputCodecCtx->height;
	
	//get frame
	Frame *frame = NULL;
	while(frame == NULL)
	{
		if(pOutputFrameHeap->isEmpty() == false)
		{
			///TODO
			frame = pOutputFrameHeap->pop();
			break;	//get output frame : success
		}
	}
	
	//convert RGB24->YUV402P
	RGB24ToYUV420P(frame->getFrame(), w, h);

	//write frame
	outSize = avcodec_encode_video(pOutputCodecCtx, pOutBuffer, outBufferSize, pOutputFrame);
	printf("[VideoIO] write Frame : id#%d, size : %d\n", frame->getId(), outSize);
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
bool VideoIO::openInputCodec(void)
{
	//open video file
	///TODO : 20byte lost
	if(av_open_input_file(&pFormatCtx, inputFilename, NULL, 0, NULL) != 0)
	{
		fprintf(stderr, "couldn't open file : %s\n", inputFilename);
		return false;	//couldn't open file
	}

	//retrieve stream information
	if(av_find_stream_info(pFormatCtx) < 0)
	{
		fprintf(stderr, "couldn't find stream information\n");
		return false;
	}

	//dump information about file onto standard error
	dump_format(pFormatCtx, 0, inputFilename, 0);

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
					
					Frame *frame = NULL;
					if(pUnusedInputFrameStack->isEmpty() == true)	///TODO
					{
						frame = new Frame(pInputCodecCtx, PIX_FMT_RGB24);
					}
					else
					{
						frame = pUnusedInputFrameStack->pop();
					}

					//convert YUV420P->RGB24
					YUV420PToRGB24(frame->getFrame(), w, h);
					
					frame->setId(frameIndex);
					
					fprintf(stderr, "[VideoIO] read frame : #%d\n", frame->getId());
					
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

bool VideoIO::openOutputCodec(int _width, int _height)
{
	pOutputFile = fopen(outputFilename, "wb");
	if(pOutputFile == NULL)
	{
		fprintf(stderr, "could not open %s\n", outputFilename);
		exit(EXIT_FAILURE);
	}

	pOutputCodec = avcodec_find_encoder(CODEC_ID_MPEG1VIDEO);

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


void VideoIO::YUV420PToRGB24(AVFrame *_dst, int _width, int _height)
{
	//convert YUV420P(pInputFrame..class member variable) to RGB24(_dst)
	int w = _width;
	int h = _height;
	int size = w * h;

	for(int y = 0 ; y < h ; y++)
	{
		for(int x = 0 ; x < w ; x++)
		{
			//get Cr, Cb
			uint8_t yuvCr = pInputFrame->data[1][y/2 * pInputFrame->linesize[1] + x/2];
			uint8_t yuvCb = pInputFrame->data[2][y/2 * pInputFrame->linesize[2] + x/2];
			
			//get Y
			uint8_t yuvY = pInputFrame->data[0][y * pInputFrame->linesize[0] + x];
			
			//calculate rgb
			uint8_t r = yuvY + (1.140*yuvCb);
			uint8_t g = yuvY - (0.395*yuvCr) - (0.581*yuvCb);
			uint8_t b = yuvY + (2.032*yuvCr);

			//assign rgb
			*(_dst->data[0] + y*_dst->linesize[0] + (3*x)) = r;
			*(_dst->data[0] + y*_dst->linesize[0] + (3*x + 1)) = g;
			*(_dst->data[0] + y*_dst->linesize[0] + (3*x + 2)) = b;
		}
	}
}


bool VideoIO::isReadingComplete(void)
{
	return readComplete;
}
