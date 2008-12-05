#include "frame.h"
#include "textFrame.h"

#define MAX_OF_FONTS 128
#define WIDTH_OF_FONTS 16 //19
#define HEIGHT_OF_FONTS 16 //33

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
	
private :
	void createEmptyFrame(void);

	Frame *fonts[MAX_OF_FONTS];
	
	Context *pContext;

	Queue<TextFrame *> *pTextFrameQueue;
	Stack<TextFrame *> *pUnusedTextFrameStack;

	FrameHeap *pOutputFrameHeap;
	Stack<Frame *> *pUnusedOutputFrameStack;

	
};
