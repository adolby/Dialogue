#ifndef DIALOGUE_SSLSERVER_HPP_
#define DIALOGUE_SSLSERVER_HPP_

#include <QTcpServer>

class SslServer : public QTcpServer {
 public:
  SslServer(QObject* parent = Q_NULLPTR);

  void incomingConnection(qintptr socketDescriptor);

 public slots:
  void ready();
};

#endif // DIALOGUE_SSLSERVER_HPP_
