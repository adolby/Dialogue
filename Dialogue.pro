QT += core network qml quick

TARGET = Dialogue

TEMPLATE = app

CONFIG += c++11

RESOURCES += src/qml.qrc resources/assets.qrc

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

HEADERS += \
  src/Application.hpp \
  src/Connection.hpp \
  src/SslServer.hpp \
  src/Message.hpp \
  src/MessageModel.hpp \
  src/Thread.hpp

SOURCES += \
  src/main.cpp \
  src/Application.cpp \
  src/Connection.cpp \
  src/SslServer.cpp \
  src/Message.cpp \
  src/MessageModel.cpp \
  src/Thread.cpp

# Platform-specific configuration
android {
  message(Android)

  ANDROID_PACKAGE_SOURCE_DIR = $$PWD/resources/android

  DISTFILES += \
    resources/android/AndroidManifest.xml \
    resources/android/gradle/wrapper/gradle-wrapper.jar \
    resources/android/gradlew \
    resources/android/res/values/libs.xml \
    resources/android/build.gradle \
    resources/android/gradle/wrapper/gradle-wrapper.properties \
    resources/android/gradlew.bat

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

    LIBS += /usr/local/Cellar/openssl/1.0.2k/lib/libssl.a \
            /usr/local/Cellar/openssl/1.0.2k/lib/libcrypto.a

    QMAKE_CXXFLAGS += -fstack-protector
    QMAKE_LFLAGS += -fstack-protector

    QMAKE_TARGET_BUNDLE_PREFIX = com.andrewdolby
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
