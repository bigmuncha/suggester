#include <iostream>


#include "server/primeserv.h"

int main(int argc, char *argv[]) {

    PrimeServ serv(50000);
    serv.quickstart();
    serv.accept();

    return 0;
}
