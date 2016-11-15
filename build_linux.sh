#!/bin/bash

set -o errexit -o nounset

# Update platform
echo "Updating platform..."
sudo -E apt-get -yq --no-install-suggests --no-install-recommends --force-yes install p7zip-full
sudo -E apt-get -yq --no-install-suggests --no-install-recommends --force-yes install chrpath

# Hold on to current directory
project_dir=$(pwd)
qt_install_dir=/usr/local

# Get Qt
echo "Installing Qt..."
cd ${qt_install_dir}
echo "Downloading Qt files..."
sudo wget https://github.com/adolby/qt-more-builds/releases/download/5.7/qt-opensource-5.7.0-linux-x86_64-gcc6.7z
echo "Extracting Qt files..."
sudo 7z x qt-opensource-5.7.0-linux-x86_64-gcc6.7z &> /dev/null

# Install Qt Installer Framework
echo "Installing Qt Installer Framework..."
sudo wget https://github.com/adolby/qt-more-builds/releases/download/qt-ifw-2.0.3/qt-installer-framework-opensource-2.0.3-linux.7z
sudo 7z x qt-installer-framework-opensource-2.0.3-linux.7z &> /dev/null

# Add Qt binaries to path
echo "Adding Qt binaries to path..."
PATH=${qt_install_dir}/Qt/5.7/gcc_64/bin/:${qt_install_dir}/QtIFW2.0.3/bin/:${PATH}
echo "New PATH:"
echo ${PATH}

# Build Dialogue
echo "Building Dialogue..."
cd ${project_dir}
qmake -v
qmake -config release
make

# Package Dialogue
echo "Packaging..."
cd ${project_dir}/build/linux/gcc/x86_64/release/Dialogue/

rm -rf moc
rm -rf obj
rm -rf qrc

echo "Copying files for archival..."
mkdir platforms
mkdir -p Qt/labs/

cp "${qt_install_dir}/Qt/5.7/gcc_64/plugins/platforms/libqxcb.so" "platforms/libqxcb.so"
cp "${qt_install_dir}/Qt/5.7/gcc_64/plugins/platforms/libqminimal.so" "platforms/libqminimal.so"

cp "${qt_install_dir}/Qt/5.7/gcc_64/lib/libQt5Core.so.5.7.0" "libQt5Core.so.5"
cp "${qt_install_dir}/Qt/5.7/gcc_64/lib/libQt5Gui.so.5.7.0" "libQt5Gui.so.5"
cp "${qt_install_dir}/Qt/5.7/gcc_64/lib/libQt5Svg.so.5.7.0" "libQt5Svg.so.5"
cp "${qt_install_dir}/Qt/5.7/gcc_64/lib/libQt5Qml.so.5.7.0" "libQt5Qml.so.5"
cp "${qt_install_dir}/Qt/5.7/gcc_64/lib/libQt5Quick.so.5.7.0" "libQt5Quick.so.5"
cp "${qt_install_dir}/Qt/5.7/gcc_64/lib/libQt5QuickControls2.so.5.7.0" "libQt5QuickControls2.so.5"
cp "${qt_install_dir}/Qt/5.7/gcc_64/lib/libQt5QuickTemplates2.so.5.7.0" "libQt5QuickTemplates2.so.5"
cp "${qt_install_dir}/Qt/5.7/gcc_64/lib/libQt5Network.so.5.7.0" "libQt5Network.so.5"
cp "${qt_install_dir}/Qt/5.7/gcc_64/lib/libQt5DBus.so.5.7.0" "libQt5DBus.so.5"
cp "${qt_install_dir}/Qt/5.7/gcc_64/lib/libQt5XcbQpa.so.5.7.0" "libQt5XcbQpa.so.5"

cp -R "${qt_install_dir}/Qt/5.7/gcc_64/qml/Qt/labs/settings/" "Qt/labs/"
cp -R "${qt_install_dir}/Qt/5.7/gcc_64/qml/QtGraphicalEffects/" "."
cp -R "${qt_install_dir}/Qt/5.7/gcc_64/qml/QtQuick/" "."
cp -R "${qt_install_dir}/Qt/5.7/gcc_64/qml/QtQuick.2/" "."

cp "${project_dir}/README.md" "README.md"
cp "${project_dir}/LICENSE" "LICENSE"
cp "${project_dir}/Qt License" "Qt License"

chrpath -r \$ORIGIN/.. platforms/libqxcb.so
chrpath -r \$ORIGIN/.. platforms/libqminimal.so
chrpath -r \$ORIGIN/../.. QtQuick/Controls.2/libqtquickcontrols2plugin.so
chrpath -r \$ORIGIN/../.. QtQuick/Templates.2/libqtquicktemplates2plugin.so

echo "Copying files for installer..."
cp -R * "${project_dir}/installer/linux/packages/com.dialogueproject.dialogue/data/"

echo "Packaging portable archive..."
cd ..
7z a dialogue_${TAG_NAME}_linux_x86_64_portable.zip Dialogue

echo "Creating installer..."
cd "${project_dir}/installer/linux/"
binarycreator --offline-only -c config/config.xml -p packages dialogue_${TAG_NAME}_linux_x86_64_installer

echo "Done!"

exit 0
