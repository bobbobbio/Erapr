#!/bin/bash
g++ -x c++ -m32 -c main.cpp -o main.o
g++ -x c++ -m32 -c Sprite.cpp -o Sprite.o
g++ -x c++ -m32 -c Map.cpp -o Map.o
g++ -x c++ -m32 -c Enemy.cpp -o Enemy.o
g++ -m32 main.o Sprite.o Map.o Enemy.o -laldmd -ldumbd `allegro-config --libs` -o erapr

mkdir linux_build
mkdir ./linux_build/Contents
mkdir ./linux_build/Contents/LinuxOS
mkdir ./linux_build/Contents/Resources
mkdir ./linux_build/Contents/Resources/Maps

cp maindata.dat ./linux_build/Contents/Resources/
cp ./map\ editor/mapdata.dat ./linux_build/Contents/Resources/
cp ./maps/* ./linux_build/Contents/Resources/Maps/
cp erapr ./linux_build/Contents/LinuxOS/
