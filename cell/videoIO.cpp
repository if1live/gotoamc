#include <cstdlib>
#include <stdio.h>
#include "videoIO.h"
#include "frame.h"
#include "frameHeap.h"
#include "context.h"
#include "queue.h"
#include "stack.h"

#include "videoIO_spe1.h"
#include <libspe2.h>

// program handle for videoIO_spe1 and videoIO_spe2
extern spe_program_handle_t videoIO_spe1_handle;
extern spe_program_handle_t videoIO_spe2_handle;

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
	writeAllFrame = false;
}

void VideoIO::requestToWrite(int count)
{
	//if -1, write all frame from frame heap
	if(-1 == count)
	{
		writeAllFrame = true;
	}
	else
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

	//create empty frame
	Frame *frame = NULL;
	int limit = pContext->getFrameLimit();
	for(int i = 0 ; i < limit ; i++)
	{
		Frame *frame;
		frame = new Frame(pInputCodecCtx, PIX_FMT_RGB24);
		pUnusedInputFrameStack->push(frame);
	}

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
		if(writingRequested != 0 || writeAllFrame == true)
		{
			//write frame
			if(writingRequested != 0)
			{
				for(int i = 0 ; i < writingRequested ; i++)
				{
					writeFrame();
				}
				writingRequested = 0;
			}
			else
			{
				while(pOutputFrameHeap->isEmpty() == false)
					writeFrame();
			}
		}
		else
		{
			//read frame
			readFrame();
			i++;
		}
	}
	//waiting to textFram2PPM exit
//	while(writeAllFrame == false)
//	{
//		;	//waiting
//	}
	
	//write remain frame
	while(pOutputFrameHeap->isEmpty() == false)
		writeFrame();

	fprintf(stderr, "[VideoIO] Exit <<<<<<<<<<<<<<<<<<\n");
	return 0;
}

bool VideoIO::writeFrame(void)
{
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
					while(frame == NULL)
					{
						if(pUnusedInputFrameStack->isEmpty() == false)
						{
							frame = pUnusedInputFrameStack->pop();
						}
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

// videoIO_spe1.cpp
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

	// start jobs with spe
	uint64_t* arrayY, *arrayCr, *arrayCb;
	spe_context_ptr_t ctx;

	// create the spe context
	ctx = spe_context_create(0, NULL);
	if(!ctx)
		throw "videoIO: spe1 spe_context_create error!";
	
	// load the program handle into the context
	retval = spe_program_load(ctx, &video_spe1_handle);
	if(retval)
		throw "videoIO: spe1 spe_program load error!";
	
	// manage spe action
	control_block cb __attribute__((aligned(128)));
	unsigned int entry_point;	// start address
	int retval;	// return value from spe thread
			
	// control block value set
	cb.width = _width;
	cb.height = _height;
	cb.AVFAddress = _src;
	cb.sizeOfAVF = sizeof(AVFrame);
	cb.sizeOfarray = sizeof( uint8_t ) * _width * _height;
	arrayY = new uint64_t[cb.sizeOfarray];
	arrayCr = new uint64_t[cb.sizeOfarray];
	arrayCb = new uint64_t[cb.sizeOfarray];
	cb.arrayAddressY = arrayY;
	cb.arrayAddressCr = arrayCr;
	cb.arrayAddressCb = arrayCb;

	// run the program inside the context
	entry_point = SPE_DEFAULT_ENTRY;
	
	for(int i=0; i<h; i++)
	{
		retval = spe_context_run(ctx, &entry_point, 0, &cb, NULL, NULL);
		if(retval < 0)
			throw "videoIO: spe1 spe_context_run error!";
		
		// save new data to pOutputFrame
		for(j=0; j<w; j++)
		{
			pOutputFrame->data[0][i * pOutputFrame->linesize[0] + j] = arrayY;
			pOutputFrame->data[1][i * pOutputFrame->linesize[1] + j] = arrayCr;
			pOutputFrame->data[2][i * pOUtputFRame->linesize[2] + j] = arrayCb;	
		}
	}

	// free the data
	delete[] arrayY;
	delete[] arrayCr;
	delete[] arrayCb;
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
