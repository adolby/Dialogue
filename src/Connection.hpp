#ifndef DIALOGUE_CONNECTION_HPP_
#define DIALOGUE_CONNECTION_HPP_

#include <QObject>
#include <QTcpSocket>
#include <QString>

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
  void startToggleModeTimer(const int msecs);
  void stopToggleModeTimer();
  void startReconnectTimer(const int msecs);
  void stopReconnectTimer();

 public slots:
  void sendMessage(const QString& msg);
  void ip(const QString& ip);
  void port(const int port);

 private slots:
  void toggleConnectionMode();
  void connectToPeer();
  void socketConnected();
  void socketError(QTcpSocket::SocketError err);
  void socketDisconnected();
  void socketReadyRead();
  void socketBytesWritten(const qint64 numberOfBytes);
  void serverNewConnection();

 private:
  void connectSocket();
  void sendStatus();
};

#endif // DIALOGUE_CONNECTION_HPP_
