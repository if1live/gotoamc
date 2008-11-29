#pragma once
#define MAX 10

class stack
{
public:
	stack(void);
	~stack(void);

	void push( int input );
	int pop();
	bool isEmpty();
	bool isFull();
	int* getTop();

private:
	int array[MAX];
	int temp;
};
