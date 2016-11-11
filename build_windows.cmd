echo on

SET project_dir="%cd%"

echo Set up environment...
set PATH=%QT%\bin\;C:\Qt\Tools\QtCreator\bin\;C:\Qt\QtIFW2.0.1\bin\;%PATH%
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" %PLATFORM%

echo Building Dialogue...
qmake -spec win32-msvc2015 CONFIG+=x86_64 CONFIG-=debug CONFIG+=release
nmake

echo Packaging...
cd %project_dir%\build\windows\msvc\x86_64\release\
windeployqt --qmldir ..\..\..\..\..\src\ Dialogue\Dialogue.exe

rd /s /q Dialogue\moc\
rd /s /q Dialogue\obj\
rd /s /q Dialogue\qrc\

echo Copying left out Qt files...
copy "%QT%\bin\Qt5QuickControls2.dll" "Dialogue\Qt5QuickControls2.dll"
copy "%QT%\bin\Qt5QuickTemplates2.dll" "Dialogue\Qt5QuickTemplates2.dll"
mkdir Dialogue\Qt\
mkdir Dialogue\Qt\labs\
mkdir Dialogue\Qt\labs\settings\
mkdir Dialogue\QtGraphicalEffects\
mkdir Dialogue\QtQuick\
mkdir Dialogue\QtQuick.2\
robocopy "%QT%\qml\Qt\labs\settings" Dialogue\Qt\labs\settings\ /E
robocopy "%QT%\qml\QtGraphicalEffects" Dialogue\QtGraphicalEffects\ /E
robocopy "%QT%\qml\QtQuick" Dialogue\QtQuick\ /E
robocopy "%QT%\qml\QtQuick.2" Dialogue\QtQuick.2\ /E


echo Copying project files for archival...
copy "%project_dir%\README.md" "Dialogue\README.md"
copy "%project_dir%\LICENSE" "Dialogue\LICENSE.txt"
copy "%project_dir%\Qt License" "Dialogue\Qt License.txt"

echo Copying files for installer...
mkdir "%project_dir%\installer\windows\x86_64\packages\com.dialogueproject.dialogue\data\"
robocopy Dialogue\ "%project_dir%\installer\windows\x86_64\packages\com.dialogueproject.dialogue\data" /E

echo Packaging portable archive...
7z a dialogue_%TAG_NAME%_windows_x86_64_portable.zip Dialogue

echo Creating installer...
cd %project_dir%\installer\windows\x86_64\
binarycreator.exe --offline-only -c config\config.xml -p packages dialogue_%TAG_NAME%_windows_x86_64_installer.exe
