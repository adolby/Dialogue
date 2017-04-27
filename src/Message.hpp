#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include <QObject>

namespace Dialogue {

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

}

#endif // MESSAGE_HPP_
