#include <stdlib.h>
//#include "videoIO_extra.h"
#include "videoIO_spe1.h"
#include <libspe2.h>

extern spe_program_handle_t videoIO_spe1_handle;
spe_context_ptr_t rgb2yuvCtx;
control_block_spe1 cb __attribute__((aligned(128)));

void* runSPE1(void* arg)
{
	//prepare to spe create
	//rgb->yuv
	//  control_block_spe1 cb __attribute__((aligned(128)));

	unsigned int entry_point; // start address
	int retval; // return value

	// fill in data
	uint8_t* arrayY;
	uint8_t* arrayCr;
	uint8_t* arrayCb;
	uint8_t* arrayR;
	uint8_t* arrayG;
	uint8_t* arrayB;

	//Y 
	/*  
		for(int y = 0 ; y < h ; y++)
		{
		for(int x = 0 ; x < w; x++)
		{
		uint8_t r = *(_src->data[0] + y * _src->linesize[0] + (3*x));
		uint8_t g = *(_src->data[0] + y * _src->linesize[0] + (3*x + 1));
		uint8_t b = *(_src->data[0] + y * _src->linesize[0] + (3*x + 2));

		uint8_t resultY  = (0.257 * r) + (0.504 * g) + (0.098 * b) + 16;

		pOutputFrame->data[0][y * pOutputFrame->linesize[0] + x] = resultY;
		}
		}
	//Cb, Cr
	for(int y = 0 ; y < h / 2 ; y++)
	{
	for(int x = 0 ; x < w / 2 ; x++)
	{
	uint8_t r = *(_src->data[0] + y*2*_src->linesize[0] + (3*2*x));
	uint8_t g = *(_src->data[0] + y*2*_src->linesize[0] + (3*2*x + 1));
	uint8_t b = *(_src->data[0] + y*2*_src->linesize[0] + (3*2*x + 2));

	uint8_t resultCr = (0.439 * r) - (0.368 * g) - (0.071 * b) + 128;
	uint8_t resultCb = -(0.148 * r) - (0.291 * g) + (0.439 * b) + 128;

	pOutputFrame->data[1][y * pOutputFrame->linesize[1] + x] = resultCr;
	pOutputFrame->data[2][y * pOutputFrame->linesize[2] + x] = resultCb;
	}
	}
	*/

	//prepare spe program
	int retVal;
	try {
		// create the spe context
		rgb2yuvCtx = spe_context_create(0, NULL);
		if(!rgb2yuvCtx) throw "videoIO spe1 spe_context_create error!";

		// load the program handle into the context
		retVal = spe_program_load(rgb2yuvCtx, &videoIO_spe1_handle);    //TODO s
		if(retVal) throw "videoIO spe1 spe_program_load error!";

		//TODO : other spe program
	} catch(const char *msg) {
		fprintf(stderr, "[spe Except] %s\n", msg);
	}
	// run the program inside the context
	entry_point = SPE_DEFAULT_ENTRY;
	retVal = spe_context_run(rgb2yuvCtx, &entry_point, 0, &cb, NULL, NULL);
	//    if(retval < 0)
	//{
	//	fprintf(stderr, "videoIO spe1 spe_context_run error!");
	//	exit(EXIT_FAILURE);
	//}

	// deallocate the context
	retval = spe_context_destroy(rgb2yuvCtx);
	if(retval) throw "videoIO spe1 spe_context_destroy error!";

}

