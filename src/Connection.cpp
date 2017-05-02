#include "Connection.hpp"
#include "SslServer.hpp"
#include <QSslSocket>
#include <QDataStream>
#include <QTimer>
#include <QStringRef>
#include <QStringBuilder>

#include <QDebug>

const quint16 kConnectionModeTimeout = 5000;
const quint16 kReconnectTimeout = 500;
const quint16 kDefaultPortNumber = 28710;

enum ConnectionMode {
  ClientMode = 0,
  ServerMode = 1
};

class Dialogue::ConnectionPrivate {
  Q_DISABLE_COPY(ConnectionPrivate)

 public:
  explicit ConnectionPrivate(Connection* parent);

  void destroySocket();

  int connectionAttempts;
  QTimer toggleModeTimer;
  QTimer reconnectTimer;

  //QSslSocket* socket;
  //SslServer server;
  QTcpSocket* socket;
  QTcpServer server;

  QDataStream stream;

  QString ip;
  quint16 port;
  ConnectionMode mode;
};

Dialogue::ConnectionPrivate::ConnectionPrivate(Connection* parent)
  : socket{new QSslSocket{parent}}, server{parent},
    ip{QStringLiteral("0.0.0.0")}, port{kDefaultPortNumber}, mode{ServerMode},
    connectionAttempts{0}, toggleModeTimer{parent}, reconnectTimer{parent} {
}

void Dialogue::ConnectionPrivate::destroySocket() {
  qDebug() << "ConnectionPrivate::destroySocket";

  if (socket) {
    socket->abort();
    socket->setParent(Q_NULLPTR);
  }

  socket->deleteLater();
  socket = Q_NULLPTR;
}

Dialogue::Connection::Connection(QObject* parent)
  : QObject{parent}, d_ptr{new ConnectionPrivate{this}} {
  Q_D(Connection);

  //d->server->setLocalCertificate(QStringLiteral(":/keys/server.crt"));
  //d->server->setPrivateKey(QStringLiteral(":/keys/private.key"));
  //d->server->setProtocol(QSsl::TlsV1_2);

  connect(d->socket, &QTcpSocket::connected,
          this, &Connection::socketConnected);
  connect(d->socket, &QTcpSocket::disconnected,
          this, &Connection::socketDisconnected);
  connect(d->socket,
          QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
          this,
          &Connection::socketError);
  connect(d->socket, &QTcpSocket::readyRead,
          this, &Connection::readSocket);

  // SSL connections
//  connect(d->socket,
//          QOverload<const QList<QSslError>&>::of(&QSslSocket::sslErrors),
//          this,
//          &Connection::sslErrors);
//  connect(d->socket,
//          &QSslSocket::encrypted,
//          this,
//          &Connection::socketEncrypted);

  connect(&d->server, &QTcpServer::newConnection,
          this, &Connection::startServerConnection);

  connect(&d->toggleModeTimer, &QTimer::timeout,
          this, &Connection::toggleConnectionMode);

  connect(&d->reconnectTimer, &QTimer::timeout,
          this, &Connection::connectToPeer);
}

Dialogue::Connection::~Connection() {
}

void Dialogue::Connection::sendMessage(const QString& message) {
  Q_D(Connection);

  qDebug() << "Connection::sendMessage";

  if ( d->socket ) {
    if (!message.isEmpty() &&
        d->socket->state() == QAbstractSocket::ConnectedState) {
      d->stream << message.toUtf8();

      emit outgoingMessage(message, tr("Contact"));
    }
  }
}

void Dialogue::Connection::ip(const QString& ip) {
  Q_D(Connection);

  if (ip != d->ip && !ip.isEmpty()) {
    d->ip = ip;
    toggleConnectionMode();
  }
}

void Dialogue::Connection::port(const int port) {
  Q_D(Connection);

  if (port != d->port && port > 0) {
    d->port = static_cast<quint16>(port);
    toggleConnectionMode();
  }
}

void Dialogue::Connection::toggleConnectionMode() {
  Q_D(Connection);
  qDebug() << "Connection::toggleConnectionMode";

  if (!d->toggleModeTimer.isActive()) {
    d->toggleModeTimer.start(kConnectionModeTimeout);
  }

  if ( d->socket ) {
    if (QAbstractSocket::UnconnectedState == d->socket->state()) {
      if (d->connectionAttempts > 10) {
        d->toggleModeTimer.stop();
        d->toggleModeTimer.start(kConnectionModeTimeout + 2500);
      }
      else if (d->connectionAttempts > 20) {
        d->toggleModeTimer.stop();
        d->toggleModeTimer.start(kConnectionModeTimeout);
        d->connectionAttempts = 0;
      }

      if (ConnectionMode::ServerMode == d->mode) {
        d->mode = ConnectionMode::ClientMode;

        d->server.close();

        d->destroySocket();
        d->socket = new QSslSocket{this};
        connectSocket();

        d->reconnectTimer.start(kReconnectTimeout);
      }
      else if (ConnectionMode::ClientMode == d->mode) {
        d->mode = ConnectionMode::ServerMode;

        // Stop client mode reconnect timer
        d->reconnectTimer.stop();

        if (d->port > 0) {
          d->socket->abort();

          const bool listeningCheck = d->server.listen(QHostAddress::Any,
                                                       d->port);

          emit sendStatus();
        }
      }
      else {
        emit displayError(tr("Can't connect: No such connection mode."));
      }

      d->connectionAttempts = d->connectionAttempts + 1;
    }
  }
}

