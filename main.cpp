#include <stdio.h>
#include "videoIO.h"
#include "textFrame2PPM.h"
#include "frame2TextFrame.h"
#include "context.h"

int main(int argc, char *argv[])
{
	Context *context = Context::instance();	//create global data

//	VideoIO *videoIO = new VideoIO();
//	videoIO->main(argc, argv);

//	VideoIO *videoIO = new VideoIO();
//	Frame2TextFrame *frame2TextFrame = new Frame2TextFrame();
	TextFrame2PPM *textFrame2PPM = new TextFrame2PPM();

	while(videoIO->isReadingComplete() == false)
	{
		videoIO->readFrame();
		frame2TextFrame->convertFrame();
		textFrame2PPM->convert();
		videoIO->writeFrame();
	}

	delete context;	//delete context data

	return 0;
}
