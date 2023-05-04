#include "Thread.hpp"

Thread::Thread(QObject* parent)
  : QThread(parent) {
}

Thread::~Thread() {
  quit();
  requestInterruption();
  wait();
}
