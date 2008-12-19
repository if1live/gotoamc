#include "textFrame.h"
#include <iostream>
#include <cstring>

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

int TextFrame::getId(void)
{
	return this->id;
}

void TextFrame::setId(int _id)
{
	this->id = _id;
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

void TextFrame::setText(unsigned char* _text)
{
	int size = strlen((const char*)_text);
	text = new unsigned char[size];
	memcpy(this->text, _text, size * sizeof(unsigned char));
}

void TextFrame::clearText(void)
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
	std::cout << text << std::endl;
	std::cout << "TextWidth: " << textWidth << "TextHeight: " << textHeight << std::endl;
}

