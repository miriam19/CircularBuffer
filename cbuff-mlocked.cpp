#include "cbuff-mlocked.h"
#include <thread>
#include <mutex>

using namespace std;
CQueue* CQueue::_cbInstance = nullptr;

void
CQueue::printQueue() {
   cout << "PRINT QUEUE " << endl ;
}

bool
CQueue::isFull() {
   lock_guard<mutex> lock(_mlck);
   if (_size == capacity) {
      return true;
   }
   return false;
}

bool
CQueue::isEmpty() {
   lock_guard<mutex> lock(_mlck);
   if (_size == 0) {
      return true;
   }
   return false;
}

int
CQueue::dequeue() {
   lock_guard<mutex> lock(_mlck);
   int val = _arr[_head];
   _head = (_head+1)%capacity;
   _size -= 1;
   return val;
}

bool
CQueue::enqueue(int num) {
   lock_guard<mutex> lock(_mlck);
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
      cin >> input;
      //bool neednotify = false;
      if (!_cBuff->isFull()) {
         _cBuff->enqueue(2*input);
         lock_guard cvlck(_cBuff->_cvlck);
         _cBuff->_cv.notify_one();
         //std::this_thread::sleep_for(std::chrono::milliseconds(200));
      } else {
         std::this_thread::sleep_for(std::chrono::milliseconds(200));
      }
   }
}

void Consumer() {
   CQueue *_cBuff = CQueue::GetInstance();
   int val;
   int dcount = 0;
   while (true) {
      //lock_guard cvlck(_cBuff->_cvlck);
      unique_lock cvlck(_cBuff->_cvlck);
      bool isEmpty = _cBuff->isEmpty();
      _cBuff->_cv.wait(cvlck, [isEmpty]{return !isEmpty; });
      if (!_cBuff->isEmpty()) {
         val = _cBuff->dequeue();
         cout << "dequeued value: " << val << endl;
         dcount++;
         //std::this_thread::sleep_for(std::chrono::milliseconds(100));
      } else {
         std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
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