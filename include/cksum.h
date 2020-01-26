#include <iostream>
#include <vector>
#include <string>
#include "checksum.h"

class CkSum : public CheckSum {
public:
    virtual void append(std::istream& istr) override;
    using CheckSum::append; //(const std::string& str);

    virtual void clear(void) { CheckSum::clear(); checksum = 0; };

    virtual std::string hexdigest(void) const override;
    virtual std::string to_string(void) const override;

    friend std::ostream& operator<<(std::ostream& out, const CkSum& cs) {
        out << cs.hexdigest() << "\t" << cs.size() << std::endl;
        return out;
    }

private:
    uint32_t get_value(void) const;

    uint32_t checksum {0};
};
