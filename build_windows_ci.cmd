echo on

echo "Setting up environment..."

SET PROJECT_DIR=%SOURCE_DIR%
SET QT_PATH=%Qt6_DIR%
SET QT_TOOLS=%IQTA_TOOLS%

SET PATH=%QT_PATH%\bin\;%QT_TOOLS%\QtInstallerFramework\4.5\bin;%PATH%
call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" %ARCH%

echo "Check qmake version..."
qmake --version

cd %PROJECT_DIR%

echo "Building Dialogue..."
qmake -makefile -spec win32-msvc CONFIG+=x86_64 CONFIG-=debug CONFIG+=release
nmake

echo "Packaging..."

echo "Copying app to packaging directory..."

robocopy "%PROJECT_DIR%\build\windows\msvc\x86_64\release\" "%PROJECT_DIR%\build\windows\msvc\x86_64\release\Dialogue\" /E

cd "%PROJECT_DIR%\build\windows\msvc\x86_64\release\"
windeployqt --qmldir ..\..\..\..\..\src\ Dialogue\Dialogue.exe

rd /s /q Dialogue\moc\
rd /s /q Dialogue\obj\
rd /s /q Dialogue\qrc\

echo "Copying files for archival..."
copy "%PROJECT_DIR%\README.md" "Dialogue\README.md"
copy "%PROJECT_DIR%\LICENSE" "Dialogue\LICENSE.txt"
copy "%PROJECT_DIR%\Qt License" "Dialogue\Qt License.txt"

echo "Copying files for installer..."
mkdir "%PROJECT_DIR%\installer\windows\x86_64\packages\app.dialogue\data\"
robocopy Dialogue\ "%PROJECT_DIR%\installer\windows\x86_64\packages\app.dialogue\data" /E

echo "Packaging portable archive..."
7z a -aoa dialogue_%DIALOGUE_VERSION%_windows_x86_64_portable.zip Dialogue

echo "Creating installer..."
cd "%PROJECT_DIR%\installer\windows\x86_64\"
binarycreator.exe --offline-only -c config\config.xml -p packages dialogue_%DIALOGUE_VERSION%_windows_x86_64_installer.exe
