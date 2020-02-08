#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

#include "gtest/gtest.h"
#include "bsd_sum.h"
#include "sysv_sum.h"
#include "cksum.h"
//#include "middle_sq_weyl.h"
#include "functional_streambuf.h"

using namespace std;

struct CheckData {
    string      input;
    string      bsd_sum;
    string      sysv_sum;
    string      cksum_crc;
};

class HashFnTestFixture: public ::testing::Test {
protected:
    HashFnTestFixture() {}
};

static const CheckData check_data[] = {
    {
        "Hello World!",
        "02760     1",
        "1085 1",
        "2817829334 12"
    },
    // Zero length string.
    {
        "",
        "00000     0",
        "0 0",
        "4294967295 0"
    },
    // Another zero length string.
    {
        "\0",
        "00000     0",
        "0 0",
        "4294967295 0"
    },
    {
        "\xff",
        "00255     1",
        "255 1",
        "3045181057 1"
    },
    {
        "\xff\xfe\xfd\xfc",
        "57817     1",
        "1014 1",
        "115125973 4"
    }
};

static CheckSum* checksums[] = {
    new BsdSum{},
    new SysVSum{},
    new CkSum{}
};

//
// Test each checksum agianst some known, simple, short test data.
//
TEST_F (HashFnTestFixture, BsdSumTest) {
    BsdSum csum {};
    for_each(begin(check_data), end(check_data), [&csum](auto& cd){
        EXPECT_EQ(cd.bsd_sum, csum(cd.input));
    });
}

TEST_F (HashFnTestFixture, SysVSumTest) {
    SysVSum csum {};
    for_each(begin(check_data), end(check_data), [&csum](auto& cd){
        EXPECT_EQ(cd.sysv_sum, csum(cd.input));
    });
}

TEST_F (HashFnTestFixture, CkSumTest) {
    CkSum csum {};
    for_each(begin(check_data), end(check_data), [&csum](auto& cd){
        EXPECT_EQ(cd.cksum_crc, csum(cd.input));
    });
}

TEST_F (HashFnTestFixture, RandStreamTest) {
    functional_streambuf fsbuf {
        4096,
        []()->int {static int a = 0; a+=3; a %= 256; return a;}
    };
    istream iss {&fsbuf};
    BsdSum csum {};
    csum.append(iss);
    cout << csum.to_string() << endl;
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

