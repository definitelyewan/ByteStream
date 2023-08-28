# ByteStream
ByteStream is a stream library entirely written in C with a specific emphasis on different text encodings such as different unicode standards and ASCII. It only requires the standard C library and works on both Linux and Windows while compiled with either GCC or Clang.

ByteStream Provides an easy way to initialize a stream from a file or directly by the caller.
From this point, the caller can easily read and write to the stream from a supplied position or
the internal position. This library also provides functions for converting different unicode standards
and other text standards such as ASCII and Latin1. This library has been optimized to reduce the runtime
of its functions and the number of CPU cycles they require. However, these optimizations have only been
done on x86 platforms such as Intel and AMD. 

### The ByteStream library provides:

 - Creating a stream from a file or supplied data
 - Reading and writing from a stream
 - Coverage of UTF8, UTF16LE, UTF16BE, Latin1, and Ascii
 - Conversion between all supported encoding standards
 - Support for reading integers and SafeSync integers
 - 100% test coverage
 - 100% benchmark coverage

All documentation, tests, benchmarks, builds, and installers can be generated using the provided python3 scripts located in scripts/