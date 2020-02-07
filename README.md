# rehash - Checksums and CRCs

## Overview
C++ library implementing some checksums and CRCs.

## sum(1) checksum command
Implemented both BSD and SysV algorithms.
Implemented main front end which apes the existing sum(1) command syntax.

## cksum(1) CRC32 command
Implemented CRC32 algorithm used by command.
Implemented main font end which aps the existing cksum(1) command syntax.

## Tests
Tests implemented with the Google GTest suite.
Set the environment variable GTEST_DIR to the path to the Google GTest directory or pass it on the command line to CMake:
    `cmake -DGTEST_DIR:STRING=<path>`
