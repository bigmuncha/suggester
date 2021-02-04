#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <thread>
#include <functional>
#include <cstring>
using boost::asio::ip::tcp;

void workerfunc(tcp::socket& sock){
    char buf[128];
    std::memset(buf, 0, 128);
    sock.read_some(boost::asio::buffer(buf));
    std::cout <<buf;
    boost::asio::write(sock,boost::asio::buffer("OMAR"));
    sock.shutdown(tcp::socket::shutdown_both);
    sock.close();
}



int main(int argc, char *argv[]) {

    boost::asio::io_context io_context;

    tcp::socket socket(io_context);

    std::vector<std::thread> pool_th;
    for(int i=0;i < 10 ; i++){
        pool_th.push_back(std::thread(workerfunc(tcp::socket &sock)));
    }

    tcp::acceptor acceptor(io_context);

    acceptor.open(tcp::v4());
    acceptor.set_option(tcp::acceptor::reuse_address(true));
    acceptor.bind(tcp::endpoint(tcp::v4(), 4000));
    acceptor.listen();

    for(;;){

        socket = acceptor.accept();
        workerfunc(socket);
    }
    return 0;
}
