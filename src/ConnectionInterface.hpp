#ifndef DIALOGUE_CONNECTIONDATA_HPP_
#define DIALOGUE_CONNECTIONDATA_HPP_

#include <QObject>
#include <QString>

class ConnectionInterface : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString status READ status NOTIFY statusChanged)

 public:
  explicit ConnectionInterface(QObject* parent = nullptr);
  QString status() const;

 signals:
  void statusChanged();
  void message(const QString& msgText, const QString& recipient);

 public slots:
  void updateStatus(const QString& status);
  void incomingMessage(const QString& msgText);
  void outgoingMessage(const QString& msgText, const QString& recipient);

 private:
  QString status_;
};

#endif // DIALOGUE_CONNECTIONDATA_HPP_
