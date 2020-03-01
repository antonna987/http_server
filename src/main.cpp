#include "options.h"
#include "server.h"
#include <cassert>
#include <iostream>
#include <signal.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    signal(SIGHUP, SIG_IGN);
    auto daemon_res = daemon(0, 0);
    assert(daemon_res == 0);

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
