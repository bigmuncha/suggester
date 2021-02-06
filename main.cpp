#include <iostream>

#include "server.h"


int main(int argc, char *argv[]) {

    Server serv;
    serv.start();
    serv.accept();

    return 0;
}
