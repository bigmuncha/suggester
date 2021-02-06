#include "server.h"

Server::Server(){

}

void Server::quickstart(){
    this->start([this](){this->workerfunc();});
}
void Server::start(MyFunc const & myfunc){

    for(int i=0;i < 5 ; i++){
        pool_th.push_back(std::thread(
                              [this, myfunc]()
                              {
                                  myfunc();
                              }
                          ));
    }
    for(int i=0;i < 5 ; i++){
        pool_th[i].detach();
    }

}

void Server::accept(){
    tcp::acceptor acceptor(io_context);

    acceptor.open(tcp::v4());
    acceptor.set_option(tcp::acceptor::reuse_address(true));
    acceptor.bind(tcp::endpoint(tcp::v4(), 4000));
    acceptor.listen();

    for(;;){
        socket_ptr sock(new tcp::socket(io_context));
        acceptor.accept(*sock);
        clients.push_back(sock);
        std::cout <<"connect\n";
        g_signal.notify_one();
        //std::cout <<"connect";
    }

}

void Server::workerfunc(){
    for(;;){
        //waiting untill client exist in set
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
        boost::asio::write(*sock,boost::asio::buffer("OMAR"));
        sock->shutdown(tcp::socket::shutdown_both);
        sock->close();
    }
    //workerfunc();
}
