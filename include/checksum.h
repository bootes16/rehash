#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <vector>
#include <string>
#include <istream>
#include <sstream>

class CheckSum {
public:
    CheckSum() = default;
    CheckSum(int block_sz) : block_sz{block_sz} {}
    
    // Append/update functions.
    // Accumulate data to the checksum state.
    virtual void append(std::istream& istr) = 0;
    virtual void append(const std::string& str) {
        std::istringstream iss {str};
        append(iss);
    }

    // Clear the checksum state.
    virtual void clear(void) { byte_count = 0; }

    // Checksum getter functions. May perform addional non-state changing
    // operations to evaluate and return. Must leave internal data in continuable
    // state.
    virtual std::string hexdigest(void) const = 0;
    virtual std::string to_string(void) const = 0;

    virtual size_t size(void) const { return byte_count; }
    virtual size_t num_blocks(void) const {
        return (byte_count / block_sz) + !!(byte_count % block_sz);
    }

    std::string operator()(const std::string& msg) {
        clear();
        append(msg);
        return to_string();
    }

    friend std::ostream& operator<<(std::ostream& out, const CheckSum& cs) {
        out << cs.to_string();
        return out;
    }

protected:
    size_t byte_count {0};
    int block_sz {1024};
};

#endif /* CHECKSUM_H */