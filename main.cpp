#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include <cstring>
#include <memory>

using boost::asio::ip::tcp;

boost::asio::io_context io_context;


typedef std::shared_ptr<tcp::socket> socket_ptr;
std::condition_variable g_signal;
bool g_done;
std::mutex g_lock;
std::vector<socket_ptr> clients;

void workerfunc(){
    {
    std::unique_lock<std::mutex> lock(g_lock);
    while(clients.empty())
        g_signal.wait(lock);
    }
    std::cout << "i am here\n" << std::this_thread::get_id() <<'\n';
    socket_ptr sock = clients.back();
    clients.pop_back();
    char buf[128];
    std::memset(buf, 0, 128);
    sock->read_some(boost::asio::buffer(buf));
    std::cout <<buf;
    boost::asio::write(*sock,boost::asio::buffer("OMAR"));
    sock->shutdown(tcp::socket::shutdown_both);
    sock->close();
    //workerfunc();
}



int main(int argc, char *argv[]) {

    boost::asio::io_context io_context;
    std::vector<std::thread> pool_th;
    for(int i=0;i < 5 ; i++){
        pool_th.push_back(std::thread(workerfunc));
    }
    for(int i=0;i < 5 ; i++){
        pool_th[i].detach();
    }

    tcp::acceptor acceptor(io_context);

    acceptor.open(tcp::v4());
    acceptor.set_option(tcp::acceptor::reuse_address(true));
    acceptor.bind(tcp::endpoint(tcp::v4(), 4000));
    acceptor.listen();


    int i =0;
    for(;;){
        socket_ptr sock(new tcp::socket(io_context));
        acceptor.accept(*sock);
        clients.push_back(sock);
        std::cout <<"connect\n";
        g_signal.notify_one();
        //std::cout <<"connect";
    }
    return 0;
}
