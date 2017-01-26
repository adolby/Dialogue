#include "Connection.hpp"
#include <QTcpSocket>
#include <QTcpServer>
#include <QTimer>
#include <QStringRef>
#include <QStringBuilder>

const int kConnectionModeTimeout = 5000;
const int kReconnectTimeout = 500;
const int kDefaultPortNumber = 28710;

enum ConnectionMode {
  ClientMode = 0,
  ServerMode = 1
};

class Connection::ConnectionPrivate {
 public:
  explicit ConnectionPrivate();
  virtual ~ConnectionPrivate();

  void destroySocket();

  QTcpSocket* socket;
  QTcpServer* server;

  QTimer* toggleModeTimer;
  QTimer* reconnectTimer;
  int connectionAttempts;

  QString ip;
  int port;
  ConnectionMode mode;
};

Connection::ConnectionPrivate::ConnectionPrivate()
  : socket{Q_NULLPTR}, server{Q_NULLPTR}, toggleModeTimer{Q_NULLPTR},
    reconnectTimer{Q_NULLPTR}, connectionAttempts{0},
    ip{QStringLiteral("0.0.0.0")}, port{kDefaultPortNumber}, mode{ServerMode} {
}

Connection::ConnectionPrivate::~ConnectionPrivate() {
}

void Connection::ConnectionPrivate::destroySocket() {
  if (socket) {
    socket->abort();
    socket->setParent(Q_NULLPTR);
  }

  delete socket;
}

Connection::Connection(QObject* parent)
  : QObject{parent}, m{new ConnectionPrivate{}} {
  m->socket = new QTcpSocket{this};

  connect(m->socket, &QTcpSocket::connected,
          this, &Connection::socketConnected);
  connect(m->socket, &QTcpSocket::disconnected,
          this, &Connection::socketDisconnected);
  connect(m->socket,
          QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
          this,
          &Connection::socketError);
  connect(m->socket, &QTcpSocket::readyRead,
          this, &Connection::socketReadyRead);
  connect(m->socket, &QTcpSocket::bytesWritten,
          this, &Connection::socketBytesWritten);

  m->server = new QTcpServer{this};
  connect(m->server, &QTcpServer::newConnection,
          this, &Connection::serverNewConnection);

  m->toggleModeTimer = new QTimer{this};
  connect(m->toggleModeTimer, &QTimer::timeout,
          this, &Connection::toggleConnectionMode);

  m->reconnectTimer = new QTimer{this};
  connect(m->reconnectTimer, &QTimer::timeout,
          this, &Connection::connectToPeer);
}

Connection::~Connection() {
  m->toggleModeTimer->stop();
  m->reconnectTimer->stop();

  delete m;
}

void Connection::sendMessage(const QString& msg) {
  if (!msg.isEmpty() && m->socket->state() == QAbstractSocket::ConnectedState) {
    const qint64 errorCheck = m->socket->write(msg.toUtf8());
    m->socket->flush();

    if (errorCheck != -1) {
      emit outgoingMessage(msg, QStringLiteral("Contact"));
    }
  }
}

void Connection::ip(const QString& ip) {
  if (ip != m->ip && !ip.isEmpty()) {
    m->ip = ip;
    m->socket->abort();
    toggleConnectionMode();
  }
}

void Connection::port(const int port) {
  if (port != m->port && port > 0) {
    m->port = port;
    m->socket->abort();
    toggleConnectionMode();
  }
}

void Connection::toggleConnectionMode() {
  if (!m->toggleModeTimer->isActive()) {
    m->toggleModeTimer->start(kConnectionModeTimeout);
  }

  if (QAbstractSocket::UnconnectedState == m->socket->state()) {
    if (m->connectionAttempts > 10) {
      m->toggleModeTimer->stop();
      m->toggleModeTimer->start(kConnectionModeTimeout + 2500);
    }
    else if (m->connectionAttempts > 20) {
      m->toggleModeTimer->stop();
      m->toggleModeTimer->start(kConnectionModeTimeout);
      m->connectionAttempts = 0;
    }

    if (ConnectionMode::ServerMode == m->mode) {
      m->mode = ConnectionMode::ClientMode;

      m->server->close();

      m->destroySocket();
      m->socket = new QTcpSocket{this};
      connectSocket();

      m->reconnectTimer->start(kReconnectTimeout);
    }
    else if (ConnectionMode::ClientMode == m->mode) {
      m->mode = ConnectionMode::ServerMode;

      // Stop client mode reconnect timer
      m->reconnectTimer->stop();

      if (m->port > 0) {
        m->socket->abort();

        const bool listeningCheck = m->server->listen(QHostAddress::Any,
                                                      m->port);

        emit sendStatus();
      }
    }
    else {
      emit displayError(QStringLiteral("Can't connect: No such connection "
                                       "mode."));
    }

    m->connectionAttempts = m->connectionAttempts + 1;
  }
}

void Connection::connectToPeer() {
  if (!m->ip.isEmpty() && m->port > 0 &&
      QAbstractSocket::UnconnectedState == m->socket->state()) {
    m->socket->connectToHost(m->ip, m->port);
    emit sendStatus();
  }
}

void Connection::socketConnected() {
  m->connectionAttempts = 0;
  emit sendStatus();
}

void Connection::socketError() {
  emit displayError(QStringLiteral("Couldn't connect. Reconnecting..."));
  sendStatus();
}

void Connection::socketDisconnected() {
  sendStatus();
}

void Connection::socketReadyRead() {
  const auto msg = m->socket->readAll();
  emit incomingMessage(msg);
}

void Connection::socketBytesWritten(const qint64 numberOfBytes) {
  Q_UNUSED(numberOfBytes);
}

void Connection::serverNewConnection() {
  m->destroySocket();

  // Grab the server socket
  m->socket = m->server->nextPendingConnection();

  // Connect new connected socket signals up to this class's slots
  connectSocket();

  m->server->close();

  emit sendStatus();
}

void Connection::connectSocket() {
  connect(m->socket, &QTcpSocket::connected,
          this, &Connection::socketConnected);
  connect(m->socket, &QTcpSocket::disconnected,
          this, &Connection::socketDisconnected);
  connect(m->socket, &QTcpSocket::readyRead,
          this, &Connection::socketReadyRead);
  connect(m->socket, &QTcpSocket::bytesWritten,
          this, &Connection::socketBytesWritten);
}

void Connection::sendStatus() {
  const auto reconnectTime =
    static_cast<int>(m->toggleModeTimer->remainingTime() / 1000);

  QString state = QStringLiteral("Not connected. Trying again in ") %
                  QString{"%1 "}.arg(reconnectTime) %
                  QStringLiteral("seconds...");

  if (QAbstractSocket::HostLookupState == m->socket->state()) {
    state = QStringLiteral("Looking up host...");
  }
  else if (QAbstractSocket::ConnectingState == m->socket->state()) {
    state = QStringLiteral("Connecting to peer as client...");
  }
  else if (m->server->isListening()) {
    state = QStringLiteral("Connecting to peer as server...");
  }
  else if (QAbstractSocket::ConnectedState == m->socket->state()) {
    state = QStringLiteral("Connected");
  }
  else if (QAbstractSocket::ClosingState == m->socket->state()) {
    state = QStringLiteral("Closing connection...");
  }

  emit updateStatus(state);
}
