#!/bin/bash

set -o errexit -o nounset

project_dir=$(pwd)

# Output macOS version
sw_vers

# Update platform
echo "Updating platform..."
brew update
brew install p7zip
npm install -g appdmg

# Get Qt
echo "Installing Qt..."
cd /usr/local/
sudo wget https://github.com/adolby/qt-more-builds/releases/download/5.7/qt-opensource-5.7.0-x86_64-macos-clang.zip
sudo 7z x qt-opensource-5.7.0-x86_64-macos-clang.zip &>/dev/null
sudo chmod -R +x /usr/local/Qt-5.7.0/bin/

# Add Qt binaries to path
PATH=/usr/local/Qt-5.7.0/bin/:${PATH}

# Create temporary symlink for Xcode8 compatibility
cd /Applications/Xcode.app/Contents/Developer/usr/bin/
sudo ln -s xcodebuild xcrun

# Build Dialogue
echo "Building Dialogue..."
cd ${project_dir}
qmake -config release
make

# Package Dialogue
echo "Packaging..."
cd ${project_dir}/build/macOS/clang/x86_64/release/

rm -rf moc
rm -rf obj
rm -rf qrc

echo "Creating dmg archive..."
macdeployqt Dialogue.app -qmldir ../../../../../src -dmg
mv Dialogue.dmg "Dialogue_${TAG_NAME}.dmg"
# appdmg json-path Dialogue_${TRAVIS_TAG}.dmg

cp "${project_dir}/README.md" "README.md"
cp "${project_dir}/LICENSE" "LICENSE"
cp "${project_dir}/Qt License" "Qt License"

echo "Packaging zip archive..."
7z a dialogue_${TAG_NAME}_macos.zip "Dialogue_${TAG_NAME}.dmg" "README.md" "LICENSE" "Qt License"

echo "Done!"

exit 0
