#include <iostream>

#include "server/server.h"


int main(int argc, char *argv[]) {

    Server serv;
    serv.quickstart();
    serv.accept();

    return 0;
}
