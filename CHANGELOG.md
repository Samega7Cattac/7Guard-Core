# 7Guard Change Log

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/) and this project adheres to [Semantic Versioning](http://semver.org/).

## [v2.0] - Unreleased
### Added
- Makefile config file
- Doxygen config file
- MacOS compatibility TODO
- Support for folders TODO

### Fixed
- Not outputing bug fixed
- Copyrights fixed
- Discord tag updated
- Huge performance increase
- Windows compatibility TODO

### Changed
- arguments convertition updated
- Errors go to stderr buffer
- Documentation updated
- Files writing method

### Removed
- threads Implementation removed
- buffer implementation removed

## [v1.2] - 2018-12-11
### Added
- Documentation added.
- Numeric range verification added.
- Implementation of "-i" option.
- Implementation of processing using threads in crypt.
- feedback thread sleep time.
- Ascii Art added.

### Fixed
- Wrong crypt vars bug fix.
- Small files don't crypt bug fix.
- Fix compatibility with linux.
- Args verification buf fix.
- "Initializing" string buf fix.
- error codes fix.
- minor code fix.

### Changed
- More info to the credits.
- help updated.

### Removed
- using namespaces and useless defines removed.

### Know Bugs
- Sometimes get stuck on crypt using "--threads" option (Windows only). Pls don't use Windows, u will get cancer!

## [v1.1] - 2018-11-15
### Added
- mutiple args verification.
- missing values verification.

### Fixed
- "--buf" bug fix.
- if the file size is smaller than the buffer.
- lag issues with the "Initializing" string.

### Changed
- help update.

## [v1b] - 2018-11-08
- initial version.
