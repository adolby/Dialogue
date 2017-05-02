#ifndef DIALOGUE_CONNECTION_HPP_
#define DIALOGUE_CONNECTION_HPP_

#include <QObject>
#include <QString>
#include <QAbstractSocket>
#include <QSslError>

namespace Dialogue {

class ConnectionPrivate;

class Connection : public QObject {
  Q_OBJECT
  Q_DECLARE_PRIVATE(Connection)
  QScopedPointer<ConnectionPrivate> const d_ptr;

 public:
  explicit Connection(QObject* parent = Q_NULLPTR);
  virtual ~Connection();

 signals:
  void updateStatus(const QString& status);
  void incomingMessage(const QString& msg);
  void outgoingMessage(const QString& msg, const QString& recipient);
  void displayError(const QString& errorMsg);

 public slots:
  void sendMessage(const QString& message);
  void ip(const QString& ip);
  void port(const int port);

 private slots:
  void toggleConnectionMode();
  void connectToPeer();
  void socketConnected();
  void socketError(QAbstractSocket::SocketError error);
  void sslErrors(const QList<QSslError>& errorList);
  void socketEncrypted();
  void socketDisconnected();
  void readSocket();
  void startServerConnection();

 private:
  void connectSocket();
  void sendStatus();
};

}

#endif // DIALOGUE_CONNECTION_HPP_
