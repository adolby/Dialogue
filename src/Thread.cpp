#include "Thread.hpp"

Dialogue::Thread::Thread(QObject* parent)
  : QThread{parent} {
}

Dialogue::Thread::~Thread() {
  quit();
  requestInterruption();
  wait();
}
