#include "Dialogue.hpp"
#include "MessageModel.hpp"
#include "ConnectionInterface.hpp"
#include "Connection.hpp"
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

class Dialogue::DialoguePrivate {
 public:
  explicit DialoguePrivate();
  virtual ~DialoguePrivate();

  QQmlApplicationEngine engine;
  Connection* connection;
  ConnectionInterface* connectionInterface;
  MessageModel* messageModel;
  QThread* connectionThread;
};

Dialogue::DialoguePrivate::DialoguePrivate()
  : connection{Q_NULLPTR}, connectionInterface{Q_NULLPTR},
    messageModel{Q_NULLPTR}, connectionThread{Q_NULLPTR} {
}

Dialogue::DialoguePrivate::~DialoguePrivate() {
}

Dialogue::Dialogue(QObject* parent)
  : QObject{parent}, m{new DialoguePrivate{}} {
  m->connection = new Connection{};
  m->connectionInterface = new ConnectionInterface{this};
  m->messageModel = new MessageModel{this};
  m->connectionThread = new QThread{this};

  m->engine.rootContext()->setContextProperty("messageModel",
                                              m->messageModel);
  m->engine.rootContext()->setContextProperty("socketConnection",
                                              m->connectionInterface);

  m->engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

  auto root = m->engine.rootObjects().first();
  auto parentWindow = qobject_cast<QQuickWindow*>(root);

  connect(parentWindow, SIGNAL(message(QString)),
          m->connection, SLOT(sendMessage(QString)));
  connect(parentWindow, SIGNAL(ip(QString)),
          m->connection, SLOT(ip(QString)));
  connect(parentWindow, SIGNAL(port(int)),
          m->connection, SLOT(port(int)));

  connect(m->connection, &Connection::updateStatus,
          m->connectionInterface, &ConnectionInterface::updateStatus);
  connect(m->connection, &Connection::incomingMessage,
          m->connectionInterface, &ConnectionInterface::incomingMessage);
  connect(m->connection, &Connection::outgoingMessage,
          m->connectionInterface, &ConnectionInterface::outgoingMessage);

  connect(m->connectionInterface,
          &ConnectionInterface::message,
          m->messageModel,
          QOverload<const QString&, const QString&>::of(&MessageModel::addMessage));

  m->connection->moveToThread(m->connectionThread);
  m->connectionThread->start();
}

Dialogue::~Dialogue() {
  m->connectionThread->quit();
  auto timedOut = !m->connectionThread->wait(2000);

  // If the thread couldn't quit within the timeout, terminate it
  if (timedOut) {
    m->connectionThread->terminate();
  }

  delete m;
}
