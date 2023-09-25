/*
    observer_c

    Created on: 24.09.2023
*/

#include <stdio.h>

#define FUNCTION_STORAGE_MAX 10

typedef void (*listenerFunction)(const char* msg);

void observerSubscribe(listenerFunction func, int* highestArrayCnt);
void publishMessage(const char* msg, int highestArrayCnt);

void subscriber_1(const char* msg);
void subscriber_2(const char* msg);
void subscriber_3(const char* msg);

listenerFunction subscriber[FUNCTION_STORAGE_MAX];

int main(int argc, char* argv[]) {

    int highestArrayCnt = 0;

    observerSubscribe(subscriber_1, &highestArrayCnt);
    observerSubscribe(subscriber_2, &highestArrayCnt);
    observerSubscribe(subscriber_3, &highestArrayCnt);

    publishMessage("Good Morning!", highestArrayCnt);

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

void observerSubscribe(listenerFunction func, int* highestArrayCnt) {
    for (int i = 0; i < FUNCTION_STORAGE_MAX; i++)
    {
        if (subscriber[i] == NULL)
        {
            subscriber[i] = func;

            if (*highestArrayCnt <= i)
            {
                *highestArrayCnt = i + 1;
            }

            return;
        }
    }
}

void publishMessage(const char* msg, int highestArrayCnt) {
    for (int i = 0; i < highestArrayCnt; i++)
    {
        if (subscriber[i] != NULL)
        {
            subscriber[i](msg);
        }
    }
}