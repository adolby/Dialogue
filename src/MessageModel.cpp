#include "MessageModel.hpp"
#include <QDateTime>

Message::Message() {
}

Message::Message(const QString& msgText, const QString& recipient)
  : m_messageText{msgText},
    m_timestamp{QDateTime::currentDateTime().toString(Qt::ISODate)},
    m_recipient{recipient} {
}

QString Message::messageText() const {
  return m_messageText;
}

QString Message::timestamp() const {
  return m_timestamp;
}

QString Message::recipient() const {
  return m_recipient;
}

MessageModel::MessageModel(QObject* parent)
  : QAbstractListModel{parent} {
}

void MessageModel::addMessage(const QString& msgText,
                              const QString& recipient) {
  addMessage(Message{msgText, recipient});
}

int MessageModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);

  return static_cast<int>(m_messages.size());
}

QVariant MessageModel::data(const QModelIndex& index, int role) const {
  const auto size = static_cast<int>(m_messages.size());

  if (index.row() < 0 || index.row() >= size) {
    return QVariant{};
  }

  const Message& message = m_messages.at(index.row());

  if (role == MessageTextRole) {
    return message.messageText();
  }
  else if (role == TimestampRole) {
    return message.timestamp();
  }
  else if (role == RecipientRole) {
    return message.recipient();
  }

  return QVariant{};
}

QHash<int, QByteArray> MessageModel::roleNames() const {
  QHash<int, QByteArray> roles;

  roles[MessageTextRole] = "messageText";
  roles[TimestampRole] = "timestamp";
  roles[RecipientRole] = "recipient";

  return roles;
}

void MessageModel::addMessage(const Message& msg) {
  beginInsertRows(QModelIndex(), 0, 0);
  m_messages.push_front(msg);
  endInsertRows();
}
