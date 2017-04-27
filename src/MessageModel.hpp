#ifndef DIALOGUE_MESSAGEMODEL_HPP_
#define DIALOGUE_MESSAGEMODEL_HPP_

#include "Message.hpp"
#include <QAbstractListModel>
#include <QHash>
#include <QString>
#include <deque>

namespace Dialogue {

class MessageModel : public QAbstractListModel {
  Q_OBJECT

 public:
  enum MessageRoles {
    RecipientRole = Qt::UserRole + 2,
    TimestampRole = Qt::UserRole + 1,
    MessageTextRole = Qt::UserRole
  };

  explicit MessageModel(QObject* parent = Q_NULLPTR);

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

 public slots:
  void addMessage(const QString& msgText);
  void addMessage(const QString& msgText, const QString& recipient);

 protected:
  QHash<int, QByteArray> roleNames() const;

 private:
  void appendMessage(const Dialogue::Message& msg);

  std::deque<Message> m_messages;
};

}

#endif // DIALOGUE_MESSAGEMODEL_HPP_
