#ifndef DIALOGUE_MESSAGEMODEL_HPP_
#define DIALOGUE_MESSAGEMODEL_HPP_

#include <QAbstractListModel>
#include <QHash>
#include <QString>
#include <deque>

class Message {
 public:
  Message();
  Message(const QString& msgText, const QString& recipient);

  QString messageText() const;
  QString timestamp() const;
  QString recipient() const;

 private:
  QString m_messageText;
  QString m_timestamp;
  QString m_recipient;
};

class MessageModel : public QAbstractListModel {
  Q_OBJECT

 public:
  enum MessageRoles {
    RecipientRole = Qt::UserRole + 2,
    TimestampRole = Qt::UserRole + 1,
    MessageTextRole = Qt::UserRole
  };

  MessageModel(QObject* parent = Q_NULLPTR);

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

 public slots:
  void addMessage(const QString& msgText, const QString& recipient);

 protected:
  QHash<int, QByteArray> roleNames() const;

 private:
  void addMessage(const Message& msg);

  std::deque<Message> m_messages;
};

#endif // DIALOGUE_MESSAGEMODEL_HPP_
