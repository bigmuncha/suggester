#include <iostream>


#include "server/primeserv.h"

int main(int argc, char *argv[]) {

    PrimeServ serv(2000);
    serv.quickstart();
    serv.accept();

    return 0;
}
