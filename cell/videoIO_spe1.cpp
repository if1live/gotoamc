#include <stdio.h>
#include <stdlib.h>
#include <spu_intrinsics.h>
#include "videoIO_spe1.h" 
#include <spu_mfcio.h>
#include "frame.h"

/* typedef struct _CONTROL_BLOCK
{
	unsigned int width;		// width of frame
	unsigned int height;	// height of frame
	uint64_t AVFAddress;	// address of AVFrame
	unsigned int sizeOfAVF;	// size of AVFrame
	uint64_t arrayAddressY;	// address of array for saving yvalues
	unsigned int sizeOfarray;	// size of array
	uint64_t arrayAddressCr;// address of array for saving Crvalues
	uint64_t arrayAddressCb;// address of array for saving Cbvalues
} control_block; */

int main( unsigned long long speid, unsigned long long argp, unsigned long long envp )
{
	int x, y, i=-1, j=-1;
	control_block_spe1 cb;
	AVFrame src;
	AVFrame output;
	vector float* rVal;
	vector float* gVal;
	vector float* bVal;
	vector float* rVal2;
	vector float* gVal2;
	vector float* bVal2;
	vector float* yVal;
	vector float* CbVal;
	vector float* CrVal;
//	vector unsigned int* rVal;
//	vector unsigned int* gVal;
//	vector unsigned int* bVal;
//	vector unsigned int* rVal2;
//	vector unsigned int* gVal2;
//	vector unsigned int* bVal2;
//	vector unsigned int* yVal;
//	vector unsigned int* CbVal;
//	vector unsigned int* CrVal;
	vector float rMul2Y = { 0.257, 0.257, 0.257, 0.257 };
	vector float gMul2Y = { 0.504, 0.504, 0.504, 0.504 };
	vector float bMul2Y = { 0.098, 0.098, 0.098, 0.098 };
	vector float rMul2Cr = { 0.439, 0.439, 0.439, 0.439 };
	vector float gMul2Cr = { 0.368, 0.368, 0.368, 0.368 };
	vector float bMul2Cr = { 0.071, 0.071, 0.071, 0.071 };
	vector float rMul2Cb = { 0.148, 0.148, 0.148, 0.148 };
	vector float gMul2Cb = { 0.291, 0.291, 0.291, 0.291 };
	vector float bMul2Cb = { 0.439, 0.439, 0.439, 0.439 };
	vector float rVal2C;
	vector float gVal2C;
	vector float bVal2C;
	vector float yAdd = { 16, 16, 16, 16 };
	vector float CrCbAdd = { 128, 128, 128, 128 };
	uint8_t Yy;
	uint8_t Cr;
	uint8_t Cb;
	

	mfc_get( &cb, argp, sizeof( cb ), 31, 0, 0 );
	mfc_write_tag_mask( 1<<31 );
	mfc_read_tag_status_all();

	mfc_get( &src, cb.AVFAddress, cb.sizeOfAVF, 31, 0, 0 );
	mfc_write_tag_mask( 1<<31 );
	mfc_read_tag_status_all();

	rVal = (vector float*) malloc( sizeof( vector float ) * cb.width * cb.height / 4 );
	bVal = (vector float*) malloc( sizeof( vector float ) * cb.width * cb.height / 4 ); 
	gVal = (vector float*) malloc( sizeof( vector float ) * cb.width * cb.height / 4 );
	rVal2 = (vector float*) malloc( sizeof( vector float ) * cb.width * cb.height / 16 );
	bVal2 = (vector float*) malloc( sizeof( vector float ) * cb.width * cb.height / 16 ); 
	gVal2 = (vector float*) malloc( sizeof( vector float ) * cb.width * cb.height / 16 );
	yVal = (vector float*) malloc( sizeof( vector float ) * cb.width * cb.height / 4 );
	CbVal = (vector float*) malloc( sizeof( vector float ) * cb.width * cb.height / 4 ); 
	CrVal = (vector float*) malloc( sizeof( vector float ) * cb.width * cb.height / 4 ); 
	
	for ( y = 0 ; y < cb.height ; y++ )
	{
		for ( x = 0 ; x < cb.width ; x++ )
		{
			if ( ( x + ( y * cb.height ) ) % 4 == 0 )
			{			
				i++;
				if ( y < (cb.height / 2) && x < (cb.width / 2) )
					 j++;
			}
			
			rVal[i] = spu_promote( (float) *(src.data[0] + y * src.linesize[0] + (3*x)), ( x + ( y * cb.height ) ) % 4 );
			gVal[i] = (float) spu_promote( (unsigned int) src.data[0] + y * src.linesize[0] + (3*x + 1), ( x + ( y * cb.height ) ) % 4 ); 
			bVal[i] = (float) spu_promote( (unsigned int) src.data[0] + y * src.linesize[0] + (3*x + 2), ( x + ( y * cb.height ) ) % 4 );
			if ( y < (cb.height / 2) && x < (cb.width / 2) )
			{
				rVal2[j] = (float) spu_promote( (unsigned int) src.data[0] + y * 2 * src.linesize[0] + (3*2*x), ( x + ( y * cb.height ) ) % 4 );
				gVal2[j] = (float) spu_promote( (unsigned int) src.data[0] + y * 2 * src.linesize[0] + (3*2*x + 1), ( x + ( y * cb.height ) ) % 4 ); 
				bVal2[j] = (float) spu_promote( (unsigned int) src.data[0] + y * 2 * src.linesize[0] + (3*2*x + 2), ( x + ( y * cb.height ) ) % 4 );
			}
		}
	}

	for ( i = 0 ; i < ( cb.height + cb.width ) / 4 ; i++ )
	{
		rVal[i] = spu_mul( rVal[i], rMul2Y );
		gVal[i] = spu_mul( gVal[i], gMul2Y );
		bVal[i] = spu_mul( bVal[i], bMul2Y );
		yVal[i] = rVal[i] + gVal[i] + bVal[i];
		yVal[i] = spu_add( yVal[i], yAdd );
	}
	for ( j = 0 ; j < ( cb.height + cb.width ) / 16 ; j++ )
	{	
		rVal2C = spu_mul( rVal2[j], rMul2Cb );
		gVal2C = spu_mul( gVal2[j], gMul2Cb );
		bVal2C = spu_mul( bVal2[j], bMul2Cb );
		CbVal[j] = rVal2C - gVal2C - bVal2C;
		CbVal[j] = spu_add( CbVal[j], CrCbAdd );
		rVal2C = spu_mul( rVal2[j], rMul2Cr );
		gVal2C = spu_mul( gVal2[j], gMul2Cr );
		bVal2C = spu_mul( bVal2[j], bMul2Cr );
		CrVal[j] = -rVal2C - gVal2C + bVal2C;
		CrVal[j] = spu_add( CrVal[j], CrCbAdd );
	}
	
	i = j = -1;
			
	for ( y = 0 ; y < cb.height ; y++ )
	{
		for ( x = 0 ; x < cb.width ; x++ )
		{
			if ( ( x + ( y * cb.height ) ) % 4  == 0 )
		   		i++;
			
			Yy = spu_extract( yVal[i], ( x + ( y * cb.height ) ) % 4 );
			mfc_put( &Yy, cb.arrayAddressY + sizeof( uint8_t ) * ( x + ( y * cb.height ) ), sizeof( uint8_t ), 31, 0, 0 );
			mfc_write_tag_mask( 1<<31 );
			mfc_read_tag_status_all();
		}
	}

	for ( y = 0 ; y < cb.height / 2 ; y++ )
	{
		for ( x = 0 ; x < cb.width / 2 ; x++ )
		{
			if ( ( x + ( y * cb.height / 2 ) ) % 4 == 0 )
				j++;
			
			Cr = spu_extract( CrVal[j], ( x + ( y * cb.height ) ) % 4 );
			Cb = spu_extract( CbVal[j], ( x + ( y * cb.height ) ) % 4 );
			mfc_put( &Cr, cb.arrayAddressCr + sizeof( uint8_t ) * ( x + ( y * cb.height / 2 ) ), sizeof( uint8_t ), 31, 0, 0 );
			mfc_put( &Cb, cb.arrayAddressCb + sizeof( uint8_t ) * ( x + ( y * cb.height / 2 ) ), sizeof( uint8_t ), 31, 0, 0 );
			mfc_write_tag_mask( 1<<31 );
			mfc_read_tag_status_all();
		}
	}

	free( rVal );
	free( bVal );
	free( gVal );
	free( rVal2 );
	free( bVal2 );
	free( gVal2 );
	free( yVal );
	free( CbVal );
	free( CrVal );
	
	return 0;
}
