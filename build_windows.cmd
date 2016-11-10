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
windeployqt Dialogue\Dialogue.exe

rd /s /q Dialogue\moc\
rd /s /q Dialogue\obj\
rd /s /q Dialogue\qrc\

echo Copying files for archival...
copy "%project_dir%\README.md" "Dialogue\README.md"
copy "%project_dir%\LICENSE" "Dialogue\LICENSE.txt"
copy ".%project_dir%\Qt License" "Dialogue\Qt License.txt"

echo Copying files for installer...
mkdir %project_dir%\installer\windows\x86_64\packages\com.dialogueproject.dialogue\data\
robocopy Dialogue\ %project_dir%\installer\windows\x86_64\packages\com.dialogueproject.dialogue\data\ /E

echo Packaging portable archive...
7z a dialogue_%TAG_NAME%_windows_x86_64_portable.zip Dialogue

echo Creating installer...
cd %project_dir%\installer\windows\x86_64\
binarycreator.exe --offline-only -c config\config.xml -p packages dialogue_%TAG_NAME%_windows_x86_64_installer.exe
