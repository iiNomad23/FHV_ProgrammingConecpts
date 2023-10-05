#pragma once

#define MY_DEFAULT_STACK_SIZE 32

class Stack {
private:
	int _sp;
	int _size;
	int* _pValues;

public:
	Stack(int size = MY_DEFAULT_STACK_SIZE);
	~Stack();

	Stack& operator=(const Stack& s);

	void push(int value);
	int pop();
	bool isEmpty();
	bool isFull();
};