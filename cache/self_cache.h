#ifndef __SELF_CACHE_H_
#define __SELF_CACHE_H_

#pragma once
#include <unordered_map>
#include <iostream>

class MyCache{
    public:
        MyCache();
        MyCache(int);
        bool find(std::string);
        std::string get(std::string);
        int  set(std::string, std::string);
        void show_all();
    private:
        //20 MB
        enum {MAX_CACHE_SIZE = 500000};
        int max_cache_size;
        int size_count;
        std::unordered_map<std::string, std::string> Cache;
};


#endif // __SELF_CACHE_H_
