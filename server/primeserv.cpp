#include "primeserv.h"


PrimeServ::PrimeServ()
{
    this->file.open("../stringset/data.txt");

    if(!file.is_open()){
        std::cerr << "file";
        exit(1);
    }
}

void PrimeServ::newWorker(){
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
        std::string result = resultStr(buf);

        boost::asio::write(*sock,boost::asio::buffer(result));
        sock->shutdown(tcp::socket::shutdown_both);
        sock->close();
    }
}

std::string PrimeServ::resultStr(std::string request){

}
