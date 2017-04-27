#include "SslServer.hpp"
#include <QSslSocket>
#include <QFile>

class Dialogue::SslServerPrivate {
  Q_DISABLE_COPY(SslServerPrivate)

 public:
  explicit SslServerPrivate(SslServer* parent);

  QList<QSslSocket*> connections;
  QSslCertificate certificate;
  QSslKey privateKey;
  QSsl::SslProtocol protocol;
};

Dialogue::SslServerPrivate::SslServerPrivate(SslServer* parent) {
}

Dialogue::SslServer::SslServer(QObject* parent)
  : QTcpServer{parent}, d_ptr{new SslServerPrivate{this}} {
}

Dialogue::SslServer::~SslServer() {
}

QSslSocket* Dialogue::SslServer::nextPendingConnection() {
  Q_D(SslServer);

  QSslSocket* connection =
      d->connections.isEmpty() ? Q_NULLPTR : d->connections.takeFirst();

  return connection;
}

void Dialogue::SslServer::setLocalCertificate(const QSslCertificate& certificate) {
  Q_D(SslServer);

  d->certificate = certificate;
}

bool Dialogue::SslServer::setLocalCertificate(const QString& fileName,
                                              QSsl::EncodingFormat format) {
  Q_D(SslServer);

  QFile certificateFile{fileName};

  if (!certificateFile.open(QIODevice::ReadOnly)) {
      return false;
  }

  d->certificate = QSslCertificate{certificateFile.readAll(), format};

  return true;
}

void Dialogue::SslServer::setPrivateKey(const QSslKey& key) {
  Q_D(SslServer);

  d->privateKey = key;
}

bool Dialogue::SslServer::setPrivateKey(const QString& fileName,
                                        QSsl::KeyAlgorithm algorithm,
                                        QSsl::EncodingFormat format,
                                        const QByteArray& passPhrase) {
  Q_D(SslServer);

  QFile keyFile{fileName};

  bool openedKeyFile = keyFile.open(QIODevice::ReadOnly);

  if (openedKeyFile) {
    d->privateKey = QSslKey{keyFile.readAll(), algorithm, format,
                            QSsl::PrivateKey, passPhrase};
  }

  return openedKeyFile;
}

void Dialogue::SslServer::setProtocol(QSsl::SslProtocol protocol) {
  Q_D(SslServer);

  d->protocol = protocol;
}

void Dialogue::SslServer::incomingConnection(qintptr socketDescriptor) {
  Q_D(SslServer);

  auto serverSocket = new QSslSocket{this};
  serverSocket->setLocalCertificate(d->certificate);
  serverSocket->setPrivateKey(d->privateKey);
  serverSocket->setProtocol(d->protocol);

  const auto socketDescriptorSet =
    serverSocket->setSocketDescriptor(socketDescriptor);

  if (socketDescriptorSet) {
    addPendingConnection(serverSocket);
    serverSocket->startServerEncryption();
  }
  else {
    delete serverSocket;
  }
}
