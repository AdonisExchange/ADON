ADON Core
=============

Setup
---------------------
[ADON Core](http://adon.adonis.exchange/wallet) is the original ADON client and it builds the backbone of the network. However, it downloads and stores the entire history of ADON transactions; depending on the speed of your computer and network connection, the synchronization process can take anywhere from a few hours to a day or more. Thankfully you only have to do this once.

Running
---------------------
The following are some helpful notes on how to run ADON Core on your native platform.

### Unix

Unpack the files into a directory and run:

- `bin/adon-qt` (GUI) or
- `bin/adond` (headless)

If this is the first time running ADON Core (since v5.0.0), you'll need to install the sapling params by running the included `install-params.sh` script, which copies the two params files to `$HOME/.adon-params`

### Windows

Unpack the files into a directory, and then run adon-qt.exe.

### macOS

Drag ADON-Qt to your applications folder, and then run ADON-Qt.

### Need Help?

* See the documentation at the [ADON Wiki](https://docs.adonis.exchange/)
for help and more information.
* Ask for help on the [ADON Forum](http://forum.adonis.exchange).
* Join our Discord server [Discord Server](https://discord.adonis.exchange)

Building
---------------------
The following are developer notes on how to build ADON Core on your native platform. They are not complete guides, but include notes on the necessary libraries, compile flags, etc.

- [Dependencies](dependencies.md)
- [macOS Build Notes](build-osx.md)
- [Unix Build Notes](build-unix.md)
- [Windows Build Notes](build-windows.md)
- [Gitian Building Guide](gitian-building.md)

Development
---------------------
The ADON repo's [root README](/README.md) contains relevant information on the development process and automated testing.

- [Developer Notes](developer-notes.md)
- [Multiwallet Qt Development](multiwallet-qt.md)
- [Release Notes](release-notes.md)
- [Release Process](release-process.md)
- [Source Code Documentation (External Link)](https://www.fuzzbawls.pw/adon/doxygen/)
- [Translation Process](translation_process.md)
- [Unit Tests](unit-tests.md)
- [Unauthenticated REST Interface](REST-interface.md)
- [Dnsseed Policy](dnsseed-policy.md)

### Resources
* Discuss on the [ADON](http://forum.adonis.exchange) forum.
* Join the [ADON Discord](https://discord.adonis.network).

### Miscellaneous
- [Files](files.md)
- [Fuzz-testing](fuzzing.md)
- [Reduce Memory](reduce-memory.md)
- [Tor Support](tor.md)
- [Init Scripts (systemd/upstart/openrc)](init.md)

License
---------------------
Distributed under the [MIT software license](/COPYING).
This product includes software developed by the OpenSSL Project for use in the [OpenSSL Toolkit](https://www.openssl.org/). This product includes
cryptographic software written by Eric Young ([eay@cryptsoft.com](mailto:eay@cryptsoft.com)), and UPnP software written by Thomas Bernard.
