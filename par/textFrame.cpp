#include "textFrame.h"
#include <iostream>
#include <cassert>
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

char* TextFrame::getText(void)
{
	return this->text;
}

void TextFrame::setText(const char* _text)
{
	assert(_text != NULL && "text is null!");
	static int prevSize;
	int size = strlen(_text) + 1;

	if(text == NULL)	//first run
	{
		text = new char[size];
	}
	else if(size != prevSize)	//text size different
	{
		delete text;
		text = new char[size];
	}
	strncpy(text, _text, size);
	prevSize = size;
}

void TextFrame::clearText(void)
{
	if(text != NULL)	// NOT NULL
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

