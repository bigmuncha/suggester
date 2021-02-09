#ifndef __PRIMESERV_H_
#define __PRIMESERV_H_

#pragma once
#include "server.h"
#include "../cache/self_cache.h"

#include <fstream>
#include <iterator>
#include <algorithm>
#include <boost/regex.hpp>
#include <unordered_map>


class PrimeServ
    :public Server
{
    public:
        PrimeServ();
        PrimeServ(int/*add Cache size*/,int ttl_size=20);
        void quickstart(int thread_size = 5 /*thread_count*/);
        void newWorker();
        std::string resultStr(std::ifstream&,std::string);
        void garbage_func();
        void setTTL(int);
        void setMaxCacheSize(int sizer);
        void run_cache_collector();
        void collector_func();
    private:
        MyCache Cache;
        int str_count;
        std::chrono::duration<int> TTL;
};
#endif // __PRIMESERV_H_
