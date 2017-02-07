#ifndef DIALOGUE_SSLSERVER_HPP_
#define DIALOGUE_SSLSERVER_HPP_

#include <QTcpServer>
#include <QObject>

class SslServer : public QTcpServer {
  Q_OBJECT

 public:
  explicit SslServer(QObject* parent = Q_NULLPTR);
  ~SslServer();

 protected:
  void incomingConnection(qintptr socketDescriptor) override final;

 public slots:
  void ready();
};

#endif // DIALOGUE_SSLSERVER_HPP_
