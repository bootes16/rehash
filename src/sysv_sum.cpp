#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

#include "sysv_sum.h"

// Return a finalised SysV checksum.
static uint16_t finalise(uint32_t s) { 
    uint32_t r = (s & 0xffff) + ((s & 0xffffffff) >> 16);
    return static_cast<uint16_t>((r & 0xffff) + (r >> 16));
}

void SysVSum::append(std::istream& istr) {
    char c;
    while (istr.get(c)) {
        checksum += static_cast<unsigned char>(c);
        byte_count++;
    }
}

//
// System V checksum output: <checksum> <blocks>
// separated by spaces where:
//  <checksum> - non-zero padded, non-aligned decimal 16bit checksum value.
//  <blocks> - number of 512byte blocks
//
std::string SysVSum::to_string(void) const {
    std::ostringstream oss;
    oss << finalise(checksum) << " " << num_blocks(512);
    return oss.str();
}