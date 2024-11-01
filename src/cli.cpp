#include <iostream>
#include <vector>
#include <string>

#include "gzip/gzip.hpp"

#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

void handler(int sig)
{
    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

int main(int argc, char **argv)
{
    signal(SIGSEGV, handler);
    if (argc < 2)
    {
        cerr << "Invalid filepath\n";
        return -1;
    }

    GZip gzip;
    gzip.dump(argv[1]);
    // cout << ".";
    // Image image("foo"s);
    // vector<string> msg{"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};

    // for (const string &word : msg)
    // {
    //     cout << word << " " << image.GetFilename();
    // }
    // cout << endl;
}