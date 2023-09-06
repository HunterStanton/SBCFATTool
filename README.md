# SBCFATTool

A C++ tool for unpacking SteamBot Chronicles FAT files. It is capable of unpacking FAT files from both the release version of the game as well as the prerelease builds.

This tool currently does not repack FAT files. Support for this is planned and will come soon.

## Usage

`SBCFATTool unpack <filename.fat> <filename.dat> <output folder>`

This will unpack the FAT file into the output folder. You *must* supply the proper DAT file for the FAT file otherwise the files unpacked will either be completely wrong or the application will outright crash.

## Building

You should be able to compile this tool by just running `make`. It does not rely on any 3rd-party libraries and should compile on any platform with a C++17 compiler.

It should be noted that if this is compiled on a big endian platform (such as PowerPC), it may not work correctly. This will be addressed in the future.
