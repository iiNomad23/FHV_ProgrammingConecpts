#include "stack.h";
#include <iostream>

Stack::Stack(int size) {
	_sp = 0;
	_size = size;
	_pValues = new int[size];
}

Stack::~Stack() {
	delete[] _pValues;
}

Stack& Stack::operator=(const Stack& s) {
	if (this != &s) // self assignment
	{
		_sp = s._sp;
		_size = s._size;

		delete[] _pValues;
		_pValues = new int[_size];

		for (int i = 0; i < _size; i++)
		{
			_pValues[i] = s._pValues[i];
		}
	}

	return (*this);
}

void Stack::push(int value) {
	if (isFull())
	{
		std::cout << "Stack is full" << std::endl;
		return;
	}

	_pValues[_sp] = value;
	_sp++;
}

int Stack::pop() {
	if (isEmpty())
	{
		std::cout << "Stack is empty" << std::endl;
		return 0;
	}

	_sp--;
	return _pValues[_sp];
}

bool Stack::isEmpty() {
	return _sp <= 0;
}

bool Stack::isFull() {
	return _sp > _size;
}