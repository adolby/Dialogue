#Dialogue

Dialogue is an open-source peer-to-peer chat app. Dialogue doesn't keep user name information or logs. It is currently a work in progress. I can't recommend using it until secure connections (SSL) are supported, which is the next anticipated feature.

[![Build Status](https://travis-ci.org/adolby/Dialogue.svg?branch=master)](https://travis-ci.org/adolby/Dialogue)

##Getting Started

###Installation
To install Dialogue on Windows or Linux, run the installer executable or extract the files from the Dialogue archive. To install Dialogue on macOS, extract the disk image archive, then mount the disk image. Drag Dialogue.app to your Applications folder. Once the install files are copied, run the Dialogue executable.

###Solving Windows missing DLL error
If you receive an error indicating a missing msvcp140.dll, vcruntime140.dll, or other DLL files when you run Dialogue, you'll need to install the Visual C++ 2015 Redistributable (x64).

The Visual C++ Redistributable installer is located where you installed Dialogue (default directory for the installer is C:\Program Files\Dialogue\). Its file name is vcredist_x64.exe.

Run the Visual C++ Redistributable installer, and you should then be able to start Dialogue.

###Using Dialogue
Enter the IP address of the person you want to chat with, then wait for the Connection Status to change to Connected. Next, select the Conversation button to go to the conversation page. Enter text into the message field and then select the Send button to send your message.

##Technical Commentary

Unlike traditional peer-to-peer networks, Dialogue does not use a central server to route connections. This allows for a direct connection between peers. To initiate the connection, the app automatically cycles between connecting as client and server. This has some advantages and disadvantages:

Pros:
- Automatic connection allows user to not have to worry about downloading two apps or coordinating connection modes
- Connection is acquired automatically

Cons:
- If the connection process is started at the same time on both peers, it is possible that the peers will never connect. This currently requires a user to restart the automatic connection process by changing IP address or ports.

##Licenses

Dialogue is licensed under the BSD-3 Clause License. Read the LICENSE file or go to https://opensource.org/licenses/BSD-3-Clause for more information about the license.

Some code is adapted from Qt Examples code, also licensed under the BSD-3 Clause License.

Qt is licensed under the GNU Lesser General Public License version 2.1. Read the Qt License file or go to https://www.gnu.org/licenses/lgpl-2.1.html for more information about the license.
Qt source code is available at https://code.qt.io.

##Developers

If you'd like to contribute to Dialogue, you can fork the project on GitHub and submit a pull request. To build Dialogue, you'll need Qt 5.6 (or later) and a C++14 capable compiler.

##Contact

You can contact the project creator via email at andrewdolby@gmail.com.
