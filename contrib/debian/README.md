
Debian
====================
This directory contains files used to package adond/adon-qt
for Debian-based Linux systems. If you compile adond/adon-qt yourself, there are some useful files here.

## adon: URI support ##


adon-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install adon-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your adon-qt binary to `/usr/bin`
and the `../../share/pixmaps/adon128.png` to `/usr/share/pixmaps`

adon-qt.protocol (KDE)

