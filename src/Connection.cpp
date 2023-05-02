#include "Connection.hpp"
#include <QTcpServer>
#include <QTimer>
#include <QStringBuilder>

const int kConnectionModeTimeout = 5000;
const int kReconnectTimeout = 500;
const int kDefaultPortNumber = 28710;

enum ConnectionMode {
  ClientMode = 0,
  ServerMode = 1
};

class ConnectionPrivate {
  Q_DISABLE_COPY(ConnectionPrivate)

 public:
  explicit ConnectionPrivate(Connection* parent);

  void destroySocket();

  QTcpSocket* socket;
  QTcpServer* server;

  QString ip;
  int port;
  ConnectionMode mode;

  int connectionAttempts;
  QTimer toggleModeTimer;
  QTimer reconnectTimer;
};

ConnectionPrivate::ConnectionPrivate(Connection* parent)
  : socket{new QTcpSocket{parent}}, server{new QTcpServer{parent}},
    ip{QStringLiteral("0.0.0.0")}, port{kDefaultPortNumber}, mode{ServerMode},
    connectionAttempts{0}, toggleModeTimer{parent}, reconnectTimer{parent} {
}

void ConnectionPrivate::destroySocket() {
  if (socket) {
    socket->abort();
    socket->setParent(Q_NULLPTR);
  }

  delete socket;
  socket = Q_NULLPTR;
}

Connection::Connection(QObject* parent)
  : QObject{parent}, d_ptr{new ConnectionPrivate{this}} {
  Q_D(Connection);

  connect(d->socket, &QTcpSocket::connected,
          this, &Connection::socketConnected);
  connect(d->socket, &QTcpSocket::disconnected,
          this, &Connection::socketDisconnected);
  connect(d->socket,
          &QTcpSocket::errorOccurred,
          this,
          &Connection::socketError);
  connect(d->socket, &QTcpSocket::readyRead,
          this, &Connection::socketReadyRead);
  connect(d->socket, &QTcpSocket::bytesWritten,
          this, &Connection::socketBytesWritten);

  connect(d->server, &QTcpServer::newConnection,
          this, &Connection::serverNewConnection);

  connect(this,
          &Connection::startToggleModeTimer,
          &d->toggleModeTimer,
          QOverload<int>::of(&QTimer::start));
  connect(this, &Connection::stopToggleModeTimer,
          &d->toggleModeTimer, &QTimer::stop);
  connect(&d->toggleModeTimer, &QTimer::timeout,
          this, &Connection::toggleConnectionMode);

  connect(this,
          &Connection::startReconnectTimer,
          &d->reconnectTimer,
          QOverload<int>::of(&QTimer::start));
  connect(this, &Connection::stopReconnectTimer,
          &d->reconnectTimer, &QTimer::stop);
  connect(&d->reconnectTimer, &QTimer::timeout,
          this, &Connection::connectToPeer);
}

Connection::~Connection() {
}

void Connection::sendMessage(const QString& msg) {
  Q_D(Connection);

  if (!msg.isEmpty() && d->socket->state() == QAbstractSocket::ConnectedState) {
    const qint64 errorCheck = d->socket->write(msg.toUtf8());
    d->socket->flush();

    if (errorCheck != -1) {
      emit outgoingMessage(msg, tr("Contact"));
    }
  }
}

void Connection::ip(const QString& ip) {
  Q_D(Connection);

  if (ip != d->ip && !ip.isEmpty()) {
    d->ip = ip;
    d->socket->abort();
    toggleConnectionMode();
  }
}

void Connection::port(const int port) {
  Q_D(Connection);

  if (port != d->port && port > 0) {
    d->port = port;
    d->socket->abort();
    toggleConnectionMode();
  }
}

void Connection::toggleConnectionMode() {
  Q_D(Connection);

  if (!d->toggleModeTimer.isActive()) {
    emit startToggleModeTimer(kConnectionModeTimeout);
  }

  if (QAbstractSocket::UnconnectedState == d->socket->state()) {
    if (d->connectionAttempts > 10) {
      emit stopToggleModeTimer();
      emit startToggleModeTimer(kConnectionModeTimeout + 2500);
    }
    else if (d->connectionAttempts > 20) {
      emit stopToggleModeTimer();
      emit startToggleModeTimer(kConnectionModeTimeout);
      d->connectionAttempts = 0;
    }

    if (ConnectionMode::ServerMode == d->mode) {
      d->mode = ConnectionMode::ClientMode;

      d->server->close();

      d->destroySocket();
      d->socket = new QTcpSocket{this};
      connectSocket();

      emit startReconnectTimer(kReconnectTimeout);
    }
    else if (ConnectionMode::ClientMode == d->mode) {
      d->mode = ConnectionMode::ServerMode;

      // Stop client mode reconnect timer
      emit stopReconnectTimer();

      if (d->port > 0) {
        d->socket->abort();

        const bool listeningCheck = d->server->listen(QHostAddress::Any,
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

void Connection::connectToPeer() {
  Q_D(Connection);

  if (!d->ip.isEmpty() && d->port > 0 &&
      QAbstractSocket::UnconnectedState == d->socket->state()) {
    d->socket->connectToHost(d->ip, d->port);
    emit sendStatus();
  }
}

void Connection::socketConnected() {
  Q_D(Connection);

  d->connectionAttempts = 0;
  emit sendStatus();
}

void Connection::socketError(QAbstractSocket::SocketError err) {
  Q_UNUSED(err);

  emit displayError(tr("Couldn't connect. Reconnecting..."));
  emit sendStatus();
}

void Connection::socketDisconnected() {
  emit sendStatus();
}

void Connection::socketReadyRead() {
  Q_D(Connection);

  const QByteArray msg = d->socket->readAll();
  emit incomingMessage(QString(msg));
}

void Connection::socketBytesWritten(const qint64 numberOfBytes) {
  Q_UNUSED(numberOfBytes);
}

void Connection::serverNewConnection() {
  Q_D(Connection);

  d->destroySocket();

  // Grab the server socket
  d->socket = d->server->nextPendingConnection();

  // Connect new connected socket signals up to this class's slots
  connectSocket();

  d->server->close();

  emit sendStatus();
}

void Connection::connectSocket() {
  Q_D(Connection);

  connect(d->socket, &QTcpSocket::connected,
          this, &Connection::socketConnected);
  connect(d->socket, &QTcpSocket::disconnected,
          this, &Connection::socketDisconnected);
  connect(d->socket, &QTcpSocket::readyRead,
          this, &Connection::socketReadyRead);
  connect(d->socket, &QTcpSocket::bytesWritten,
          this, &Connection::socketBytesWritten);
}

void Connection::sendStatus() {
  Q_D(Connection);

  const auto reconnectTime =
    static_cast<int>(d->toggleModeTimer.remainingTime() / 1000);

  QString state = tr("Not connected. Trying again in ") %
                  QString{"%1 "}.arg(reconnectTime) %
                  tr("seconds...");

  if (QAbstractSocket::HostLookupState == d->socket->state()) {
    state = tr("Looking up host...");
  }
  else if (QAbstractSocket::ConnectingState == d->socket->state()) {
    state = tr("Connecting to peer as client...");
  }
  else if (d->server->isListening()) {
    state = tr("Connecting to peer as server...");
  }
  else if (QAbstractSocket::ConnectedState == d->socket->state()) {
    state = tr("Connected");
  }
  else if (QAbstractSocket::ClosingState == d->socket->state()) {
    state = tr("Closing connection...");
  }

  emit updateStatus(state);
}
