#include <stdio.h>
#include "frame.h"
/* typedef _CONTROL_BLOCK
{
	uint32_t fontAddress; 		// address of fonts
	unsigned int sizeOfFonts; 	// size of fonts
	uint32_t tbAddress;			// address of text buffer
	unsigned int sizeOfTb;		// size of text buffer
	uint32_t ofAddress;			// address of output frame
	unsigned int sizeOfOf;		// size of output frame
	int height;					// height of text frame with ratio
	int width;					// width of text frame with ratio
} control_block;

MAX_OF_FONTS, HEIGHT_OF_FONTS, WIDTH_OF_FONTS // is defined in textFrame2PPM.h
*/


int main( unsigned long long spe_id, unsigned long long argp, unsigned long long envp )
{
	Frame* fonts[MAX_OF_FONTS];
	unsigned char* textBuffer;
	control_block cb;
	Frame* outputFrame;

	mfc_get( &cb, argp, sizeof( cb ), 31, 0, 0 );
	mfc_write_tag_mask( 1<<31 );
	mfc_read_tag_status_all();

	mfc_get( fonts, cb.fontAddress, cb.sizeOfFonts, 31, 0, 0 );
	mfc_write_tag_mask( 1<<31 );
	mfc_read_tag_status_all();

	mfc_get( textBuffer, cb.tbAddress, cb.sizeOfTb, 31, 0, 0 );
	mfc_write_tag_mask( 1<<31 );
	mfc_read_tag_status_all();

	for ( int i = 0 ; i < cb.height ; i++ )
	{
		for ( int p = 0 ; p < HEIGHT_OF_FONTS ; p++ )
		{
			for ( int j = 0 ; j < cb.width ; j++ )
			{
				for ( int q = 0 ; q < WIDTH_OF_FONTS ; q++ )
				{
					int rgb = fonts[textBuffer[i*width + j]]->getRGB(q, p);
					unsigned char value = 0x000000ff & rgb;
					
					outputFrame->setGrey( q + j*WIDTH_OF_FONTS, p + i*HEIGHT_OF_FONTS, value );
				}
			}
		}
	}

	mfc_put( outputFrame, cb.ofAddress, cb.sizeOfOf, 31, 0, 0 );
	mfc_write_tag_mask( 1<<31 );
	mfc_read_tag_status_all();
	spu_write_out_mbox( 0 );
}
