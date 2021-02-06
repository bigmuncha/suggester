#ifndef __PRIMESERV_H_
#define __PRIMESERV_H_

#include "server.h"
#include <fstream>
#include <iterator>
#include <algorithm>

class PrimeServ
    :public Server
{
    public:
        PrimeServ();
        void quickstart();
        void newWorker();
        std::string resultStr(std::string);
    private:
        std::ifstream file;
        int str_count;

};
#endif // __PRIMESERV_H_
