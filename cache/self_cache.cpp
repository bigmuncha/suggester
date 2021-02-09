#include "self_cache.h"

MyCache::MyCache()
    :size_count(0), max_cache_size(MAX_CACHE_SIZE)
{}

MyCache::MyCache(int size)
    :size_count(0),max_cache_size(size)
{}
//поиск элемента в кэше
bool MyCache::find(std::string real_request){
    return (Cache.find(real_request) ==Cache.end())? 0:1;
}

//получение значения элемента(результирующая строка,
//которая будет отправлена клиенту)
std::string MyCache:: get(std::string real_request){
    return Cache[real_request].second;
}

//установка значения
//если размер текущего_счетчика + новый элемент > чем макс_значение кэша
//то мы вытесняем данные из кэша, пока не достигнем допустимого значения
int MyCache::set(std::string key, std::string value){

    std::unique_lock<std::mutex> lock(g_lock);
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

//показывает данные
void MyCache::show_all(){
    for(auto&a:Cache){
        std::cout << a.first <<" " << a.second.second <<"| ";
    }
    std::cout <<std::endl;
}

//удаление элемента
int MyCache::erase(std::string key){
    if(!find(key)){
        return 0;
    }
    Cache.erase(key);
    return 1;
}

//функция, которая пробегает по мапе и ищет значения с просроченным TTL
//захватываем мьютекс, чтобы никто не вставлял данные во время пробежки
void MyCache::collector_func(std::chrono::duration<int> TTL){

    std::unique_lock<std::mutex> lock(g_lock);
    auto temp = Cache.begin();
    auto current =Cache.begin();
    for(;current!=Cache.end();){
        current++;
        if( TTL < std::chrono::steady_clock::now()
            - temp->second.first ){
            Cache.erase(temp);
            //std::cout <<"Че то удалил\n";
        }else{
            //std::cout << "niche ne ydalil\n";
        }
        temp = current;
    }
}

//запускает верхнюю функцию в цикле, с указанной задержкой:
//мы засыпаем в треде, чтобы не грузить систему
//
void MyCache::collector_loop(std::chrono::duration<int> TTL, int timeout){
    for(;;){
        collector_func(TTL);
        std::this_thread::sleep_for(std::chrono::seconds(timeout));
    }
}

//запуск сборщика в отдельной треде, для избежания блокировки
void MyCache::run_collector(std::chrono::duration<int> TTL, int timeout){
        std::thread t([this,TTL,timeout](){
        this->collector_loop(TTL,timeout);
    });
    t.detach();
}
//установка макс значения кэша
void MyCache::setMaxCacheSize(int i){
    max_cache_size = i;
}
