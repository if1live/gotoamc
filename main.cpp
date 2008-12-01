#include <stdio.h>
#include "videoIO.h"
#include "textFrame2PPM.h"
#include "context.h"

int main(int argc, char *argv[])
{
	Context *context = Context::instance();	//create global data

	VideoIO videoIO;
	videoIO.main(argc, argv);

//	TextFrame2PPM test;
//	test.Convert();
	
	delete context;	//delete context data

	return 0;
}
