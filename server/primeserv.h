#ifndef __PRIMESERV_H_
#define __PRIMESERV_H_

#include "server.h"
#include <fstream>
#include <iterator>
#include <algorithm>
#include <boost/regex.hpp>
#include <map>
#include <unordered_map>
#include "../cache/self_cache.h"

class PrimeServ
    :public Server
{
    public:
        PrimeServ();
        PrimeServ(int/*add Cache size*/);
        void quickstart();
        void newWorker();
        std::string resultStr(std::ifstream&,std::string);
    private:
        MyCache Cache;
        int str_count;

};
#endif // __PRIMESERV_H_
