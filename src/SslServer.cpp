#include "SslServer.hpp"
#include <QSslSocket>

SslServer::SslServer(QObject* parent)
  : QTcpServer{parent} {
}

SslServer::~SslServer() {
}

void SslServer::incomingConnection(qintptr socketDescriptor) {
  auto serverSocket = new QSslSocket{};

  const auto socketDescriptorSet =
    serverSocket->setSocketDescriptor(socketDescriptor);

  if (socketDescriptorSet) {
    addPendingConnection(serverSocket);
    connect(serverSocket, &QSslSocket::encrypted, this, &SslServer::ready);
    serverSocket->startServerEncryption();
  }
  else {
    delete serverSocket;
  }
}

void SslServer::ready() {

}
