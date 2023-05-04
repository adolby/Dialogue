#ifndef DIALOGUE_THREAD_HPP_
#define DIALOGUE_THREAD_HPP_

#include <QThread>
#include <QObject>

class Thread : public QThread {
  Q_OBJECT
  using QThread::run; // This is a final class

 public:
  explicit Thread(QObject* parent = nullptr);
  ~Thread();
};

#endif // DIALOGUE_THREAD_HPP_
