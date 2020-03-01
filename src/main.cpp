#include "options.h"
#include "server.h"
#include <cassert>
#include <getopt.h>
#include <iostream>
#include <signal.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    signal(SIGHUP, SIG_IGN);
    auto daemon_res = daemon(0, 0);
    assert(daemon_res == 0);

    // getopt dummy
    int c;
    int digit_optind = 0;

    while (1) {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] = {
            { "add", 1, 0, 0 },
            { "append", 0, 0, 0 },
            { "delete", 1, 0, 0 },
            { "verbose", 0, 0, 0 },
            { "create", 1, 0, 'c' },
            { "file", 1, 0, 0 },
            { 0, 0, 0, 0 }
        };
        c = getopt_long(argc, argv, "abc:d:012", long_options, &option_index);
        break;
    }

    Options options(argc, argv);
    if (options.help_requested()) {
        std::cout << options.help();
        return 0;
    }

    Server server(options);

    for (;;) {
        std::this_thread::sleep_for(std::chrono::seconds(60));
    }

    return 0;
}
