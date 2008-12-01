#include <cassert>
#include "textFrame2PPM.h"

/* TextFrame2PPM::TextFrame2PPM()
{
    for ( int i = 0 ; i < MAX_OF_FONTS ; i++ )
    {
	fonts[i] = new Frame();
	fonts[i]->setBlankFrame( WIDTH_OF_FONTS, HEIGHT_OF_FONTS );

	char* buff;
	snprintf( buff, sizeof( buff ), "%d.ppm", i );
	fonts[i]->openP3PPM( buff );
    }
}

void TextFrame2PPM::Convert( TextFrame* _inputFrame )
{
    Frame outputFrame;
    outputFrame.setBlankFrame( WIDTH_OF_FONTS * _inputFrame->getTextWidth(), HEIGHT_OF_FONTS * _inputFrame->getTextHeight() );

    for ( int i = 0 ; i < _inputFrame->getTextHeight() ; i++ )
    {
	for ( int p = 0 ; p < HEIGHT_OF_FONTS ; p++ )
	{
	    for ( int j = 0 ; j < _inputFrame->getTextWidth() ; j++ )
	    {
		for ( int q = 0 ; q < WIDTH_OF_FONTS ; q++ )
		{
		    unsigned char* buff =  _inputFrame->getText();
		    outputFrame.setGrey( q, p, 0x000000ff&fonts[(int) buff[j]]->getRGB( q, p ) );
		}
	    }
	}
    }

    outputFrame.saveP6PPM( "output.ppm" );		    
}

TextFrame2PPM::~TextFrame2PPM()
{
    for ( int i = 0 ; i < MAX_OF_FONTS ; i++ )
	delete[] fonts[i];
} */

TextFrame2PPM::TextFrame2PPM()
{
	int i;
  for (i = 97 ; i < 100 ; i++ )
    {
      fonts[i] = new Frame();
   
      char* buff = new char[32];
      snprintf( buff, sizeof( buff ), "%d.ppm", i );
      fonts[i]->openP3PPM( buff );
	  delete[] buff;
    }

  fonts[32] = new Frame();
  
  char* buff = new char[32];
  snprintf( buff, sizeof( buff ), "%d.ppm", 32 );
  fonts[32]->openP3PPM( buff );
  delete[] buff;
}

void TextFrame2PPM::Convert()
{
  int height = 3;
  int width = 5;
  char* testChar = " abc  bca  cab ";
  Frame outputFrame;
    outputFrame.setBlankFrame( WIDTH_OF_FONTS * width, HEIGHT_OF_FONTS * height );

  for ( int i = 0 ; i < height ; i++ )
  {
	  for ( int p = 0 ; p < HEIGHT_OF_FONTS ; p++ )
	  {
		  for ( int j = 0 ; j < width ; j++ )
		  {
			  //printf("%c %d", testChar[i*width + j], testChar[i*width + j] );
			  for ( int q = 0 ; q < WIDTH_OF_FONTS ; q++ )
			  {
				  int rgb = fonts[testChar[i*width + j]]->getRGB(q, p);
				  unsigned char value = 0x000000ff & rgb;
				   //outputFrame.setGrey( q, p, value );
				  if(value != 0)
					  outputFrame.setBlack(q + j*WIDTH_OF_FONTS, p + i*HEIGHT_OF_FONTS);
				  else
					  outputFrame.setWhite(q + j*WIDTH_OF_FONTS, p + i*HEIGHT_OF_FONTS);
			  }
		  }
		  printf("\n");
	  }
  }

  outputFrame.saveP3PPM( "output.ppm" );
  fprintf(stderr, "fsdfsdfss");
}

TextFrame2PPM::~TextFrame2PPM()
{
  for ( int i = 97 ; i < 100 ; i++ )
    delete fonts[i];
  delete fonts[32]; 
}


