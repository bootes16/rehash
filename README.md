# rehash - Checksums and CRCs

## Overview
C++ library implementing some checksums and CRCs.

## sum(1) checksum command
Implemented both BSD and SysV algorithms.
Implemented main front end which apes the existing sum(1) command syntax.

## cksum(1) CRC32 command
Implemented CRC32 algorithm used by command.
Implemented main font end which aps the existing cksum(1) command syntax.

## Testing
Tests implemented with the Google GTest suite.
Set the environment variable GTEST_DIR to the path to the Google GTest directory or pass it on the command line to CMake:
    `cmake -DGTEST_DIR:STRING=<path>`

### Submodules
Submodules `functional_streambuf` and `simple_prng` are used to provide input for the GTest unittest. `simple_prng` is used to provide a simple random sequence of characters (not cryptographically secure.) `functional_streambuf` extends the STL `streambuf` class with a subclass which takes a function object which returns randome characters. This is then used as a source to the istream instance required by the checksum interface.

`simple_prng` module also builds the `prng` executable which produces the same random character sequence as is used in the unittests. The purpose of this is to generate a the same random stream to use as intput into the reference implementations of the checksum and CRC algorithms to generate reference results for the test cases.