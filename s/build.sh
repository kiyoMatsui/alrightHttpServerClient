#!/bin/bash

mkdir -p build && cd build

cmake ..
# or cmake -DCMAKE_INSTALL_PREFIX:PATH=/home
make

# apt-file can find package from file ... run ~$ apt-file update and then ~$ apt-file search kwallet.h


#to install go to build directory and run:
#~$ sudo make install ..
#to uninstall software (note will not remove directories)
#~$ xargs sudo rm < install_manifest.txt


#for manual install use ~$ g++ -std=c++11 ../src/Fireworks.cpp -lsfml-window -lsfml-graphics -lsfml-audio -lsfml-system -lthor -I . -o test


#git -A && git commit -m "commit message here" && git push

#interesting qt5-base examples see the wizards and sql examples Also see standardDialogs for file io
