#ifndef DIALOGUE_DIALOGUE_HPP_
#define DIALOGUE_DIALOGUE_HPP_

#include <QObject>
#include <QThread>

class Dialogue : public QObject {
  Q_OBJECT

 public:
  explicit Dialogue(QObject* parent = Q_NULLPTR);
  virtual ~Dialogue();

 private:
  class DialoguePrivate;
  DialoguePrivate* m;
};

#endif // DIALOGUE_DIALOGUE_HPP_
