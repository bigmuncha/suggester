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
    return Cache[real_request].second;
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
                iter_main->second.second.capacity();
            iter_main++;
            Cache.erase(iter_temp);
            iter_temp = iter_main;
        }
    }

    Cache[key].second = value;
    Cache[key].first = std::chrono::steady_clock::now();
    size_count = tempcapacity;

    return 1;
}

void MyCache::show_all(){
    for(auto&a:Cache){
        std::cout << a.first <<" " << a.second.second <<"| ";
    }
    std::cout <<std::endl;
}

int MyCache::erase(std::string key){
    if(!find(key)){
        return 0;
    }
    Cache.erase(key);
    return 1;
}

void MyCache::garb_collector(std::chrono::duration<int> TTL){

    auto temp = Cache.begin();
    auto current =Cache.begin();
    for(;current!=Cache.end();){
        current++;
        if( TTL < std::chrono::steady_clock::now()
            - temp->second.first ){
            Cache.erase(temp);
            std::cout <<"Че то удалил\n";
        }else{
            std::cout << "niche ne ydalil\n";
        }
        temp = current;
    }

}

void MyCache::setMaxCacheSize(int i){
    max_cache_size = i;
}
