QT += core network qml quick

TARGET = Dialogue

TEMPLATE = app

CONFIG += c++11

RESOURCES += src/qml.qrc

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
  src/main.cpp \
  src/Dialogue.cpp \
  src/Connection.cpp \
  src/SslServer.cpp \
  src/ConnectionInterface.cpp \
  src/MessageModel.cpp \
  src/Thread.cpp

HEADERS += \
  src/Dialogue.hpp \
  src/Connection.hpp \
  src/SslServer.hpp \
  src/ConnectionInterface.hpp \
  src/MessageModel.hpp \
  src/Thread.hpp

# Platform-specific configuration
android {
  message(Android)

  HEADERS +=

  ANDROID_PACKAGE_SOURCE_DIR = resources/android/

  OTHER_FILES += resources/android/AndroidManifest.xml

  debug {
    message(Debug)
    DESTDIR = build/android/debug/
  }
  release {
    message(Release)
    DESTDIR = build/android/release/
  }
} else:ios {
  message(iOS)
  message(clang)

  debug {
    message(Debug)
    DESTDIR = build/iOS/debug/
  }
  release {
    message(Release)
    DESTDIR = build/iOS/release/
  }
} else { # Desktop OS
  linux {
    message(Linux)

    QMAKE_CXXFLAGS += -fstack-protector
    QMAKE_LFLAGS += -fstack-protector
    QMAKE_LFLAGS += -Wl,-rpath,"'\$$ORIGIN'"

    linux-clang {
      message(clang x86_64)

      debug {
        message(Debug)
        DESTDIR = build/linux/clang/x86_64/debug/Dialogue/
      }
      release {
        message(Release)
        DESTDIR = build/linux/clang/x86_64/release/Dialogue/
      }
    }

    linux-g++-64 {
      message(g++ x86_64)

      debug {
        message(Debug)
        DESTDIR = build/linux/gcc/x86_64/debug/Dialogue/
      }
      release {
        message(Release)
        DESTDIR = build/linux/gcc/x86_64/release/Dialogue/
      }
    }
  } # End Linux

  macx {
    message(macOS)
    message(clang x86_64)

    QMAKE_CXXFLAGS += -fstack-protector
    QMAKE_LFLAGS += -fstack-protector

    ICON = resources/icons/dialogue.icns

    debug {
      message(Debug)
      DESTDIR = build/macOS/clang/x86_64/debug/
    }
    release {
      message(Release)
      DESTDIR = build/macOS/clang/x86_64/release/
    }
  }

  win32 {
    message(Windows)

    HEADERS +=

    win32-g++ {
      message(MinGW x86)

      debug {
        message(Debug)
        DESTDIR = build/windows/mingw/x86/debug/Dialogue/
      }
      release {
        message(Release)
        DESTDIR = build/windows/mingw/x86/release/Dialogue/
      }
    }

    win32-msvc2015 {
      contains(QT_ARCH, x86_64) {
        message(MSVC x86_64)

        debug {
          message(Debug)
          DESTDIR = build/windows/msvc/x86_64/debug/Dialogue/
        }
        release {
          message(Release)
          DESTDIR = build/windows/msvc/x86_64/release/Dialogue/
        }
      } else {
        message(MSVC x86)

        debug {
          message(Debug)
          DESTDIR = build/windows/msvc/x86/debug/Dialogue/
        }
        release {
          message(Release)
          DESTDIR = build/windows/msvc/x86/release/Dialogue/
        }
      }
    }

    RC_ICONS += resources/icons/dialogue.ico
  } # End win32
} # End desktop

OBJECTS_DIR = $$DESTDIR/obj
MOC_DIR = $$DESTDIR/moc
RCC_DIR = $$DESTDIR/qrc

include(deployment.pri)
