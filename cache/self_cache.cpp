#include "self_cache.h"

MyCache::MyCache()
    :size_count(0), max_cache_size(MAX_CACHE_SIZE)
{

}


MyCache::MyCache(int size)
    :size_count(0),max_cache_size(size)
{

}

bool MyCache::find(std::string real_request){
    return (Cache.find(real_request) ==Cache.end())? 0:1;
}

std::string MyCache:: get(std::string real_request){
    return Cache[real_request];
}

int MyCache::set(std::string key, std::string value){

    if(find(key)){
        return 0;
    }

    int tempcapacity = key.capacity() + value.capacity();
    tempcapacity = size_count + tempcapacity;

    if(tempcapacity >= max_cache_size){
        auto iter_temp = Cache.begin();
        auto iter_main = Cache.begin();
        while(tempcapacity >=max_cache_size){
            tempcapacity-=iter_main->first.capacity() +
                iter_main->second.capacity();
            iter_main++;
            Cache.erase(iter_temp);
            iter_temp = iter_main;
        }
    }

    Cache[key] = value;
    size_count = tempcapacity;

    return 1;
}

void MyCache::show_all(){
    for(auto&a:Cache){
        std::cout << a.first <<" " << a.second <<"| ";
    }
    std::cout <<std::endl;
}
