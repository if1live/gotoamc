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
}

Frame::Frame(int _id)
{
	//use to test
	//do not use real program
	width = -1;
	height = -1;
	id = _id;
	pFrame = NULL;
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

Frame::~Frame()
{
	free(buffer);

	//free the frame
	av_free(pFrame);
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
