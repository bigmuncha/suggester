#include "primeserv.h"

PrimeServ::PrimeServ()
    :str_count(0),Cache()
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


PrimeServ::PrimeServ(int cache_size)
    :str_count(0),Cache(cache_size)
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


void PrimeServ::newWorker(){

    std::ifstream file;

    file.open("../stringset/data.txt");

    if(!file.is_open()){
        std::cerr << "file";
        exit(1);
    }
    for(;;){
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

        boost::asio::write(*sock,boost::asio::buffer(result));
        sock->shutdown(tcp::socket::shutdown_both);
        sock->close();
    }
    file.close();
}

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
    file.clear();
    std::map<int,std::string> Store;

    char stringa[1024];
    std::cout <<"here\n";

    if(Cache.find(real_request)){
        result = Cache.get(real_request);
        std::cout <<"Cache exist\n";
    }else{
        std::cout <<"Cannot find cache\n";
        for(int i=0; i <str_count; i++){
            getline(file,buf,'\n');
            if(buf.substr(0,len) == real_request){
                if(regex_match(buf,m,re)){
                    int number = std::stoi(m[2]);
                    Store[number] = m[1];
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

void PrimeServ::quickstart(int thread_count){
    this->start([this](){this->newWorker();},thread_count);
}
