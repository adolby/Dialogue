#include "Connection.hpp"
#include "../../src/utility/pimpl_impl.h"
#include <QTcpSocket>
#include <QTcpServer>

class Connection::ConnectionPrivate {
 public:
  ConnectionPrivate();
  virtual ~ConnectionPrivate();

  void destroySocket();

  QTcpSocket* socket;
  QTcpServer* server;

  int port;
};

Connection::ConnectionPrivate::ConnectionPrivate()
  : socket{Q_NULLPTR}, server{Q_NULLPTR}, port{0}
  {}

Connection::ConnectionPrivate::~ConnectionPrivate() {}

void Connection::ConnectionPrivate::destroySocket() {}

Connection::Connection(QObject* parent)
  : QObject{parent}
{
  m->server = new QTcpServer{this};

  connect(m->server, &QTcpServer::newConnection,
          this, &Connection::serverNewConnection);
}

Connection::~Connection()
{
  m->server->close();
}

void Connection::chatMessage(const QString& msg) {}

void Connection::ip(const QString& ip) {}

void Connection::port(const int port)
{
  if (port != m->port && port > 0)
  {
    m->port = port;
    m->server->listen(QHostAddress::Any, port);
  }
}

void Connection::connectSocket()
{
  connect(m->socket, &QTcpSocket::connected,
          this, &Connection::socketConnected);
  connect(m->socket, &QTcpSocket::disconnected,
          this, &Connection::socketDisconnected);
  connect(m->socket, &QTcpSocket::readyRead,
          this, &Connection::socketReadyRead);
  connect(m->socket, &QTcpSocket::bytesWritten,
          this, &Connection::socketBytesWritten);
}

void Connection::swapModes() {}

void Connection::connectToPeer() {}

void Connection::socketConnected()
{
  qDebug() << "Socket connected!";
}

void Connection::socketError()
{
  qDebug() << "Socket error: " << m->socket->errorString();
}

void Connection::socketDisconnected()
{
  qDebug() << "Socket disconnected!";
}

void Connection::socketReadyRead()
{
  const auto responseMsg = m->socket->readAll();

  qDebug() << "Socket reading: " << responseMsg;
}

void Connection::socketBytesWritten(const qint64 numberOfBytes)
{
  qDebug() << "Bytes written by client: " << numberOfBytes;
}

void Connection::serverNewConnection()
{
  qDebug() << "Server new connection!";

  // Grab the server socket
  m->socket = m->server->nextPendingConnection();
  connectSocket();
}
