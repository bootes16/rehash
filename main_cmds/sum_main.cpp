//
// Front end command line processing. Mimic the behaviour of the
// Unix sum(1) command.
//
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>

#include "checksum.h"
#include "bsd_sum.h"
#include "sysv_sum.h"

using namespace std;

// The sum(1) behaviour is such that a '-' is printed for stdin if and only if
// there are also named files for processing, otherwise for stdin only, no filename
// is printed.
void process_files(CheckSum&& cs, vector<string>& files) {
    if (1 == files.size() && 0 == files[0].compare("-")) files.clear();

    if (0 == files.size()) {
        cs.clear();
        cs.append(cin);
        return;
    }

    for (auto& fname : files) {
        cs.clear();

        if (0 == fname.compare("-")) {
            cs.append(cin);
        } else {
            ifstream ifs {fname, ifstream::in | ifstream::binary};
            cs.append(ifs);
        }
        cout << cs << " " << fname << endl;
    }

    return;
}

//
// Usage text.
//
void usage(void) {
    cout <<
        "Usage: sum [OPTION]... [FILE]...\n"
        "Print checksum and block counts for each FILE.\n\n"
        "With no FILE, or when FILE is -, read standard input.\n"
        "  -r              use BSD sum algorithm, use 1K blocks\n"
        "  -s, --sysv      use System V sum algorithm, use 512 bytes blocks\n"
        "      --help     display this help and exit\n"
        "      --version  output version information and exit\n\n"
        "Options mimic GNU coreutils sum. See:\n"
        "GNU coreutils online help: <http://www.gnu.org/software/coreutils/>\n"
        "Full documentation at: <http://www.gnu.org/software/coreutils/sum>\n"
    << endl;
}

//
// Show version.
//
void version(void) {
    cout <<
        "sum 0.1\n"
    << endl;
}

//
// Respond to invalid arguement.
//
void invalid(string s) {
    cout << "sum: invalid option '" << s << "'" << endl;
    cout << "Try 'sum --help' for more information." << endl;
}

//
// Parse args and process files or stdin.
//
int main(int argc, char *argv[]) {
    bool bsd_algo = true;
    vector<string> files;

    for (int i = 1; i < argc; i++) {
        string s {argv[i]};
        if (0 == s.compare("-r")) {
            bsd_algo = true;
        } else if (0 == s.compare("-s") || 0 == s.compare("--sysv")) {
            bsd_algo = false;
        } else if (0 == s.compare("--help")) {
            usage();
            return 0;
        } else if (0 == s.compare("--version")) {
            version;
            return 0;
        } else if (0 == s.compare("-")) {
            files.push_back(s);
        } else if (0 == s.find('-')) {
            invalid(s);
            return 1;
        } else {
            files.push_back(s);
        }
    }

    if (bsd_algo) process_files(move(BsdSum{}), files);
    else process_files(move(SysVSum{}), files);
}
