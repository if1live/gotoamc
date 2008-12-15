#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include "frame.h"

using namespace std;

Frame::Frame()
{
	//do not use real program
	height = -1;
	width = -1;
	id = 0;
	pFrame = NULL;
	buffer = NULL;
}

Frame::Frame(int _id)
{
	//use to test
	//do not use real program
	width = -1;
	height = -1;
	id = _id;
	pFrame = NULL;
	buffer = NULL;
}

Frame::Frame(AVCodecContext *_pCodecCtx, int _type)
{
	try
	{
		id = 0;
		pFrame = NULL;

		//allocate an AVFrame structure
		pFrame = avcodec_alloc_frame();
		if(pFrame == NULL)
			throw "cannot allocate frame";

		width = _pCodecCtx->width;
		height = _pCodecCtx->height;
	   
		//determine required buffer size and allocate buffer
		int numBytes = avpicture_get_size(_type, width, height);
		buffer = (uint8_t *) av_malloc (sizeof(uint8_t) * numBytes);

		//assign appropriate parts of buffer to image planes in pFrame
		//not that pFrame is an AVFrame, but AVFrame is a superset of AVPicture
		avpicture_fill((AVPicture *)pFrame, buffer, _type, width, height);
	}
	catch(char *msg)
	{
		cout << msg;
	}
}

Frame::Frame(int _width, int _height, int _id)
{
	id = _id;
	setBlankFrame(_width, _height);
}

Frame::~Frame()
{
	if(buffer != NULL)
		free(buffer);

	//free the frame
	if(pFrame != NULL)
		av_free(pFrame);
}

void Frame::setBlankFrame(int _width, int _height)
{
	//if already pFrame exist, delete it
	if(buffer != NULL)
		free(buffer);

	//free the frame
	if(pFrame != NULL)
		av_free(pFrame);

	try
	{
		pFrame = NULL;

		//allocate an AVFrame structure
		pFrame = avcodec_alloc_frame();
		if(pFrame == NULL)
			throw "cannot allocate frame";

		//create P6 PPM blank Image
		int type = PIX_FMT_RGB24;

		width = _width;
		height = _height;

		//determine required buffer size and allocate buffer
		int numBytes = avpicture_get_size(type, _width, _height);
		buffer = (uint8_t *) av_malloc (sizeof(uint8_t) * numBytes);

		//assign appropriate parts of buffer to image planes in pFrame
		//not that pFrame is an AVFrame, but AVFrame is a superset of AVPicture
		avpicture_fill((AVPicture *)pFrame, buffer, type, _width, _height);
	}
	catch(char *msg)
	{
		cout << msg;
	}
}

int Frame::getId(void)
{
	return id;
}

void Frame::setId(int _id)
{
	id = _id;
}

bool Frame::operator>(const Frame &_frame)
{
	if(this->id > _frame.id)
		return true;
	else
		return false;
}

bool Frame::operator>=(const Frame &_frame)
{
	if(this->id >= _frame.id)
		return true;
	else
		return false;
}

bool Frame::operator<(const Frame &_frame)
{
	return !operator>(_frame);
}

bool Frame::operator<=(const Frame &_frame)
{
	return !operator<=(_frame);
}


bool Frame::operator==(const Frame &_frame)
{
	if(this->id == _frame.id)
		return true;
	else
		return false;
}

bool Frame::operator!=(const Frame &_frame)
{
	return !operator==(_frame);
}

AVFrame *Frame::getFrame(void)
{
	return pFrame;
}

int Frame::getWidth(void)
{
	return width;
}

int Frame::getHeight(void)
{
	return height;
}

int Frame::getRGB(int _x, int _y)
{
	assert(_x >= 0 && _x < width && _y >= 0 && _y < height && "not available coord");

	//return value
	//int is maybe 4byte
	//red, green, blue are 1 byte
	//4byte = 1byte(trash) + 1byte(red) + 1byte(green) + 1byte(blue)
	unsigned char r = *(pFrame->data[0] + _y*pFrame->linesize[0] + (_x*3));
	unsigned char g = *(pFrame->data[0] + _y*pFrame->linesize[0] + (_x*3 + 1));
	unsigned char b = *(pFrame->data[0] + _y*pFrame->linesize[0] + (_x*3 + 2));
	
	int intR = 0xff & r;
	int intG = 0xff & g;
	int intB = 0xff & b;
	
	intR = intR << 16;
	intG = intG << 8;
	
	int rgb = intR | intG | intB;
	return rgb;
}

bool Frame::setGrey(int _x, int _y, unsigned char _value)
{
	assert(_x >= 0 && _x < width && _y >= 0 && _y < height && "not available coord");

	*(pFrame->data[0] + _y*pFrame->linesize[0] + (_x*3)) = _value;
	*(pFrame->data[0] + _y*pFrame->linesize[0] + (_x*3 + 1)) = _value;
	*(pFrame->data[0] + _y*pFrame->linesize[0] + (_x*3 + 2)) = _value;
	
	return true;
}


