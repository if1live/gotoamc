#include <stdio.h>
#include <spu_intrinsics.h>

/* typedef struct _CONTROL_BLOCK
{
	int width;					// width of frame
	int height;					// height of frame
	uint64_t AVFrameAddress;	// address of AVFrame
	unsigned int sizeOfAVF;		// size of AVFrame
	uint64_t inputFrameAddress;	// address of inputFrame
	unsigned int sizeOfiF;		// size of inputFrame
	uint64_t arrayAddressR;		// address of array for saving rvalues
	unsigned int sizeOfarray;	// size of array
	uint64_t arrayAddressG;		// address of array for saving gvalues
	uint64_t arrayAddressB;		// address of array for saving bvalues
} control_block; */

int main( unsigned long long speid, unsigned long long argp, unsigned long long envp )
{
	int x, y, i=-1;
	control_block cb;
	AVFrame dst;
	AVFrame input;
	vector unsigned int* rVal;
	vector unsigned int* gVal;
	vector unsigned int* bVal;
	vector unsigned int* yVal;
	vector unsigned int* CbVal;
	vector unsigned int* CrVal;
	vector unsigned float CbMul2r = { 1.140, 1.140, 1.140, 1.140 };
	vector unsigned float CbMul2g = { 0.581, 0.581, 0.581, 0.581 };
	vector unsigned float CrMul2g = { 0.395, 0.395, 0.395, 0.395 };
	vector unsigned float CrMul2b = { 2.032, 2.032, 2.032, 2.032 };
	vector unsigned int CbVal2r;
	vector unsigned int CbVal2g;
	vector unsigned int CrVal2g;
	vector unsigned int CrVal2b;
	uint8_t r;
	uint8_t g;
	uint8_t b;
	
	mfc_get( &cb, argp, sizeof( cb ), 31, 0, 0 );
	mfc_write_tag_mask( 1<<31 );
	mfc_read_tag_status_all();
	
	mfc_get( &dst, cb.AVFrameAddress, cb.sizeOfAVF, 31, 0, 0 );
	mfc_write_tag_mask( 1<<31 );
	mfc_read_tag_status_all();

	mfc_get( &input, cb.inputFrameAddress, cb.sizeOfiF, 31, 0, 0 );
	mfc_write_tag_mask( 1<<31 );
	mfc_read_tag_status_all();

	rVal = (vector unsigned int*) malloc( sizeof( vector unsigned int ) * cb.width * cb.height / 4 );
	bVal = (vector unsigned int*) malloc( sizeof( vector unsigned int ) * cb.width * cb.height / 4 ); 
	gVal = (vector unsigned int*) malloc( sizeof( vector unsigned int ) * cb.width * cb.height / 4 );
	yVal = (vector unsigned int*) malloc( sizeof( vector unsigned int ) * cb.width * cb.height / 4 );
	CbVal = (vector unsigned int*) malloc( sizeof( vector unsigned int ) * cb.width * cb.height / 4 ); 
	CrVal = (vector unsigned int*) malloc( sizeof( vector unsigned int ) * cb.width * cb.height / 4 );

	for ( y = 0 ; y < cb.height ; y++ )
	{
		for ( x = 0 ; x < cb.width ; x++ )
		{
			if ( ( x + ( y * cb.height ) ) % 4 == 0 )
				i++;
			
			CrVal[i] = spu_promote( input.data[1][y/2 * input.linesize[1] + x/2], ( x + ( y * cb.height ) ) % 4 );
			CbVal[i] = spu_promote( input.data[2][y/2 * input.linesize[2] + x/2], ( x + ( y * cb.height ) ) % 4 );
			yVal[i] = spu_promote( input.data[0][y * input.linesize[0] + x],  ( x + ( y * cb.height ) ) % 4 );
		}
	}
	
	for ( i = 0 ; i < ( cb.height + cb.width ) / 4 ; i++ )
	{
		CbVal2r = spu_mul( CbVal[i], CbMul2r );
		CbVal2g = spu_mul( CbVal[i], CbMul2g );
		CrVal2g = spu_mul( CrVal[i], CrMul2g );
		CrVal2b = spu_mul( CrVal[i], CrMul2b );
		rVal[i] = yVal[i] + CbVal2r;
		gVal[i] = yVal[i] - CrVal2g - CbVal2g;
		bVal[i] = yVal[i] + CrVal2b;
	}

	cb.sizeOfarray = sizeof( uint8_t ) * cb.height * cb.width;
	spu_write_out_mbox( cb.sizeOfarray );
	spu_write_out_mbox( cb.sizeOfarray );	//for blocking

	i = -1;

	for ( y = 0 ; y < cb.height ; y++ )
	{
		for (x = 0 ; x < cb.width ; x++ )
		{
			if ( ( x + ( y * cb.height ) ) % 4 == 0 )
				i++;

			r = spu_extract( rVal[i], ( x + ( y * cb.height ) ) % 4 );
			g = spu_extract( gVal[i], ( x + ( y * cb.height ) ) % 4 );
			b = spu_extract( bVal[i], ( x + ( y * cb.height ) ) % 4 );
			mfc_put( &r, cb.arrayAddressR + sizeof( uint8_t ) *  ( x + ( y * cb.height ) ), sizeof( uint8_t ), 31, 0, 0 );
			mfc_put( &g, cb.arrayAddressG + sizeof( uint8_t ) *  ( x + ( y * cb.height ) ), sizeof( uint8_t ), 31, 0, 0 );
			mfc_put( &b, cb.arrayAddressB + sizeof( uint8_t ) *  ( x + ( y * cb.height ) ), sizeof( uint8_t ), 31, 0, 0 );
			mfc_write_tag_mask( 1<<31 );
			mfc_read_tag_status_all();
		}
	}

	free( rVal );
	free( bVal );
	free( gVal );
	free( yVal );
	free( CbVal );
	free( CrVal );

	return 0;
}
