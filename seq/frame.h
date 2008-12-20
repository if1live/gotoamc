#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// include required libs
#include <avformat.h>
#include <avcodec.h>

#ifdef __cplusplus
}
#endif

class Frame
{
public:
	//ctor
	Frame();
	Frame(AVCodecContext *_pCodecCtx, int _type);
	Frame(int _id);
	Frame(int _width, int _height, int _id);

	//dtor
	~Frame();

	int getId(void);
	void setId(int _id);

	void setBlankFrame(int _width, int _height);

	//operator overloading
	bool operator>(const Frame &_frame);
	bool operator>=(const Frame &_frame);
	bool operator<(const Frame &_frame);
	bool operator<=(const Frame &_frame);
	bool operator==(const Frame &_frame);
	bool operator!=(const Frame &_frame);

	AVFrame *getFrame(void);
	int getWidth(void);
	int getHeight(void);
 
	int getRGB(int _x, int _y);
	bool setGrey(int _x, int _y, unsigned char _value);
	bool setBlack(int _x, int _y);
	bool setWhite(int _x, int _y);

	//only ppm support
	bool openPPM(char *_filename);
	bool openP3PPM(FILE* pFile);
	bool openP4PPM(FILE* pFile);
	bool openP5PPM(FILE* pFile);

	bool saveP3PPM(char *_filename);
	bool saveP6PPM(char *_filename);

private:
	AVFrame *pFrame;
	int width;
	int height;
	int id;

	uint8_t *buffer;
};
