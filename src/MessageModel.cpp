#include "MessageModel.hpp"

#include <QDebug>

Dialogue::MessageModel::MessageModel(QObject* parent)
  : QAbstractListModel{parent} {
}

int Dialogue::MessageModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return static_cast<int>(m_messages.size());
}

QVariant Dialogue::MessageModel::data(const QModelIndex& index,
                                      int role) const {
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

void Dialogue::MessageModel::addMessage(const QString& msgText) {
  addMessage(msgText, tr("Me"));
}

void Dialogue::MessageModel::addMessage(const QString& msgText,
                                        const QString& recipient) {
  appendMessage(Message{msgText, recipient});
}

QHash<int, QByteArray> Dialogue::MessageModel::roleNames() const {
  QHash<int, QByteArray> roles;

  roles[MessageTextRole] = "messageText";
  roles[TimestampRole] = "timestamp";
  roles[RecipientRole] = "recipient";

  return roles;
}

void Dialogue::MessageModel::appendMessage(const Message& msg) {
  qDebug() << "Appending msg: " << msg.messageText();

  beginInsertRows(QModelIndex{}, 0, 0);
  m_messages.push_front(msg);
  endInsertRows();
}
