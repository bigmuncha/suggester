#ifndef __PRIMESERV_H_
#define __PRIMESERV_H_

#include "server.h"
#include <fstream>
#include <iterator>
#include <algorithm>
#include <boost/regex.hpp>
#include <map>
#include <unordered_map>
class PrimeServ
    :public Server
{
    public:
        PrimeServ();
        void quickstart();
        void newWorker();
        std::string resultStr(std::ifstream&,std::string);
    private:
        std::unordered_map<std::string, std::string> Cache;
        int str_count;

};
#endif // __PRIMESERV_H_
