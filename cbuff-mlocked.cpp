// Circular Buffer. 
// Producer enqueues twice the input.

// Fixes after review from Chat-GPT

#include "cbuff-mlocked.h"
#include <thread>
#include <mutex>

using namespace std;
CQueue* CQueue::_cbInstance = nullptr;

void
CQueue::printQueue() {
   cout << "-----------PRINT QUEUE-------- " << endl ;
   cout << "head " << _head << endl;
   cout << "tail " << _tail << endl;
   if (_size == 0) {
      cout << "Queue is empty" << endl;
      cout << "-------------------------------------------" << endl;
      return;
   }
   if (_head <= _tail) {
      for (int i=_head; i<=_tail;i++) {
        cout << _arr[i] << " ";
      }
   } else {
     for (int i=_head; i<capacity;i++) {
        cout << _arr[i] << " ";
     }
     for (int i=0; i<_tail;i++) {
        cout << _arr[i] << " ";
     }
   }
   cout << endl;
   cout << "-------------------------------------------" << endl;
}

bool
CQueue::isFull() {
   if (_size == capacity) {
      return true;
   }
   return false;
}

bool
CQueue::isEmpty() {
   if (_size == 0) {
      return true;
   }
   return false;
}

int
CQueue::dequeue() {
   int val = _arr[_head];
   _head = (_head+1)%capacity;

   _size -= 1;
   return val;
}

bool
CQueue::enqueue(int num) {
   _tail = (_head+_size)%capacity;
   _arr[_tail] = num;
   _size += 1;
   cout << "Enqueued val: " << _arr[_tail] << endl;
   return true;
}

void Producer() {
   CQueue *_cBuff = CQueue::GetInstance();
   int input;
   while(true) {
      unique_lock plck(_cBuff->_cvlck);
      _cBuff->_cvp.wait(plck, [&_cBuff](){
                      cout << "Inside producer lambda isfull " << _cBuff->isFull() << endl;
                      return !_cBuff->isFull(); });
      cin >> input;
      _cBuff->enqueue(2*input);
      _cBuff->printQueue();
      _cBuff->_cvc.notify_one();
   }
}

void Consumer() {
   CQueue *_cBuff = CQueue::GetInstance();
   int val;
   while(true) {
      unique_lock plck(_cBuff->_cvlck);
      _cBuff->_cvc.wait(plck, [&_cBuff](){
                      cout << "Inside consumer lambda isempty " << _cBuff->isEmpty() << endl;
                      return !_cBuff->isEmpty(); });
      while (!_cBuff->isEmpty()) {
         val = _cBuff->dequeue();
         cout << "Dequeud val: " << val << endl;
      }
      _cBuff->printQueue();
      _cBuff->_cvp.notify_one();
   }
}

CQueue*
CQueue::GetInstance() {
   if (_cbInstance != nullptr) {
      return _cbInstance;
   }
   int capacity;
   cout << "ENTER BUFFER CAPCITY: ";
   cin >> capacity;
   _cbInstance = new CQueue(capacity);
   cout << "Used capacity: " << capacity << endl;
   return _cbInstance;
}

int main() {
   CQueue::GetInstance();

   thread t1(Producer);
   thread t2(Consumer);

   t1.join();
   t2.join();
   return 0;
}