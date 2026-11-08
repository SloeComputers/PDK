# PDK

PDK is a Portable Development Kit for C++ host and embedded applications. It is intended to be used as a git submodule inside application projects.

## Status

[![Build Status](https://github.com/SloeComputers/PDK/actions/workflows/build_on_push.yml/badge.svg)](https://github.com/SloeComputers/PDK/actions/workflows/build_on_push.yml)

## Libraries

Embedded application support libraries

|Library|Purpose|
|-------|-------|
|[TGT](TGT)|Target support: toolchains, vector tables, linker scripts, loadable image generation, and build macros.|
|[UCL](UCL)|Small standard C library subset for embedded builds.|
|[MTL](MTL)|Bare metal support for microcontrollers: MCU bringup, clocks, UARTs, timers, I2C, SPI etc.|
|[TNY](TNY)|Additional standard C library support dependent on `MTL` for I/O.|
|[HWR](HWR)|Hardware configuration layer for reusable board/product APIs and native simulation variants.|

General purpose libraries for host and embedded applications

|Library|Purpose|
|-------|-------|
|STB|Static data structures and utility types for limited-memory code.|
|USB|USB descriptors and support utilities.|
|SIG|Signal processing and synthesis.|
|MIDI|MIDI protocol support.|
|GUI|Simple GUI and 2D rendering.|
|TRM|Lightweight terminal/curses-style interface.|

Host OS abstraction libraries

|Library|Purpose|
|-------|-------|
|PLT|Platform abstraction for display, MIDI, audio etc.|

## Starting a new project using the PDK

Create a new **git** project directory and add the **PDK** as a git submodule.

```
mkdir MyProject
cd MyProject
git init .
git submodule add https://github.com/SloeComputers/PDK.git
cd PDK
make template
```

The project template installer should run..

```
Project dir [/Users/fred/MyProjects] : <project root directory>
Copyright owner [John D. Haughton] : <copyright owner>
Copyright year [2017] : <copyright year>
User name [SloeComputers] : <GitHub user id>
Application [MyProject] : <name of application>
Version [0.0.0] : <initial version>
Description [Example application] : <application purpose>
Binary [foo] : <filename of application binary>

Install 'CMakeLists.txt' to '<project root directory>'
Install 'LICENSE' to '<project root directory>'
Install 'Makefile' to '<project root directory>'
Install '<filename of application binary>.cpp' to '<project root directory>/Source'
Install 'CMakeLists.txt' to '<project root directory>/Source'
Install 'README.md' to '<project root directory>'
Install '.gitignore' to '<project root directory>'
Install '_config.yml' to '<project root directory>'
Install '.clang-format' to '<project root directory>'
Install 'build_on_push.yml' to '<project root dir>/.github/workflows'
```

Add the newly instantiated project as an initial git patch then build.

```
cd ..
git add .
git commit -m Initial
make
```

Run the example application just built. (Try with -v and -h too)

```
build/native/Source/myproject
Hello, world!
```

The following small projects are using the PDK...
* [MineSweeper](https://github.com/SloeComputers/MineSweeper/)
* [Sternhalma](https://github.com/SloeComputers/Sternhalma/)
* [Zif](https://github.com/SloeComputers/Zif/)

### Linux

One of the primary target platforms.

Depend on SDL3, so a development install of SDL3 is required.

Typically the following packages will be required...

```
sudo apt-get install cmake
sudo apt-get install libsdl3-dev
```

### macOS

One of the primary target platforms.

Depend on SDL3, so a development install of SDL3 is required.

Typically the following packages will be required...

```
brew update
brew install cmake
brew install sdl3
```

### Kindle3

Although this is also a Linux build, it does not depend on SDL.

Requires gcc/g++ built for arm-linux-gnueabihf targets that supports C++11 and a set of
headers and static runtime libraries that are compatible with the Linux kernel v2.6.26
installed on the Kindle3.

#### Using a Raspberry Pi as a build machine for Kindle3

The original ARMv6 Raspberry Pi running a Debian based Linux has been found to be suitable
platform to build for Kindle3. But this is far easier if an old version of Rasbian (Wheezy)
is used to ensure that libc is compatible with the old kernel on the Kindle.

Other packages required...

```
sudo apt-get install cmake
sudo apt-get install g++-4.7
```

### Browser (via Emscripten)

Uses the SDL library supplied with Emscripten. (CURRENT STATUS UNCERTAIN)

### iOS

Requires access to a source installation of SDL which is built into a static lib as part of
the application build process.

Build of an actual app has not been achieved yet, and may not be easily achievable without
an Xcode project file.

### Android

Requires an NDK installation. Currently builds as a console only app as an integration with
an Android build of SDL has not been achieved yet.

## Coding style

The source is C++ but has the following non-typical for modern C++ features ...

* Memory is mostly statically or stack allocated in preference to new/delete
* Very limited STL usage
* Use of C style stdio API
* In some places have re-invented the wheel avoiding functionality that is in standard libraries
* Refer to [STYLE.md](STYLE.md) and the repository `.clang-format` for specific naming and formatting preferences.

