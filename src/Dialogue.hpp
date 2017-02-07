#ifndef DIALOGUE_DIALOGUE_HPP_
#define DIALOGUE_DIALOGUE_HPP_

#include <QObject>

class DialoguePrivate;

class Dialogue : public QObject {
  Q_OBJECT
  Q_DECLARE_PRIVATE(Dialogue)
  QScopedPointer<DialoguePrivate> const d_ptr;

 public:
  explicit Dialogue(QObject* parent = Q_NULLPTR);
  virtual ~Dialogue();

 private slots:
  void cleanup();
};

#endif // DIALOGUE_DIALOGUE_HPP_
