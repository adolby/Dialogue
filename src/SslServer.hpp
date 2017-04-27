#ifndef DIALOGUE_SSLSERVER_HPP_
#define DIALOGUE_SSLSERVER_HPP_

#include <QTcpServer>
#include <QSslSocket>
#include <QSslCertificate>
#include <QSslKey>
#include <QSsl>
#include <QObject>
#include <QByteArray>

namespace Dialogue {

class SslServerPrivate;

class SslServer : public QTcpServer {
  Q_OBJECT
  Q_DECLARE_PRIVATE(SslServer)
  QScopedPointer<SslServerPrivate> const d_ptr;

 public:
  explicit SslServer(QObject* parent = Q_NULLPTR);
  ~SslServer();

  QSslSocket* nextPendingConnection();

  void setLocalCertificate(const QSslCertificate& certificate);
  bool setLocalCertificate(const QString& fileName,
                           QSsl::EncodingFormat format);

  void setPrivateKey(const QSslKey& key);
  bool setPrivateKey(const QString& fileName,
                     QSsl::KeyAlgorithm algorithm,
                     QSsl::EncodingFormat format,
                     const QByteArray& passPhrase);

  void setProtocol(QSsl::SslProtocol protocol);

 protected:
  void incomingConnection(qintptr socketDescriptor) override final;
};

}

#endif // DIALOGUE_SSLSERVER_HPP_
