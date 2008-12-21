#pragma once

#include <stdint.h>
typedef struct _CONTROL_BLOCK_SPE1
{	
	uint32_t arrayAddressY;	// address of array for saving yvalues
	unsigned int sizeOfarray;	// size of array
	uint32_t arrayAddressCr;// address of array for saving Crvalues
	uint32_t arrayAddressCb;// address of array for saving Cbvalues

	unsigned int width;
	unsigned int height;

	uint32_t rValAddr;
	uint32_t gValAddr;
	uint32_t bValAddr;
	uint32_t rVal2Addr;
	uint32_t gVal2Addr;
	uint32_t bVal2Addr;

} control_block_spe1;

