#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_

#include <QObject>
#include <QThread>
#include <QString>

#if defined(Q_OS_ANDROID)

#else
#include "utility/pimpl.h"
#endif

class Connection : public QObject {
  Q_OBJECT

 public:
  explicit Connection(QObject* parent = Q_NULLPTR);
  virtual ~Connection();

 signals:
  void updateStatus(const QString& status);
  void incomingMessage(const QString& msg);
  void outgoingMessage(const QString& msg, const QString& recipient);
  void displayError(const QString& errorMsg);

 public slots:
  void sendMessage(const QString& msg);
  void ip(const QString& ip);
  void port(const int port);

 private slots:
  void toggleConnectionMode();
  void connectToPeer();
  void socketConnected();
  void socketError();
  void socketDisconnected();
  void socketReadyRead();
  void socketBytesWritten(const qint64 numberOfBytes);
  void serverNewConnection();

 private:
  void connectSocket();
  void sendStatus();

  class ConnectionPrivate;
#if defined (Q_OS_ANDROID)
  ConnectionPrivate* m;
#else
  pimpl<ConnectionPrivate> m;
#endif
};

#endif // CONNECTION_HPP_
