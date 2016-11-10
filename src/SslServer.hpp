#ifndef SSLSERVER_HPP_
#define SSLSERVER_HPP_

#include <QTcpServer>

class SslServer : public QTcpServer {
 public:
  SslServer(QObject* parent = Q_NULLPTR);

  void incomingConnection(qintptr socketDescriptor);

 public slots:
  void ready();
};

#endif // SSLSERVER_HPP_
