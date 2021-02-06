#ifndef __PRIMESERV_H_
#define __PRIMESERV_H_

#include "server.h"
#include <fstream>

class PrimeServ
    :public Server
{
    public:
        PrimeServ();
        void newWorker();
        std::string resultStr(std::string);
    private:
        std::ifstream file;

};
#endif // __PRIMESERV_H_
