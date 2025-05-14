#include "cbuff.h"
#include <thread>

using namespace std;
CQueue* CQueue::_cbInstance = nullptr;

void
CQueue::printQueue() {
   cout << "PRINT QUEUE " << endl ;
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
   for (int i=0; i < 13; i++) {
      _cBuff->enqueue(2*(i+1));
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
   }
}

void Consumer() {
   CQueue *_cBuff = CQueue::GetInstance();
   int val;
   for (int i=0; i < 13; i++) {
      val = _cBuff->dequeue();
      cout << "dequeued value: " << val << endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
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