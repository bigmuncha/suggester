#ifndef __SERVER_H_
#define __SERVER_H_

#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include <cstring>
#include <memory>

using boost::asio::ip::tcp;

class Server{
    public:
        Server();
        void workerfunc();
        void start();
        void accept();

        typedef std::shared_ptr<tcp::socket> socket_ptr;


    private:
        std::condition_variable g_signal;
        bool g_done;
        std::mutex g_lock;
        std::vector<socket_ptr> clients;
        boost::asio::io_context io_context;
        std::vector<std::thread> pool_th;

};

#endif // __SERVER_H_
