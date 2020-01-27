//
// Front end command line processing. Mimic the behaviour of the
// Unix cksum(1) command.
//
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>

#include "checksum.h"
#include "cksum.h"

using namespace std;

// cksum(1) always display's a '-' for the filename if '-' is specified on the command line.
void process_files(CheckSum&& cs, vector<string>& files) {
    if (0 == files.size()) {
        cs.clear();
        cs.append(cin);
        cout << cs << endl;
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
            cout << "Usage:" << endl;
            return 1;
        } else if (0 == s.compare("--version")) {
            cout << "Version 0.1" << endl;
        } else {
            files.push_back(string(argv[i]));
        }
    }


    process_files(move(CkSum{}), files);
}
