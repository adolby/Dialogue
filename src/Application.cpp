#include "Application.hpp"
#include "MessageModel.hpp"
#include "ConnectionInterface.hpp"
#include "Connection.hpp"
#include "Thread.hpp"
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QQuickWindow>

class Dialogue::ApplicationPrivate {
  Q_DISABLE_COPY(ApplicationPrivate)

 public:
  explicit ApplicationPrivate(Application* parent);

  QQmlApplicationEngine engine;
  MessageModel messageModel;

  Connection connection;
  Thread connectionThread;
};

Dialogue::ApplicationPrivate::ApplicationPrivate(Application* parent)
  : messageModel{parent}, connectionThread{parent} {
}

Dialogue::Application::Application(QObject* parent)
  : QObject{parent}, d_ptr{new ApplicationPrivate{this}} {
  Q_D(Application);

  connect(&d->connectionThread, &QThread::finished,
          this, &Application::cleanup,
          Qt::DirectConnection);

  connect(&d->connection,
          &Connection::incomingMessage,
          &d->messageModel,
          QOverload<const QString&>::of(&MessageModel::addMessage));
  connect(&d->connection,
          &Connection::outgoingMessage,
          &d->messageModel,
          QOverload<const QString&, const QString&>::
            of(&MessageModel::addMessage));

  connect(&d->connection, &Connection::updateStatus,
          this, &Application::updateStatus);

  connect(&d->engine, &QQmlApplicationEngine::objectCreated,
          this, &Application::connectGUI);

  d->engine.rootContext()->setContextProperty(QStringLiteral("messageModel"),
                                              &d->messageModel);

  d->engine.load(QUrl{QStringLiteral("qrc:/main.qml")});

  d->connection.moveToThread(&d->connectionThread);
  d->connectionThread.start();
}

Dialogue::Application::~Application() {
}

void Dialogue::Application::connectGUI(QObject* object, const QUrl& url) {
  Q_D(Application);
  Q_UNUSED(url);

  if ( object ) {
    auto parentWindow = qobject_cast<QQuickWindow*>(object);

    connect(parentWindow, SIGNAL(message(const QString&)),
            &d->connection, SLOT(sendMessage(const QString&)),
            Qt::QueuedConnection);
    connect(parentWindow, SIGNAL(ip(const QString&)),
            &d->connection, SLOT(ip(const QString&)),
            Qt::QueuedConnection);
    connect(parentWindow, SIGNAL(port(int)),
            &d->connection, SLOT(port(int)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(updateStatus(const QVariant&)),
            parentWindow, SLOT(updateStatus(const QVariant&)),
            Qt::DirectConnection);
  }
}

void Dialogue::Application::cleanup() {
  Q_D(Application);

  d->connection.moveToThread(this->thread());
}
