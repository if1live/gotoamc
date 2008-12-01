#include "textFrame2PPM.h"

TextFrame2PPM::TextFrame2PPM()
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
}

