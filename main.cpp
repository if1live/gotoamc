#include <stdio.h>
#include "videoIO.h"

int main(int argc, char *argv[])
{
/*
	VideoIO *videoIO = new VideoIO();
	videoIO->main(argc, argv);
	delete videoIO;
*/

	VideoIO videoIO;
	videoIO.main(argc, argv);
	
	return 0;
}
