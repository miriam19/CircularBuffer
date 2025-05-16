#include <iostream>
#include <mutex>
#include <condition_variable>

using namespace std;


class CQueue {
public:
   int capacity;
   static CQueue *_cbInstance;
   mutex _cvlck;
   condition_variable _cvc;
   condition_variable _cvp;
private:
   int *_arr;
   int _head; int _tail; int _size;
   mutex _mlck;

   CQueue(int cap)
      : capacity(cap){
      _head = 0; _tail = 0;
      _size = 0;
      _arr = new int[capacity];
   }

public:
   void printQueue();
   bool isEmpty();
   bool isFull();
   bool enqueue(int num);
   int dequeue();
   static CQueue* GetInstance();
};