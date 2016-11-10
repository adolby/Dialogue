#ifndef DIALOGUE_HPP_
#define DIALOGUE_HPP_

#include "utility/pimpl.h"
#include <QObject>
#include <QThread>

class Dialogue : public QObject {
  Q_OBJECT

 public:
  explicit Dialogue(QObject* parent = Q_NULLPTR);
  virtual ~Dialogue();

 private:
  class DialoguePrivate;
#if defined(Q_OS_ANDROID)
  DialoguePrivate* m;
#else
  pimpl<DialoguePrivate> m;
#endif
};

#endif // DIALOGUE_HPP_
