#include "frame.h"
#include "textFrame.h"
#include "constNum.h"

class Context;
class TextFrameQueue;
class TextFrameStack;
class FrameHeap;
class FrameStack;

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

	TextFrameQueue *pTextFrameQueue;
	TextFrameStack *pUnusedTextFrameStack;

	FrameHeap *pOutputFrameHeap;
	FrameStack *pUnusedOutputFrameStack;

	
};
