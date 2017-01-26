#include "Dialogue.hpp"
#include <QCoreApplication>
#include <QGuiApplication>

int main(int argc, char* argv[])
{
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication app{argc, argv};
  app.setOrganizationName(QStringLiteral("The Dialogue Project"));
  app.setOrganizationDomain(QStringLiteral("andrewdolby.com"));
  app.setApplicationName(QStringLiteral("Dialogue"));

  Dialogue dialogue{};

  const auto retval = app.exec();

  return retval;
}
