#include "primeserv.h"

//конструктор без параметров
//высчитывает колличество строк в исходном файле
//вызывает стандартный конструктор кэша, ставит стандартный ttl
PrimeServ::PrimeServ()
    :str_count(0),Cache(),TTL(10)
{
    std::ifstream file;
    file.open("../stringset/data.txt");

    if(!file.is_open()){
        std::cerr << "file";
        exit(1);
    }

    char temp[1024];
    while(!file.eof()){
        file.getline(temp, 1024, '\n');
        str_count++;
    }
    std::cout <<str_count << '\n';
    file.close();

}

//тоже самое, но ttl и размер кэша устанавливает пользователь
PrimeServ::PrimeServ(int cache_size,int ttl_size)
    :str_count(0),Cache(cache_size),TTL(ttl_size)
{
    std::ifstream file;
    file.open("../stringset/data.txt");

    if(!file.is_open()){
        std::cerr << "file";
        exit(1);
    }

    char temp[1024];
    while(!file.eof()){
        file.getline(temp, 1024, '\n');
        str_count++;
    }
    std::cout <<str_count << '\n';
    file.close();
}


/*
** новая воркер функция,рабоает так же как
** у родительского сервера, но возвращает другую строку
 */
void PrimeServ::newWorker(){
    std::ifstream file;
    file.open("../stringset/data.txt");
    if(!file.is_open()){
        std::cerr << "file";
        exit(1);
    }
    for(;;){
        file.open("../stringset/data.txt");
        if(!file.is_open()){
            std::cerr << "file";
            exit(1);
        }
        socket_ptr sock;
        {
            std::unique_lock<std::mutex> lock(g_lock);
            while(clients.empty())
                g_signal.wait(lock);
            std::cout << "i am here\n" << std::this_thread::get_id() <<'\n';
            sock = clients.back();
            clients.pop_back();
        }
        char buf[128];
        std::memset(buf, 0, 128);

        sock->read_some(boost::asio::buffer(buf));

        std::cout <<buf;
        file.clear();
        file.seekg(0);
        std::string result = resultStr(file,buf);
        //std::string result = "Omar\n";

        boost::asio::write(*sock,boost::asio::buffer(result));
        sock->shutdown(tcp::socket::shutdown_both);
        sock->close();
    }
    file.close();
}


/*
** Функция обработки файла:
** парсит файл(используются буст регексы)
** по какой схеме возвращается ответ читать в how_it_work_ru.txt
** используется временная мапа Store для упорядочения строк по конечной цифре
** ответ возвращается в обратном порядке
** после всех манипуляций если данных не было в кэше, они туда помещаются
 */
std::string PrimeServ::resultStr(std::ifstream &file,std::string request){

    boost::regex re("^(.+)\\s(\\d+)$");
    boost::smatch m;

    if(!file.is_open()){
        std::cerr << "file";
        exit(1);
    }

    int len = request.length();
    len--;
    std::string real_request = request.substr(0,len);
    std::cout <<len << '\n';
    std::string buf;
    std::string result;
    //std::cout <<file.beg << '\n';
    file.seekg(0);
    //file.clear();
    std::map<int,std::string> Store;

    char stringa[1024];
    std::cout <<"here\n";

    if(Cache.find(real_request)){
        result = Cache.get(real_request);
        std::cout <<"Cache exist\n";

    }else{
        std::cout <<"Cannot find cache\n";
        for(int i=0,j=0; i <str_count ; i++){
        getline(file,buf,'\n');
        if(buf.substr(0,len) == real_request){
            if(regex_match(buf,m,re)){
                int number = std::stoi(m[2]);
                Store[number] = m[1];
                j++;
                if(j > 10) break;
            }else{
                std::cerr <<"NO matches\n";
            }
        }
    }
    for(const auto &a:Store){
        std::cout << a.second << " " << a.first <<"\n";
        result = a.second  + '\n'+ result ;
    }
    Cache.set(real_request, result);
    }
    return result;
}

//функция старта по анологии с род. классом
void PrimeServ::quickstart(int thread_count){
    this->start([this](){this->newWorker();},thread_count);
}

//запуск полной установки сборщика мусора
//2 аргумент это время засыпания сборщика мусора в секундах
void PrimeServ::run_cache_collector(){
    Cache.run_collector(TTL, collector_sleep_time);
}

// explicit
void PrimeServ::setTTL(int sec){
    TTL = std::chrono::duration<int>{sec};
}

//explicit
void PrimeServ::setMaxCacheSize(int sizer){
    Cache.setMaxCacheSize(sizer);
}
