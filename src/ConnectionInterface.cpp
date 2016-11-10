#include "ConnectionInterface.hpp"

ConnectionInterface::ConnectionInterface(QObject* parent)
  : QObject{parent}, m_status{"Not connected"}
{}

QString ConnectionInterface::status() const
{
  return m_status;
}

void ConnectionInterface::updateStatus(const QString& status)
{
  m_status = status;
  emit statusChanged();
}

void ConnectionInterface::incomingMessage(const QString& msgText)
{
  emit message(msgText, "Me");
}

void ConnectionInterface::outgoingMessage(const QString& msgText,
                                          const QString& recipient)
{
  emit message(msgText, recipient);
}
