#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char *argv[]) {

    boost::asio::io_context io_context;

    tcp::socket socket(io_context);

    tcp::acceptor acceptor(io_context);

    acceptor.open(tcp::v4());
    acceptor.set_option(tcp::acceptor::reuse_address(true));
    acceptor.bind(tcp::endpoint(tcp::v4(), 4000));
    acceptor.listen();

    for(;;){
        socket = acceptor.accept();
        //socket.set_option(tcp::acceptor::reuse_address(true));
        char buf[64];
        socket.read_some(boost::asio::buffer(buf));

        socket.write_some(boost::asio::buffer("OMAR"));
        std::cout <<"con\n";
        socket.shutdown(tcp::socket::shutdown_both);
        socket.close();
    }
    return 0;
}