void Dialogue::Connection::connectToPeer() {
  Q_D(Connection);
  qDebug() << "Connection::connectToPeer";

  if ( d->socket ) {
    if (!d->ip.isEmpty() && d->port > 0 &&
        QAbstractSocket::UnconnectedState == d->socket->state()) {
      //d->socket->addCaCertificates(QStringLiteral(":/keys/server.crt"));

      d->socket->connectToHost(d->ip, d->port);

      //d->socket->startClientEncryption();

      emit sendStatus();
    }
  }
}

void Dialogue::Connection::socketConnected() {
  Q_D(Connection);
  qDebug() << "Connection::socketConnected";

  d->connectionAttempts = 0;
  d->toggleModeTimer.stop();
  d->reconnectTimer.stop();
  emit sendStatus();
}

void Dialogue::Connection::socketError(QAbstractSocket::SocketError error) {
  Q_UNUSED(error);
  qDebug() << "Connection::socketError";

  emit displayError(tr("Couldn't connect. Reconnecting..."));
  emit sendStatus();
}

void Dialogue::Connection::sslErrors(const QList<QSslError>& errors) {
  qDebug() << errors;
}

void Dialogue::Connection::socketEncrypted() {
  qDebug() << "Connection::socketEncrypted";
  qDebug() << "ENCRYPTION IT WORKED!!";
}

void Dialogue::Connection::socketDisconnected() {
  Q_D(Connection);
  qDebug() << "Connection::socketDisconnected";

  d->toggleModeTimer.start(kConnectionModeTimeout);

  emit sendStatus();
}

void Dialogue::Connection::readSocket() {
  Q_D(Connection);
  qDebug() << "Connection::readSocket";

  if (d->socket) {
    d->stream.startTransaction();

    QByteArray message;
    d->stream >> message;

    if (d->stream.commitTransaction()) {
      qDebug() << "Message from socket: " << message;
      emit incomingMessage(message);
    }
  }
}

void Dialogue::Connection::startServerConnection() {
  Q_D(Connection);

  qDebug() << "Connection::serverNewConnection";

  d->destroySocket();

  // Grab the server socket
  d->socket = d->server.nextPendingConnection();

  // Connect new connected socket signals up to this class's slots
  connectSocket();

  d->server.close();

  d->toggleModeTimer.stop();

  emit sendStatus();
}

void Dialogue::Connection::connectSocket() {
  Q_D(Connection);

  if (d->socket) {
    connect(d->socket, &QTcpSocket::connected,
            this, &Connection::socketConnected);
    connect(d->socket, &QTcpSocket::disconnected,
            this, &Connection::socketDisconnected);
    connect(d->socket,
            QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this,
            &Connection::socketError);
    connect(d->socket, &QTcpSocket::readyRead,
            this, &Connection::readSocket);

    // SSL connections
    connect(static_cast<QSslSocket*>(d->socket),
            QOverload<const QList<QSslError>&>::of(&QSslSocket::sslErrors),
            this,
            &Connection::sslErrors);
    connect(static_cast<QSslSocket*>(d->socket),
            &QSslSocket::encrypted,
            this,
            &Connection::socketEncrypted);

    d->stream.setDevice(d->socket);
  }
}

void Dialogue::Connection::sendStatus() {
  Q_D(Connection);

  const auto reconnectTime =
    static_cast<int>(d->toggleModeTimer.remainingTime() / 1000);

  const QString connectionState = [d, reconnectTime]() {
    QString state = tr("Not connected. Trying again in ") %
                    QString{"%1 "}.arg(reconnectTime) % tr("seconds...");

    if (d->socket) {
      if (QAbstractSocket::HostLookupState == d->socket->state()) {
        state = tr("Looking up host...");
      }
      else if (QAbstractSocket::ConnectingState == d->socket->state()) {
        state = tr("Connecting to peer as client...");
      }
      else if (d->server.isListening()) {
        state = tr("Connecting to peer as server...");
      }
      else if (QAbstractSocket::ConnectedState == d->socket->state()) {
        state = tr("Connected");
      }
      else if (QAbstractSocket::ClosingState == d->socket->state()) {
        state = tr("Closing connection...");
      }
    }

    return state;
  }();

  emit updateStatus(connectionState);
}
