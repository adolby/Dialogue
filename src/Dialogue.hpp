#ifndef DIALOGUE_DIALOGUE_HPP_
#define DIALOGUE_DIALOGUE_HPP_

#include <QObject>
#include <memory>

class DialoguePrivate;

class Dialogue : public QObject {
  Q_OBJECT
  Q_DECLARE_PRIVATE(Dialogue)
  std::unique_ptr<DialoguePrivate> const d_ptr;

 public:
  explicit Dialogue(QObject* parent = nullptr);
  virtual ~Dialogue();

 private slots:
  void cleanup();
};

#endif // DIALOGUE_DIALOGUE_HPP_
