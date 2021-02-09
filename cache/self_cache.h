#ifndef __SELF_CACHE_H_
#define __SELF_CACHE_H_

#pragma once
#include <unordered_map>
#include <iostream>
#include <chrono>
#include <mutex>
#include <thread>

/*
** Класс для кэша данных типа строка- строка
** используется контейнер ун_мапа
** реализованы стандартные методы гет сeт файнд
** можно установить максимальный размер кэша
** в методе сет реализована защита от переполнения кэша
** мьютекс для захвата мапы
** реализован сборщик мусора, который собирает данные с просроченным TTL
** для TTL решил засунуть время создания элемента во вторую переменную контейнера
** TTL высчитывается с помощью <chrono> - там какие то дурэйшоны и еще какая-та херня,
** сборщик мусора имеет 3 вспомогательные функции
** collector_func - единовременный заход по мапе и очистка просрочки
** collector_loop - такой же заход, но в цикле с определенной задержкой(засыпание в потоке, чтобы сильно не грузить все)
** run_collector - запуск мусорщика в новом потоке, детач
 */
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
        void collector_func(std::chrono::duration<int>);
        void run_collector(std::chrono::duration<int>,int);
        void collector_loop(std::chrono::duration<int>,int);
        void setMaxCacheSize(int );
    private:
        //20 MB
        std::mutex g_lock;
        enum {MAX_CACHE_SIZE = 500000};
        int max_cache_size;
        int size_count;
        std::unordered_map<std::string,
                           std::pair<Time,std::string>> Cache;
};


#endif // __SELF_CACHE_H_
