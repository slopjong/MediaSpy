README.txt
==========

MediaSpy requires at least Qt 4.5!


How to build MediaSpy:
=====================

With MinGW/Windows:

* Open a console, and go to the sources directory.
        * qmake && mingw32-make

With MSVC/Windows:

* Open a console, and go to the sources directory.
        * qmake && nmake // possible you will need to set, for wich compiler to generate makefile like : qmake -spec win32-msvc2005

With Linux/*Nix:

* Open a shell, and go to the sources directory.
        * qmake && make // some distros use qmake-qt4 for qmake of Qt 4

With Mac OS X:

* Open a terminal, and go to the sources directory.
        * qmake -spec macx-g++ && make


How to execute MediaSpy:
=======================
* Once you have build MediaSpy you need to install resources executing this command : $$MAKE install
  ( replace $$MAKE by make/mingw32-make/nmake according to your platform ).
  You may require admin acces when using $$MAKE install. For unix-like, simply use a sudo,
  for windows, you need to have admin access.


