#include <iostream>

using namespace std;


class CQueue {
public:
   int capacity;
   static CQueue *_cbInstance;
private:
   int *_arr;
   int _head; int _tail; int _size;

   CQueue(int cap)
      : capacity(cap){
      _head = 0; _tail = 0;
      _size = 0;
      _arr = new int[capacity];
   }

public:
   void printQueue();
   bool enqueue(int num);
   int dequeue();
   static CQueue* GetInstance();
};