#ifndef __SELF_CACHE_H_
#define __SELF_CACHE_H_

#pragma once
#include <unordered_map>
#include <iostream>
#include <chrono>

class MyCache{
    public:
        typedef std::chrono::steady_clock::time_point Time;
        MyCache();
        MyCache(int);
        bool find(std::string);
        std::string get(std::string);
        int  set(std::string, std::string);
        void show_all();
        int erase(std::string);
        int garb_coll();
    private:
        //20 MB
        enum {MAX_CACHE_SIZE = 500000};
        int max_cache_size;
        int size_count;
        std::unordered_map<std::string,
                           std::pair<Time,std::string>> Cache;
};


#endif // __SELF_CACHE_H_
