#include "Dialogue.hpp"
#include <QCoreApplication>
#include <QGuiApplication>

int main(int argc, char* argv[]) {
  QGuiApplication app(argc, argv);
  app.setOrganizationName(QStringLiteral("The Dialogue Project"));
  app.setOrganizationDomain(QStringLiteral("andrewdolby.com"));
  app.setApplicationName(QStringLiteral("Dialogue"));

  Dialogue dialogue;

  const auto rc = app.exec();

  return rc;
}
