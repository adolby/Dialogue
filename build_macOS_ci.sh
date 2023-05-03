#!/bin/bash

set -o errexit -o nounset

PROJECT_DIR="${SOURCE_DIR}"
QT_PATH="${Qt6_DIR}"
DIALOGUE_VERSION="${DIALOGUE_VERSION:-dev}"

# Output macOS version
sw_vers

# Update platform
echo "Updating platform..."
brew update
brew install p7zip
# npm install -g appdmeg

# Add Qt binaries to path
echo "Adding Qt binaries to path..."
PATH="${QT_PATH}/bin/:${PATH}"

# Check qmake version
echo "Check qmake version..."
qmake --version

# Build Dialogue
echo "Building Dialogue..."
cd "${PROJECT_DIR}"

cd ${project_dir}
qmake -makefile CONFIG+=release
make

# Package Dialogue
echo "Packaging..."
cd "${PROJECT_DIR}/build/macOS/clang/x86_64/release/"

rm -rf moc
rm -rf obj
rm -rf qrc

echo "Creating dmg archive..."
macdeployqt ./Dialogue.app -qmldir=../../../../../src -dmg
mv ./Dialogue.dmg "./Dialogue_${DIALOGUE_VERSION}.dmg"

cp -a "${PROJECT_DIR}/README.md" "./README.md"
cp -a "${PROJECT_DIR}/LICENSE" "./LICENSE"
cp -a "${PROJECT_DIR}/Qt License" "./Qt License"

echo "Packaging archive..."
tar czvf "dialogue_${DIALOGUE_VERSION}_macos.tar.gz" "Dialogue_${DIALOGUE_VERSION}.dmg" "README.md" "LICENSE" "Qt License"

echo "Done!"

exit 0
