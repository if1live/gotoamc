#pragma once

#include <stdint.h>

typedef struct _CONTROL_BLOCK_SPE1
{
	unsigned int width;		// width of frame
	unsigned int height;	// height of frame
	uint64_t AVFAddress;	// address of AVFrame
	unsigned int sizeOfAVF;	// size of AVFrame
	uint64_t arrayAddressY;	// address of array for saving yvalues
	unsigned int sizeOfarray;	// size of array
	uint64_t arrayAddressCr;// address of array for saving Crvalues
	uint64_t arrayAddressCb;// address of array for saving Cbvalues

	uint64_t rValAddr;
	uint64_t gValAddr;
	uint64_t bValAddr;
	uint64_t rVal2Addr;
	uint64_t gVal2Addr;
	uint64_t bVal2Addr;
} control_block_spe1;


