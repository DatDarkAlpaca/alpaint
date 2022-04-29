# Alpaint

A painting software made with Qt.

# Getting Started

These instructions are aimed towards getting you a copy of the project up and running on your local machine for development and testing purposes.

## Prerequisites

In order to build this project, you'll need to download the following dependencies and IDEs:

- [Qt](https://www.qt.io/download-qt-installer)
- [Visual Studio](https://visualstudio.microsoft.com/pt-br/downloads/)

Make sure to install the MSVC compiler when installing Qt.

## Installing

It is fairly easy to install the project, all you need to do is to 
[clone](https://github.com/DatDarkAlpaca/alpaint/) it from
GitHub. There is also the option to [download](https://github.com/DatDarkAlpaca/alpaint/archive/refs/heads/main.zip)
a copy of the repository.

If you want to clone the repository using the terminal instead, use:

    git clone https://github.com/DatDarkAlpaca/alpaint/

## Visual Studio Setup

Once everything is downloaded, open the solution `alpaint.sln`, click on `Extensions` and `Manage Extensions`. A dropdown list will show up. You'll need to install the [Qt Visual Studio Tools](https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools-19123) extension. You may need to restart Visual Studio.

Once installed, open the versions menu under `Extensions` > `Qt VS Tools` > `Qt Version`, then click on the plu sign, and select the `qmake.exe` path from your Qt installation. It can usually be found under `C:\Qt\Version\msvc...\bin\qmake.exe`.

## Building the project

To build the project, click `Build` then `Build Solution` on Visual Studio. Alternatively, you can use the shortcut `Ctrl+Shift+B`.
The executable files are generated under `bin/Configuration/Platform`, and the intermediate files are generated under `intermediate/Configuration/Platform`.

## License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT) - see the
[LICENSE](LICENSE) file for details
