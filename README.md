Monkey Log Viewer
========

The Monkey Log Viewer is viewer for log files developed with Qt 4.7.0. It is designed to find information easily from the plain text file. You can fully customize the keyword highlighting and color scheme. This feature possible to quickly find interested keyword.

##Screenshot
![screenshot#1](https://lh5.googleusercontent.com/-22li12ILdjE/UJT5hZ2P4bI/AAAAAAAAGe4/E5urhDr7dNo/s800/monkeylogviewer-20121103.png)

##Demo Video
 * http://www.youtube.com/watch?v=YY23R3RmRz0

##Features
 * Cross-platform. Runs on Linux, Windows, OS/2
 * Colorize log line
 * Export as HTML
 * Printing support
 * Multi-tab interface
 * Regular expression
 * Keyword search
 * Keyword statistics - create a chart base on total number of keywords in the log file
 * Zoom in / out
 * Drag & drop support
 * File browser
 * Unicode support
 * Recent files support
 * Jump to line
 * Line numbers
 * Update support - only available in Windows

##Downloads
 The latest stable version of Monkey Log Viewer is 1.1.0
 * Windows Installer - <a href="http://monkeylogviewer.googlecode.com/files/monkeyLogViewer-1.1.0.exe">monkeyLogViewer-1.1.0.exe</a> 
 * Windows Portable - <a href="http://monkeylogviewer.googlecode.com/files/monkeyLogViewer-1.1.0.zip">monkeyLogViewer-1.1.0.zip</a> 

##Dependency 
 * Qt version 4.7 or higher (It yout want to build source code)

##How to build
###On the command line
 * Setp 1: Set environment variables in your ~/.profie 

 ```
 export QTDIR=/Your/Qt/Directory
 ```
 * Step 2: Create a platform specific Makefile

 ```
 cd monkeylogviewer
 qmake
 ``` 
 * Step 3: Compile
 ```
 make 
 ```

###Using Qt Creator
 * Setp 1: Open a project
  * Select "File > Open File or Project"
  * Select the monkeyLogviewer.pro to open 
 * Step 2: Compile
  * "CTRL + B"

###Win7/msvc2008
 * Add below code to monkeyLogViewer.pro

 ```
 win32:LIBS += shell32.lib
 ```