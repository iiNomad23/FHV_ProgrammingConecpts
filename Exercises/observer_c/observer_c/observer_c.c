/*
    observer_c

    Created on: 24.09.2023
*/

#include <stdio.h>

#define FUNCTION_STORAGE_MAX 15

typedef void (*listenerFunction)(const char* msg);

void observerSubscribe(listenerFunction func, int arrayIndex, int* arraySize);
void publishMessage(const char* msg, int arraySize);

void subscriber_1(const char* msg);
void subscriber_2(const char* msg);
void subscriber_3(const char* msg);

listenerFunction subscriber[FUNCTION_STORAGE_MAX];

int main(int argc, char* argv[]) {

    int arraySize = 0;

    observerSubscribe(subscriber_1, 0, &arraySize);
    observerSubscribe(subscriber_2, 1, &arraySize);
    observerSubscribe(subscriber_3, 2, &arraySize);

    publishMessage("Good Morning!", arraySize);

    return 0;
}

void subscriber_1(const char* msg) {
    printf("subscriber_1:\n%s\n\n", msg);
}

void subscriber_2(const char* msg) {
    printf("subscriber_2:\n%s\n\n", msg);
}

void subscriber_3(const char* msg) {
    printf("subscriber_3:\n%s\n\n", msg);
}

void observerSubscribe(listenerFunction func, int arrayIndex, int* arraySize) {
    if (arrayIndex < 0) 
    {
        printf("Index has to be positive - arrayIndex: %d", arrayIndex);
        return;
    }
    if (arrayIndex >= FUNCTION_STORAGE_MAX)
    {
        printf("Index above max storage capacity - arrayIndex: %d", arrayIndex);
        return;
    }

    if (subscriber[arrayIndex] == NULL)
    {
        subscriber[arrayIndex] = func;
        *arraySize = *arraySize + 1;
    }
    else {
        printf("Array field of Index '%d' already taken", arrayIndex);
    }
}

void publishMessage(const char* msg, int arraySize) {
    for (int i = 0; i < arraySize; i++)
    {
        if (subscriber[i] != NULL)
        {
            subscriber[i](msg);
        }
    }
}