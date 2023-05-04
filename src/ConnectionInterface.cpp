#include "ConnectionInterface.hpp"

ConnectionInterface::ConnectionInterface(QObject* parent)
  : QObject(parent), status_(tr("Not connected")) {
}

QString ConnectionInterface::status() const {
  return status_;
}

void ConnectionInterface::updateStatus(const QString& status) {
  status_ = status;
  emit statusChanged();
}

void ConnectionInterface::incomingMessage(const QString& msgText) {
  emit message(msgText, QStringLiteral("Me"));
}

void ConnectionInterface::outgoingMessage(const QString& msgText,
                                          const QString& recipient) {
  emit message(msgText, recipient);
}
