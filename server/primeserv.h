#ifndef __PRIMESERV_H_
#define __PRIMESERV_H_

#include "server.h"
#include <fstream>
#include <iterator>
#include <algorithm>
#include <boost/regex.hpp>
#include <map>

class PrimeServ
    :public Server
{
    public:
        PrimeServ();
        void quickstart();
        void newWorker();
        std::string resultStr(std::ifstream&,std::string);
    private:
        //std::ifstream file;
        int str_count;

};
#endif // __PRIMESERV_H_
