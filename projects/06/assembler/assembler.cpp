#include "parser.h"

#include <iostream>

using namespace std;

void usage(const std::string &p)
{
    cout << "./" << p << " program.asm > program.hack" << endl;
}

int main(int argc, char *argv[])
{
    string line;

    if (argc != 2) {
        usage(argv[0]);
        return 1;
    }

    ifstream hack(argv[1]);
    parser p(hack);
    p.get_symbols();
    p.parse();

    return 0;
}
