#pragma once

class TextFrame
{
public:
	TextFrame();	// ctor
	~TextFrame();	// dtor
	int getWidth(void);
	int getHeight(void);
	void setWidth(int _width);
	void setHeight(int _height);
	int getTextWidth(void);
	int getTextHeight(void);
	void setTextWidth(int _textWidth);
	void setTextHeight(int _textHeight);
	unsigned char* getText(void);	// get text!
	void setText(unsigned char* _text);	// set text.
	void clearText(void); // clear saved text
	void print(void);	// for debugging purpose

	//dtor
	~TextFrame();
private:
	unsigned char *text;	// converted data
	int width;	//width of original frame
	int height;	//height of original frame
	int textWidth;		// aa width
	int textHeight;		// aa height

};
