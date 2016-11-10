#include "Dialogue.hpp"
#include <QCoreApplication>
#include <QGuiApplication>

int main(int argc, char* argv[])
{
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication app{argc, argv};
  app.setOrganizationName("The Dialogue Project");
  app.setOrganizationDomain("andrewdolby.com");
  app.setApplicationName("Dialogue");

  Dialogue dialogue{};

  auto retval = app.exec();

  return retval;
}
