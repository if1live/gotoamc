#include <stdio.h>
#include <spu_intrinsics.h>

/* typedef struct _CONTROL_BLOCK
{
	unsigned int width;		// width of frame
	unsigned int height;	// height of frame
	uint64_t AVFAddress;	// address of AVFrame
	unsigned int sizeOfAVF;	// size of AVFrame
	uint64_t arrayAddressY;	// address of array for saving yvalues
	unsigned int sizeOfop;	// size of array
	uint64_t arrayAddressCr;// address of array for saving Crvalues
	uint64_t arrayAddressCb;// address of array for saving Cbvalues
} control_block; */

int main( unsigned long long speid, unsigned long long argp, unsigned long long envp )
{
	int x, y, i=-1, j=-1;
	control_block cb;
	AVFrame src;
	AVFrame output;
	vector unsigned int* rVal;
	vector unsigned int* gVal;
	vector unsigned int* bVal;
	vector unsigned int* rVal2;
	vector unsigned int* gVal2;
	vector unsigned int* bVal2;
	vector unsigned int* yVal;
	vector unsigned int* CbVal;
	vector unsigned int* CrVal;
	vector unsigned float rMul2Y = { 0.257, 0.257, 0.257, 0.257 };
	vector unsigned float gMul2Y = { 0.504, 0.504, 0.504, 0.504 };
	vector unsigned float bMul2Y = { 0.098, 0.098, 0.098, 0.098 };
	vector unsigned float rMul2Cr = { 0.439, 0.439, 0.439, 0.439 };
	vector unsigned float gMul2Cr = { 0.368, 0.368, 0.368, 0.368 };
	vector unsigned float bMul2Cr = { 0.071, 0.071, 0.071, 0.071 };
	vector unsigned float rMul2Cb = { 0.148, 0.148, 0.148, 0.148 };
	vector unsigned float gMul2Cb = { 0.291, 0.291, 0.291, 0.291 };
	vector unsigned float bMul2Cb = { 0.439, 0.439, 0.439, 0.439 };
	vector unsigned float rVal2C;
	vector unsigned float gVal2C;
	vector unsigned float bVal2C;
	uint8_t y;
	uint8_t Cr;
	uint8_t Cb;
	

	mfc_get( &cb, argp, sizeof( cb ), 31, 0, 0 );
	mfc_write_tag_mask( 1<<31 );
	mfc_read_tag_status_all();

	mfc_get( &src, cb.AVFAddress, cb.sizeOfAVF, 31, 0, 0 );
	mfc_write_tag_mask( 1<<31 );
	mfc_read_tag_status_all();

	rVal = (vector unsigned int*) malloc( sizeof( vector unsigned int ) * cb.width * cb.height / 4 );
	bVal = (vector unsigned int*) malloc( sizeof( vector unsigned int ) * cb.width * cb.height / 4 ); 
	gVal = (vector unsigned int*) malloc( sizeof( vector unsigned int ) * cb.width * cb.height / 4 );
	rVal2 = (vector unsigned int*) malloc( sizeof( vector unsigned int ) * cb.width * cb.height / 16 );
	bVal2 = (vector unsigned int*) malloc( sizeof( vector unsigned int ) * cb.width * cb.height / 16 ); 
	gVal2 = (vector unsigned int*) malloc( sizeof( vector unsigned int ) * cb.width * cb.height / 16 );
	yVal = (vector unsigned int*) malloc( sizeof( vector unsigned int ) * cb.width * cb.height / 4 );
	CbVal = (vector unsigned int*) malloc( sizeof( vector unsigned int ) * cb.width * cb.height / 4 ); 
	CrVal = (vector unsigned int*) malloc( sizeof( vector unsigned int ) * cb.width * cb.height / 4 ); 
	
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
			
			rVal[i] = spu_promote( src.data[0] + y * src.linesize[0] + (3*x), ( x + ( y * cb.height ) ) % 4 );
			gVal[i] = spu_promote( src.data[0] + y * src.linesize[0] + (3*x + 1), ( x + ( y * cb.height ) ) % 4 ); 
			bVal[i] = spu_promote( src.data[0] + y * src.linesize[0] + (3*x + 2), ( x + ( y * cb.height ) ) % 4 );
			if ( y < (cb.height / 2) && x < (cb.width / 2) )
			{
				rVal2[j] = spu_promote( src.data[0] + y * 2 * src.linesize[0] + (3*2*x), ( x + ( y * cb.height ) ) % 4 );
				gVal2[j] = spu_promote( src.data[0] + y * 2 * src.linesize[0] + (3*2*x + 1), ( x + ( y * cb.height ) ) % 4 ); 
				bVal2[j] = spu_promote( src.data[0] + y * 2 * src.linesize[0] + (3*2*x + 2), ( x + ( y * cb.height ) ) % 4 );
			}
		}
	}

	for ( i = 0 ; i < ( cb.height + cb.width ) / 4 ; i++ )
	{
		rVal[i] = spu_mul( rVal[i], rMul2Y );
		gVal[i] = spu_mul( gVal[i], gMul2Y );
		bVal[i] = spu_mul( bVal[i], bMul2Y );
		yVal[i] = rVal[i] + gVal[i] + bVal[i];
		yVal[i] = spu_add( yVal[i], 16 );
	}
	for ( j = 0 ; j < ( cb.height + cb.width ) / 16 ; j++ )
	{	
		rVal2C = spu_mul( rVal2[j], rMul2Cb );
		gVal2C = spu_mul( gVal2[j], gMul2Cb );
		bVal2C = spu_mul( bVal2[j], bMul2Cb );
		CbVal[j] = rValC - gValC - bValC;
		CbVal[j] = spu_add( CbVal[j], 128 );
		rVal2C = spu_mul( rVal2[j], rMul2Cr );
		gVal2C = spu_mul( gVal2[j], gMul2Cr );
		bVal2C = spu_mul( bVal2[j], bMul2Cr );
		CrVal[j] = -rValC - gValC + bValC;
		CrVal[j] = spu_add( CrVal[j], 128 );
	}
	
	i = j = -1;
			
	for ( y = 0 ; y < cb.height ; y++ )
	{
		for ( x = 0 ; x < cb.width ; x++ )
		{
			if ( ( x + ( y * cb.height ) ) % 4  == 0 )
		   		i++;
			
			y = spu_extract( yVal[i], ( x + ( y * cb.height ) ) % 4 );
			mfc_put( &y, cb.arrayAddressY + sizeof( uint8_t ) * ( x + ( y * cb.height ) ), 31, 0, 0 );
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
			mfc_put( &Cr, cb.arrayAddressCr + sizeof( uint8_t ) * ( x + ( y * cb.height / 2 ) ), 31, 0, 0 );
			mfc_put( &Cb, cb.arrayAddressCb + sizeof( uint8_t ) * ( x + ( y * cb.height / 2 ) ), 31, 0, 0 );
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
