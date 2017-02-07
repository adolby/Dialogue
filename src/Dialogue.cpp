#include "Dialogue.hpp"
#include "MessageModel.hpp"
#include "ConnectionInterface.hpp"
#include "Connection.hpp"
#include "Thread.hpp"
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

class DialoguePrivate {
  Q_DISABLE_COPY(DialoguePrivate)

 public:
  explicit DialoguePrivate(Dialogue* parent);

  QQmlApplicationEngine engine;
  ConnectionInterface* connectionInterface;
  MessageModel* messageModel;

  Connection connection;
  Thread connectionThread;
};

DialoguePrivate::DialoguePrivate(Dialogue* parent)
  : connectionInterface{new ConnectionInterface{parent}},
    messageModel{new MessageModel{parent}},
    connectionThread{parent} {
}

Dialogue::Dialogue(QObject* parent)
  : QObject{parent}, d_ptr{new DialoguePrivate{this}} {
  Q_D(Dialogue);

  d->engine.rootContext()->setContextProperty(QStringLiteral("messageModel"),
                                              d->messageModel);
  d->engine.rootContext()->setContextProperty(QStringLiteral("socketConnection"),
                                              d->connectionInterface);

  d->engine.load(QUrl{QStringLiteral("qrc:/main.qml")});

  auto root = d->engine.rootObjects().first();
  auto parentWindow = qobject_cast<QQuickWindow*>(root);

  connect(&d->connectionThread, &QThread::finished,
          this, &Dialogue::cleanup,
          Qt::DirectConnection);

  connect(parentWindow, SIGNAL(message(QString)),
          &d->connection, SLOT(sendMessage(QString)),
          Qt::QueuedConnection);
  connect(parentWindow, SIGNAL(ip(QString)),
          &d->connection, SLOT(ip(QString)),
          Qt::QueuedConnection);
  connect(parentWindow, SIGNAL(port(int)),
          &d->connection, SLOT(port(int)),
          Qt::QueuedConnection);

  connect(&d->connection, &Connection::updateStatus,
          d->connectionInterface, &ConnectionInterface::updateStatus);
  connect(&d->connection, &Connection::incomingMessage,
          d->connectionInterface, &ConnectionInterface::incomingMessage);
  connect(&d->connection, &Connection::outgoingMessage,
          d->connectionInterface, &ConnectionInterface::outgoingMessage);

  connect(d->connectionInterface,
          &ConnectionInterface::message,
          d->messageModel,
          QOverload<const QString&, const QString&>::
            of(&MessageModel::addMessage));

  d->connection.moveToThread(&d->connectionThread);
  d->connectionThread.start();
}

Dialogue::~Dialogue() {
}

void Dialogue::cleanup() {
  Q_D(Dialogue);

  d->connection.moveToThread(this->thread());
}
