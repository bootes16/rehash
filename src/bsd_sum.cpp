#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

#include "bsd_sum.h"

void BsdSum::append(std::istream& istr) {
    char c;
    while (istr.get(c)) {
        checksum = (checksum >> 1) + ((checksum & 1) << 15);
        checksum += static_cast<unsigned char>(c);
        checksum &= 0xffff;       /* Keep it within bounds. */

        byte_count++;
    }
}

std::string BsdSum::hexdigest(void) const {
    char buf[8];
    sprintf(buf, "%05x", checksum);
    return std::string(buf);
}

//
// System V checksum output: <checksum> <blocks>
// separated by spaces where:
//  <checksum> - non-zero padded, non-aligned decimal 16bit checksum value.
//  <blocks> - number of 512byte blocks
//
std::string BsdSum::to_string(void) const {
    std::ostringstream oss;
    oss << std::setw(5) << std::setfill('0') << checksum <<
        " " << std::setfill(' ') << std::setw(5) << num_blocks();
    return oss.str();
}
