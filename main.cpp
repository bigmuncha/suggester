#include <iostream>

#include "server/server.h"
#include "server/primeserv.h"

int main(int argc, char *argv[]) {

    PrimeServ serv;
    serv.quickstart();
    serv.accept();

    return 0;
}
