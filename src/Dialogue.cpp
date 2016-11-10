#include "Dialogue.hpp"
#include "MessageModel.hpp"
#include "ConnectionInterface.hpp"
#include "Connection.hpp"
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#if defined(Q_OS_ANDROID)
#else
#include "utility/pimpl_impl.h"
#endif

class Dialogue::DialoguePrivate {
public:
  DialoguePrivate();
  virtual ~DialoguePrivate();

  QQmlApplicationEngine engine;
  std::unique_ptr<Connection> connection;
  std::unique_ptr<ConnectionInterface> connectionInterface;
  std::unique_ptr<MessageModel> messageModel;
  std::unique_ptr<QThread> connectionThread;
};

Dialogue::Dialogue(QObject* parent)
  : QObject{parent}
{
#if defined(Q_OS_ANDROID)
  m = new DialoguePrivate{};
#endif

  m->engine.rootContext()->setContextProperty("messageModel",
                                              m->messageModel.get());
  m->engine.rootContext()->setContextProperty("socketConnection",
                                              m->connectionInterface.get());

  m->engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

  auto root = m->engine.rootObjects().first();
  auto parentWindow = qobject_cast<QQuickWindow*>(root);

  connect(parentWindow, SIGNAL(message(QString)),
          m->connection.get(), SLOT(sendMessage(QString)));
  connect(parentWindow, SIGNAL(ip(QString)),
          m->connection.get(), SLOT(ip(QString)));
  connect(parentWindow, SIGNAL(port(int)),
          m->connection.get(), SLOT(port(int)));

  connect(m->connection.get(), &Connection::updateStatus,
          m->connectionInterface.get(), &ConnectionInterface::updateStatus);
  connect(m->connection.get(), &Connection::incomingMessage,
          m->connectionInterface.get(), &ConnectionInterface::incomingMessage);
  connect(m->connection.get(), &Connection::outgoingMessage,
          m->connectionInterface.get(), &ConnectionInterface::outgoingMessage);

  connect(m->connectionInterface.get(),
          &ConnectionInterface::message,
          m->messageModel.get(),
          QOverload<const QString&, const QString&>::of(&MessageModel::addMessage));

  m->connection->moveToThread(m->connectionThread.get());
  m->connectionThread->start();
}

Dialogue::~Dialogue()
{
  m->connectionThread->quit();
  auto timedOut = !m->connectionThread->wait(2000);

  // If the thread couldn't quit within the timeout, terminate it
  if (timedOut)
  {
    m->connectionThread->terminate();
  }

#if defined(Q_OS_ANDROID)
  delete m;
#endif
}

Dialogue::DialoguePrivate::DialoguePrivate()
  :
#if defined(Q_OS_ANDROID)
   connection{new Connection{}}
#else
   connection{std::make_unique<Connection>()},
   connectionInterface{std::make_unique<ConnectionInterface>()},
   messageModel{std::make_unique<MessageModel>()},
   connectionThread{std::make_unique<QThread>()}
#endif
{}

Dialogue::DialoguePrivate::~DialoguePrivate() {}
