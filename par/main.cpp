#include <stdio.h>
#include "videoIO.h"
#include "textFrame2PPM.h"
#include "frame2TextFrame.h"
#include "context.h"

int main(int argc, char *argv[])
{
	Context *context = Context::instance();	//create global data

	VideoIO *videoIO = new VideoIO();
	Frame2TextFrame *frame2TextFrame = new Frame2TextFrame();
	TextFrame2PPM *textFrame2PPM = new TextFrame2PPM();

	videoIO->init(argc, argv);

//	while(videoIO->isReadingComplete() == false)
	for(int i = 0 ; i < 100 ; i++)
	{
		try
		{
			videoIO->readFrame();
			frame2TextFrame->convertFrame();
			textFrame2PPM->convert();
			videoIO->writeFrame();
		}
		catch(const char *msg)
		{
			fprintf(stderr, "Except : %s\n", msg);
			delete context;
			return 1;
		}
	}
	delete context;	//delete context data

	return 0;
}
