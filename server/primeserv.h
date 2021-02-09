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


/*
** Прайм-сервер- Cервер для конечной обработки клиента
** Унаследован от основного сервера и работает по тому же принципу
** Этот сервер уже работает не просто отдавая стандартную строку, а парсит
** исходный файл и отдает правильный ответ (смотреть файл how_it_work_ru.txt)
** Имеет при себе Кэш, который ограничен по размеру и TTL
** Кэш имеет собственный сборщик мусора. Запускает его именно Прайм сервер,
** Реализация сборщика мусора в классе кэша
** сначала думал отнаследовать и с класса сервер, типо такое множественное наследование
** а у, но че то подумал говно все это лее, поэтому пока так
*/
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
    private:
        enum {collector_sleep_time = 2};
        MyCache Cache;
        int str_count;
        std::chrono::duration<int> TTL;
};
#endif // __PRIMESERV_H_
