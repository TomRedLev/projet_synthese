#!/bin/bash
echo '-----emplacement des libGL*.so-----'
find /usr/lib/ -name libGL.so;
find /usr/lib/ -name libGLU.so;
find /usr/lib/ -name libglut.so;
echo '-----emplacement des include gl*.h-----'
find /usr/include/ -name gl.h;
find /usr/include/ -name glu.h;
find /usr/include/ -name glut.h;
echo '-----glxinfo------'
glxinfo | grep 'version'
echo '-----glxgears-----'
glxgears
echo ''
