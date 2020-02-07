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
//#include "rand_char_streambuf.h"
//#include "functional_streambuf"

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

#if 0
TEST_F (HashFnTestFixture, BsdSumRandSeq) {
    RandCharStreambuf<char> rcs {4096};
    istream iss {&rcs};
    BsdSum csum {};
    csum.append(iss);
    cout << csum.to_string() << endl;
}
#endif
#if 0

// Test breaking a buffer across multiple append commands.
TEST_F (HashFnTestFixture, Test2) {
    istringstream iss_a { "\xff\xfe" };
    istringstream iss_b { "\xfd\xfc" };
    istringstream iss { "\xff\xfe\xfd\xfc" };

    BsdSum bs {};
    bs.append(iss);
    string iss_result { bs.to_string() };

    bs.clear();
    bs.append(iss_a);
    bs.append(iss_b);

    EXPECT_EQ(iss_result, bs.to_string());
}

TEST_F (HashFnTestFixture, sum_sysv_test) {
    SysVSum svs {};
    for_each(begin(check_data), end(check_data),
        [&svs](auto& cd){EXPECT_EQ(cd.sysv_sum, svs(cd.input));});
}

TEST_F (HashFnTestFixture, cksum_test) {
    CkSum cs {};
    for_each(begin(check_data), end(check_data),
        [&cs](auto& cd){EXPECT_EQ(cd.cksum_crc, cs(cd.input));});
}

//
// Test accumulating data buffers with psuedo-random data.
//
TEST_F (HashFnTestFixture, Test3) {
    //MiddleSqWeyl<1024> msw;
}
#if 0
    const size_t buf_sz {1024};
    const int num_reps {40};
    vector<char> vc(buf_sz);
    MiddleSqWeyl rng;
    BsdSum bs {};
    for (int i = 0; i < num_reps; i++) {
        generate(vc.begin(), vc.end(), [&rng](){return rng();});
        bs.append(vc.data(), vc.size());
    }
    EXPECT_EQ(string("45402    40"), bs.to_string());
}
#endif
#endif

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

