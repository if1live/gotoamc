#include <stdio.h>
#include "videoIO.h"
#include "textFrame2PPM.h"

int main(int argc, char *argv[])
{
/*
	VideoIO *videoIO = new VideoIO();
	videoIO->main(argc, argv);
	delete videoIO;
*/

/*	VideoIO videoIO;
	videoIO.main(argc, argv);
*/
    TextFrame2PPM test;
    test.Convert();
	
	return 0;
}
