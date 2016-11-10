#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_

#include "../../src/utility/pimpl.h"
#include <QObject>
#include <QString>

class Connection : public QObject {
  Q_OBJECT

 public:
  explicit Connection(QObject* parent = Q_NULLPTR);
  virtual ~Connection();

 signals:
//  void responseChatMessage(const QString& responseMsg);
  void reconnect();

 public slots:
  void chatMessage(const QString& msg);
  void ip(const QString& ip);
  void port(const int port);

 private slots:
  void swapModes();
  void connectToPeer();
  void socketConnected();
  void socketError();
  void socketDisconnected();
  void socketReadyRead();
  void socketBytesWritten(const qint64 numberOfBytes);
  void serverNewConnection();

 private:
  void connectSocket();

  class ConnectionPrivate;
  pimpl<ConnectionPrivate> m;
};

#endif // CONNECTION_HPP_
