ADON Core integration/staging repository
=====================================

[![GitHub release (latest by date)](https://img.shields.io/github/v/release/AdonisExchange/ADON?color=%23002144&cacheSeconds=60)](https://github.com/AdonisExchange/ADON/releases)
[![GitHub Release Date](https://img.shields.io/github/release-date/AdonisExchange/ADON?color=%23002144&cacheSeconds=3600)](https://github.com/AdonisExchange/ADON/releases)

## What is ADON VIP?

ADONIS VIP Coin (ADON) is a decentralized, MIT-licensed open-source, fair-launch blockchain/cryptocurrency project managed, developed, governed, and stewarded by a community-driven decentralized autonomous organization (DAO). It has been designed, engineered, and tested using advanced cryptography protocols to provide, first and foremost, user Financial Data Protection.

### Main aspects:

1. User Data Protection: Through the use of SHIELD, a zk-SNARKs based privacy protocol.
2. Low environmental footprint and network participation equality: Through the use of a highly developed Proof of Stake protocol.
3. Decentralized Governance System: A DAO built on top of the tier two Masternodes network, enabling a monthly community treasury, proposals submission and decentralized voting.
4. Fast Transactions: Through the use of fast block times and the tier two network, ADON is committed to continue researching new and better instant transactions mechanisms.
5. Ease of Use: ADON is determined to offer the best possible graphical interface for a core node/wallet. A full featured graphical product for new and advanced users.

A lot more information and specs at [adon.adonis.exchange](https://adon.adonis.exchange/). Join the community at [ADON Telegram]( https://t.me/AdonisExchange).

## License
ADON Core is released under the terms of the MIT license. See [COPYING](https://github.com/AdonisExchange/ADON/blob/master/COPYING) for more information or see https://opensource.org/licenses/MIT.

## Development Process

The master branch is regularly built (see doc/build-*.md for instructions) and tested, but it is not guaranteed to be completely stable. [Tags](https://github.com/AdonisExchange/ADON/tags) are created regularly from release branches to indicate new official, stable release versions of ADON Core.

The contribution workflow is described in [CONTRIBUTING.md](https://github.com/AdonisExchange/ADON/blob/master/CONTRIBUTING.md) and useful hints for developers can be found in [doc/developer-notes.md](https://github.com/AdonisExchange/ADON/blob/master/doc/developer-notes.md).

## Testing

Testing and code review is the bottleneck for development; we get more pull requests than we can review and test on short notice. Please be patient and help out by testing other people's pull requests, and remember this is a security-critical project where any mistake might cost people a lot of money.

## Automated Testing

Developers are strongly encouraged to write [unit tests](https://github.com/AdonisExchange/ADON/blob/master/src/test/README.md) for new code, and to submit new unit tests for old code. Unit tests can be compiled and run (assuming they weren't disabled in configure) with: make check. Further details on running and extending unit tests can be found in [/src/test/README.md](https://github.com/AdonisExchange/ADON/blob/master/src/test/README.md).

There are also regression and integration tests, written in Python. These tests can be run (if the test dependencies are installed) with: test/functional/test_runner.py`

The CI (Continuous Integration) systems make sure that every pull request is built for Windows, Linux, and macOS, and that unit/sanity tests are run automatically.

## Manual Quality Assurance (QA) Testing

Changes should be tested by somebody other than the developer who wrote the code. This is especially important for large or high-risk changes. It is useful to add a test plan to the pull request description if testing the changes is not straightforward.

