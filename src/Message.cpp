#include "Message.hpp"
#include <QDateTime>

Dialogue::Message::Message() {
}

Dialogue::Message::Message(const QString& msgText, const QString& recipient)
  : m_messageText{msgText},
    m_timestamp{QDateTime::currentDateTime().toString(Qt::ISODate)},
    m_recipient{recipient} {
}

QString Dialogue::Message::messageText() const {
  return m_messageText;
}

QString Dialogue::Message::timestamp() const {
  return m_timestamp;
}

QString Dialogue::Message::recipient() const {
  return m_recipient;
}
