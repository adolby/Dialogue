QT += core network qml quick

TARGET = ServerTest

TEMPLATE = app

CONFIG += c++14

RESOURCES += ../../src/qml.qrc

# Qt Creator Debug/Release Differentiation
# Ensure one "debug_and_release" in CONFIG, for clarity.
debug_and_release {
  CONFIG -= debug_and_release
  CONFIG += debug_and_release
}
# Ensure one "debug" or "release" in CONFIG so they can be used as conditionals
# instead of writing "CONFIG(debug, debug|release)"
CONFIG(debug, debug|release) {
  CONFIG -= debug release
  CONFIG += debug
}
CONFIG(release, debug|release) {
  CONFIG -= debug release
  CONFIG += release
}

SOURCES += \
  ../../src/main.cpp \
  ../../src/SslServer.cpp \
  ../../src/Dialog.cpp \
  Connection.cpp

HEADERS += \
  ../../src/pimpl.h \
  ../../src/pimpl_impl.h \
  ../../src/SslServer.hpp \
  ../../src/Dialog.hpp \
  Connection.hpp

macx {
  message(macOS)

  debug {
    message(Debug)
    DESTDIR = build/macOS/clang/x86_64/debug/test/ServerTest/
  }
  release {
    message(Release)
    DESTDIR = build/macOS/clang/x86_64/release/test/ServerTest/
  }
}

OBJECTS_DIR = $$DESTDIR/obj
MOC_DIR = $$DESTDIR/moc
RCC_DIR = $$DESTDIR/qrc

include(../../deployment.pri)
