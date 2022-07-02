
#include <iostream>           // std::cout
#include <thread>             // std::thread
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <queue>
#include <string>

#include "msgQueue.hpp"

msgQueue ToMQTTQueue;
msgQueue FromMQTTQueue;
void msgQueue::getMsg ( std::string &msg ) {
  std::unique_lock<std::mutex> lck(mtx);
  while (ready <=0 ) cv.wait(lck);

  msg =  myqueue.front();
  myqueue.pop();
  ready = myqueue.size();
}

void msgQueue::putMsg( std::string msg ) {
  std::unique_lock<std::mutex> lck(mtx);

  if(myqueue.size() > 10) return;

  myqueue.push(msg);

  ready = myqueue.size();
  cv.notify_all();
}

