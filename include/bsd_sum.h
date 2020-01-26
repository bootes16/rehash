#ifndef BSD_SUM_H
#define BSD_SUM_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>

#include "checksum.h"

class BsdSum : public CheckSum {
public:
    virtual void append(std::istream& istr) override;
    using CheckSum::append; //(const std::string& str);

    virtual void clear(void) { CheckSum::clear(); checksum = 0; };

    virtual std::string hexdigest(void) const override;
    virtual std::string to_string(void) const override;

private:
    uint32_t checksum {0};
};

#endif /* BSD_SUM_H */