bool Frame::setBlack(int _x, int _y)
{
	assert(_x >= 0 && _x < width && _y >= 0 && _y < height && "not available coord");

	*(pFrame->data[0] + _y*pFrame->linesize[0] + (_x*3)) = (unsigned char) 0;
	*(pFrame->data[0] + _y*pFrame->linesize[0] + (_x*3 + 1)) = (unsigned char) 0;
	*(pFrame->data[0] + _y*pFrame->linesize[0] + (_x*3 + 2)) = (unsigned char) 0;
	
	return true;
}


bool Frame::setWhite(int _x, int _y)
{
	assert(_x >= 0 && _x < width && _y >= 0 && _y < height && "not available coord");

	*(pFrame->data[0] + _y*pFrame->linesize[0] + (_x*3)) = (unsigned char) 255;
	*(pFrame->data[0] + _y*pFrame->linesize[0] + (_x*3 + 1)) = (unsigned char) 255;
	*(pFrame->data[0] + _y*pFrame->linesize[0] + (_x*3 + 2)) = (unsigned char) 255;
	
	return true;
}



bool Frame::openP3PPM(char *_filename)
{
	//if already allocate frame, return false(error code)
	if(pFrame != NULL)
		return false;

	//if cannot open _filename, return false(error code)
	//open only ascii ppm type P6
	FILE *pFile = fopen(_filename, "r");
	if(pFile == NULL)
	{
		fprintf(stderr, "cannot open file : %s\n", _filename);
		return false;
	}
	else
	{
		printf("open image : %s\n", _filename);
	}

	//get header
	//head must be "P3 19 33 255" style
	fscanf(pFile, "P3 %d %d 255", &width, &height);
	
	//allocate frame
	pFrame = avcodec_alloc_frame();
	if(pFrame == NULL)
		throw "Cannot allocate frame";
	int numBytes = avpicture_get_size(PIX_FMT_RGB24, width, height);
	buffer = (uint8_t *) av_malloc (sizeof(uint8_t) * numBytes);

	avpicture_fill((AVPicture *)pFrame, buffer, PIX_FMT_RGB24, width, height);

	for(int y = 0 ; y < height ; y++)
	{
		for(int x = 0 ; x < width ; x++)
		{
			//get r, g, b
			int intR, intG, intB;
			fscanf(pFile, "%d", &intR);
			fscanf(pFile, "%d", &intG);
			fscanf(pFile, "%d", &intB);

   			unsigned char r = (unsigned char) intR;
			unsigned char g = (unsigned char) intG;
			unsigned char b = (unsigned char) intB;

			//assign pixel data
			*(pFrame->data[0] + y*pFrame->linesize[0] + x*3) = r;
			*(pFrame->data[0] + y*pFrame->linesize[0] + x*3+1) = g;
			*(pFrame->data[0] + y*pFrame->linesize[0] + x*3+2) = b;
		}
	}
	
	//close file
	fclose(pFile);

	return true;
}


//write frame to P6 type ppm
bool Frame::saveP6PPM(char *_filename)
{
	//if frame not defined, return false(error code)
	if(width == -1 || height == -1)
		return false;

	//open file
	FILE *pFile = fopen(_filename, "wb");
	if(pFile == NULL)
	{
		fprintf(stderr, "cannot write ppm file\n");
		return false;
	}
	else
	{
		printf("write image : %s\n", _filename);
	}
	
	//write header
	fprintf(pFile, "P6\n%d %d\n255\n", width, height);

	//write pixel data
	for(int y = 0 ; y < height ; y++)
	{
		fwrite(pFrame->data[0] + y*pFrame->linesize[0], 1, width * 3, pFile);
	}

	//close file
	fclose(pFile);

	return true;
}

bool Frame::openPPM(char *_filename)
{
	///TODO
}

bool Frame::openP4PPM(char *_filename)
{
	///TODO
}

bool Frame::openP5PPM(char *_filename)
{
	///TODO
}


bool Frame::saveP3PPM(char *_filename)
{
	//if frame not defined, return false(error code)
	if(width == -1 || height == -1)
		return false;

	//open file
	FILE *pFile = fopen(_filename, "wb");
	if(pFile == NULL)
	{
		fprintf(stderr, "cannot write ppm file\n");
		return false;
	}
	else
	{
		printf("write image : %s\n", _filename);
	}
	
	//write header
	fprintf(pFile, "P3\n%d %d\n255\n", width, height);

	//write pixel data
	for(int y = 0 ; y < height ; y++)
	{
		for(int x = 0 ; x < width ; x++)
		{
			int rgb = getRGB(x, y);
			
			unsigned char r = (0x00ff0000 & rgb) >> 16;
			unsigned char g = (0x0000ff00 & rgb) >> 8;
			unsigned char b = (0x000000ff & rgb);
			
			fprintf(pFile, "%d %d %d\n", r, g, b);
		}
	}

	//close file
	fclose(pFile);

	return true;
}
