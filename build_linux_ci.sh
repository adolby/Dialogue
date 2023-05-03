#!/bin/bash

set -o errexit -o nounset

PROJECT_DIR="${SOURCE_DIR}"
QT_PATH="${Qt6_DIR}"
QT_TOOLS="${IQTA_TOOLS}"
KRYVO_VERSION="${KRYVO_VERSION:-dev}"
OPENSSL_INCLUDE_PATH="/usr/include"
OPENSSL_LIB_PATH="/usr/lib/x86_64-linux-gnu"

# Add Qt binaries to path
echo "Adding Qt binaries to path..."
PATH="${QT_PATH}/bin/:${QT_TOOLS}/QtInstallerFramework/4.5/bin/:${PATH}"

# Check qmake version
echo "Check qmake version..."
qmake --version

# Build Dialogue
echo "Building Dialogue..."
cd "${PROJECT_DIR}"

qmake -makefile -spec linux-g++ CONFIG+=release
make

# Package Dialogue
echo "Packaging..."

echo "Copying app to packaging directory..."
cp -a "${PROJECT_DIR}/build/linux/gcc/x86_64/release/." "${PROJECT_DIR}/build/linux/gcc/x86_64/release/Dialogue/"

cd "${PROJECT_DIR}/build/linux/gcc/x86_64/release/Kryvo/"

rm -rf moc
rm -rf obj
rm -rf qrc

echo "Copying Qt dependencies..."

cp -a "${QT_PATH}/lib/libicui18n.so.56.1" "./libicui18n.so.56.1"
ln -s "./libicui18n.so.56.1" "./libicui18n.so"
ln -s "./libicui18n.so.56.1" "./libicui18n.so.56"

cp -a "${QT_PATH}/lib/libicuuc.so.56.1" "./libicuuc.so.56.1"
ln -s "./libicuuc.so.56.1" "./libicuuc.so"
ln -s "./libicuuc.so.56.1" "./libicuuc.so.56"

cp -a "${QT_PATH}/lib/libicudata.so.56.1" "./libicudata.so.56.1"
ln -s "./libicudata.so.56.1" "./libicudata.so"
ln -s "./libicudata.so.56.1" "./libicudata.so.56"

mkdir platforms
cp -a "${QT_PATH}/plugins/platforms/libqxcb.so" "./platforms/libqxcb.so"
cp -a "${QT_PATH}/plugins/platforms/libqminimal.so" "./platforms/libqminimal.so"

cp -a "${QT_PATH}/lib/libQt6Core.so.6.5.0" "./libQt6Core.so.6.5.0"
ln -s "./libQt6Core.so.6.5.0" "./libQt6Core.so"
ln -s "./libQt6Core.so.6.5.0" "./libQt6Core.so.6"

cp -a "${QT_PATH}/lib/libQt6Gui.so.6.5.0" "./libQt6Gui.so.6.5.0"
ln -s "./libQt6Gui.so.6.5.0" "./libQt6Gui.so"
ln -s "./libQt6Gui.so.6.5.0" "./libQt6Gui.so.6"

cp -a "${QT_PATH}/lib/libQt6OpenGL.so.6.5.0" "./libQt6OpenGL.so.6.5.0"
ln -s "./libQt6OpenGL.so.6.5.0" "./libQt6OpenGL.so"
ln -s "./libQt6OpenGL.so.6.5.0" "./libQt6OpenGL.so.6"

cp -a "${QT_PATH}/lib/libQt6Svg.so.6.5.0" "./libQt6Svg.so.6.5.0"
ln -s "./libQt6Svg.so.6.5.0" "./libQt6Svg.so"
ln -s "./libQt6Svg.so.6.5.0" "./libQt6Svg.so.6"

cp -a "${QT_PATH}/lib/libQt6Qml.so.6.5.0" "./libQt6Qml.so.6.5.0"
ln -s "./libQt6Qml.so.6.5.0" "./libQt6Qml.so"
ln -s "./libQt6Qml.so.6.5.0" "./libQt6Qml.so.6"

