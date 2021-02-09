#include <iostream>


#include "server/primeserv.h"
#include <chrono>
int main(int argc, char *argv[]) {

    PrimeServ serv;
    //serv.setMaxCacheSize(3000);
    //serv.setTTL(10);
    serv.quickstart();
    serv.run_cache_collector();
    serv.accept();
    //serv.run_cache_collector();
    return 0;
}
