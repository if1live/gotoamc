#include "textFrame.h"
#include <iostream>
#include <string>

TextFrame::TextFrame()
{
	textWidth = -1;
	textHeight = -1;
	text = NULL;
}

TextFrame::~TextFrame()
{
	clearText();
}

int TextFrame::getWidth(void)
{
	return this->width;
}

int TextFrame::getHeight(void)
{
	return this->height;
}

void TextFrame::setWidth(int _width)
{
	this->width = _width;
} 

void TextFrame::setHeight(int _height)
{
	this->height = _height;
}

int TextFrame::getTextWidth(void)
{
	return this->textWidth;
}

int TextFrame::getTextHeight(void)
{
	return this->textHeight;
}

void TextFrame::setTextWidth(int _textWidth)
{
	this->textWidth = _textWidth;
}

void TextFrame::setTextHeight(int _textHeight)
{
	this->textHeight = _textHeight;
}

unsigned char* TextFrame::getText(void)
{
	return this->text;
}

void setText(unsigned char* _text)
{
	text = new char[strlen(_text)];
	strncpy(this->text, _text, strlen(_text));
}

void clearText(void)
{
	if(text)	// NOT NULL
	{
		delete[] text;
		text = NULL;	
	}
	textWidth = -1;
	textHeight = -1;
}

void TextFrame::print(void) // for debugging purpose
{
	std::cout << aadata << std::endl;
	std::cout << "TextWidth: " << textWidth << "TextHeight: " << textHeight << std::endl;
}

