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

	VideoIO *videoIO = new VideoIO();
	Frame2TextFrame *frame2TextFrame = new Frame2TextFrame();
	TextFrame2PPM *textFrame2PPM = new TextFrame2PPM();

	videoIO->init(argc, argv);

	while(videoIO->isReadingComplete() == false)
	{
		perror("0 ");
		videoIO->readFrame();
		perror("1 ");
		frame2TextFrame->convertFrame();
		perror("2 ");
		textFrame2PPM->convert();
		perror("3 ");
		videoIO->writeFrame();
		perror("4 \n");
	}

	delete context;	//delete context data

	return 0;
}
