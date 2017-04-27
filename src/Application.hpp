#ifndef DIALOGUE_APPLICATION_HPP_
#define DIALOGUE_APPLICATION_HPP_

#include <QObject>
#include <QVariant>
#include <QString>

namespace Dialogue {

class ApplicationPrivate;

class Application : public QObject {
  Q_OBJECT
  Q_DECLARE_PRIVATE(Application)
  QScopedPointer<ApplicationPrivate> const d_ptr;

 public:
  explicit Application(QObject* parent = Q_NULLPTR);
  virtual ~Application();

 signals:
  void updateStatus(const QVariant& status);

 private slots:
  void connectGUI(QObject* object, const QUrl& url);
  void cleanup();
};

}

#endif // DIALOGUE_APPLICATION_HPP_
