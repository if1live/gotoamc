#pragma once

#include <pthread.h>

#include "frame.h"
#include "textFrame.h"

#define MAX_OF_FONTS 128
#define WIDTH_OF_FONTS 8 //19
#define HEIGHT_OF_FONTS 8 //33

class Context;
class FrameHeap;
class TextFrame;

template <class T> class Queue;
template <class T> class Stack;

class TextFrame2PPM
{
public :
	TextFrame2PPM();
	void convert(void);

	int main(void);	//entry point
	~TextFrame2PPM();
	void createEmptyFrame(void);

private :

	Frame *fonts[MAX_OF_FONTS];
	
	Context *pContext;

	Queue<TextFrame *> *pTextFrameQueue;
	Stack<TextFrame *> *pUnusedTextFrameStack;

	FrameHeap *pOutputFrameHeap;
	Stack<Frame *> *pUnusedOutputFrameStack;

	int ratio;	//resize factor

	unsigned char *textBuffer;
	int outputFrameCount;	//how many image exist in outputFrameHeap?

	void incIndex(void);	//index++

	int index;	//how many frame to write?
	pthread_mutex_t indexLock;
	pthread_mutex_t lock1;
	pthread_mutex_t lock2;
};