cp -a "${QT_PATH}/lib/libQt6QmlCore.so.6.5.0" "./libQt6QmlCore.so.6.5.0"
ln -s "./libQt6QmlCore.so.6.5.0" "./libQt6QmlCore.so"
ln -s "./libQt6QmlCore.so.6.5.0" "./libQt6QmlCore.so.6"

cp -a "${QT_PATH}/lib/libQt6Quick.so.6.5.0" "./libQt6Quick.so.6.5.0"
ln -s "./libQt6Quick.so.6.5.0" "./libQt6Quick.so"
ln -s "./libQt6Quick.so.6.5.0" "./libQt6Quick.so.6"

cp -a "${QT_PATH}/lib/libQt6QuickLayouts.so.6.5.0" "./libQt6QuickLayouts.so.6.5.0"
ln -s "./libQt6QuickLayouts.so.6.5.0" "./libQt6QuickLayouts.so"
ln -s "./libQt6QuickLayouts.so.6.5.0" "./libQt6QuickLayouts.so.6"

cp -a "${QT_PATH}/lib/libQt6QuickControls2.so.6.5.0" "./libQt6QuickControls2.so.6.5.0"
ln -s "./libQt6QuickControls2.so.6.5.0" "./libQt6QuickControls2.so"
ln -s "./libQt6QuickControls2.so.6.5.0" "./libQt6QuickControls2.so.6"

cp -a "${QT_PATH}/lib/libQt6QuickTemplates2.so.6.5.0" "./libQt6QuickTemplates2.so.6.5.0"
ln -s "./libQt6QuickTemplates2.so.6.5.0" "./libQt6QuickTemplates2.so"
ln -s "./libQt6QuickTemplates2.so.6.5.0" "./libQt6QuickTemplates2.so.6"

cp -a "${QT_PATH}/lib/libQt6Network.so.6.5.0" "./libQt6Network.so.6.5.0"
ln -s "./libQt6Network.so.6.5.0" "./libQt6Network.so"
ln -s "./libQt6Network.so.6.5.0" "./libQt6Network.so.6"

cp -a "${QT_PATH}/lib/libQt6DBus.so.6.5.0" "./libQt6DBus.so.6.5.0"
ln -s "./libQt6DBus.so.6.5.0" "./libQt6DBus.so"
ln -s "./libQt6DBus.so.6.5.0" "./libQt6DBus.so.6"

cp -a "${QT_PATH}/lib/libQt6XcbQpa.so.6.5.0" "libQt6XcbQpa.so.6.5.0"
ln -s "./libQt6XcbQpa.so.6.5.0" "./libQt6XcbQpa.so"
ln -s "./libQt6XcbQpa.so.6.5.0" "./libQt6XcbQpa.so.6"

cp -a "${QT_PATH}/qml/QtCore/" "."
cp -a "${QT_PATH}/qml/QtQml/" "."
cp -a "${QT_PATH}/qml/QtQuick/" "."

chrpath -r \$ORIGIN/.. ./platforms/libqxcb.so
chrpath -r \$ORIGIN/.. ./platforms/libqminimal.so
chrpath -r \$ORIGIN/../.. ./QtQuick/Controls/libqtquickcontrols2plugin.so
chrpath -r \$ORIGIN/../.. ./QtQuick/Templates/libqtquicktemplates2plugin.so

cp "${PROJECT_DIR}/README.md" "./README.md"
cp "${PROJECT_DIR}/LICENSE" "./LICENSE"
cp "${PROJECT_DIR}/Qt License" "./Qt License"

echo "Copying files for installer..."
mkdir -p "${PROJECT_DIR}/installer/linux/packages/app.dialogue/data/"
cp -a * "${PROJECT_DIR}/installer/linux/packages/app.dialogue/data/"

echo "Packaging portable archive..."
cd ..
tar czvf dialogue_${DIALOGUE_VERSION}_linux_x86_64_portable.tar.gz Dialogue

echo "Creating installer..."
cd "${PROJECT_DIR}/installer/linux/"
binarycreator --offline-only -c config/config.xml -p packages dialogue_${DIALOGUE_VERSION}_linux_x86_64_installer

echo "Done!"

exit 0
