#ifndef SYSV_SUM_H
#define SYSV_SUM_H

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include "checksum.h"

//
// TODO: There is scope to extend the hierarchy with a class UnixSum:
// [ CheckSum] <-- [ UnixSum ] <-- [ SysVSum ]
//                              +- [ BsdSum ]
//
// UnixSum could implement num_blocks(), clean(), hexdigest(), value().
//
class SysVSum : public CheckSum {
public:
    virtual void append(std::istream& istr) override;
    using CheckSum::append; //(const std::string& str);

    // Clear the checksum state.
    virtual void clear(void) { CheckSum::clear(); checksum = 0; };

    virtual std::string hexdigest(void) const override { return std::string("");}
    virtual std::string to_string(void) const override;

    int num_blocks(size_t block_sz) const {
        return (byte_count / block_sz) + !!(byte_count % block_sz);
    }

private:
    uint32_t checksum {0};
};

#endif /* SYSV_SUM_H */