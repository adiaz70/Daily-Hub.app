## Building wxWidgets Library from Source Code

Tested on a freshly installed Ubuntu 20.04.2 64-bit virtual machine. (If this truly is a brand-new machine, run the Software Updater first.)

Install some basic tools by running the following commands in the terminal:
```
sudo apt-get install build-essential     
sudo apt install libgtk-3-dev
```
Download wxWidgets source code, version 3.0.5: https://www.wxwidgets.org/downloads/

Place source code directory anywhere convenient, such as the desktop, and open it in the terminal. Execute the following commands:
```
mkdir build_gtk
cd build_gtk
../configure --with-gtk=3 --enable-debug
make
sudo make install
sudo ldconfig
make clean
```
The library and header files should now be at /usr/local/lib and /usr/local/include. If the header files aren’t able to be found when compiling code, navigate to /usr/local/include and run:
```
sudo ln -sv [name of wx header directory]/wx wx
```

## Installing SQLite3

Download `sqlite-autoconf-3350400.tar.gz` on the page: https://www.sqlite.org/download.html

Extract the contents somewhere on the computer, such as the desktop, and open the directory in the terminal. Execute the following commands:
```
./configure --prefix=/usr/local
make
make install
```

## Compilation and Linker Flags for Prerequisites

Any code that uses wxWidgets needs the following code added somewhere in the terminal command to compile and link it:
```
`wx-config --cxxflags --libs`
```
Note that the back quote \` is a required part of the syntax and that it is not the same as a single quote ‘

Any code using SQLite3 needs the following flag in the terminal command to link it:
```
-lsqlite3
```
