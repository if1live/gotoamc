#pragma once

class TextFrame;

class TextFrameStack
{
public:
	TextFrameStack( int _capacity = 10 );
	~TextFrameStack(void);

	void push( TextFrame* _pTextFrame );
	TextFrame* pop();
	bool isEmpty();
	bool isFull();
	TextFrame* top();

private:
	TextFrame** ppArray;
	int capacity;
	int m_top;
};
