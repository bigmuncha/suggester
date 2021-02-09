#include "server.h"

Server::Server()
{}

//функция старта сервера - запуск n колличество потоков в начале, вызывает некоторую функцию
//открепляет потоки с помощью детача
void Server::start(MyFunc const & myfunc,int thread_count){

    for(int i=0;i < thread_count ; i++){
        pool_th.push_back(std::thread(
                              [this, myfunc]()
                              {
                                  myfunc();
                              }
                          ));
    }
    for(int i=0;i < thread_count ; i++){
        pool_th[i].detach();
    }

}
//быстрый старт функции заглушки возращающей обычную строку
void Server::quickstart(int thread_count){
    this->start([this](){this->workerfunc();}, thread_count);
}

//функция аксептор(приемник соединений)
//запускается главным потоком - он принимает соединение, на созданный азио-сокет
//заталкивает клиентский сокет в очередь и посылает сигнал воркер тредам
//кто первый перехватит, тот и обрабатывает соединение, но вроде они все равно делают это по порядку.
//правда это работает, только если воркер сразу отдал ответ, в противном случае он блокируется и ждет пока
//клиент скинет данные, в это время другие воркеры без проблем могут вытащить новый сокет из очереди
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

//функция-обработчик соединений
//захватывает очередь, чтобы никто не мешал, и ждет сигнала, как только мастер закинул новый сокет
//воркер получает сигнал и вытаскивает сокет, освобождая ресурс очереди
//клиентский сокет выталкивается и никто другой его не обработает
//кажется что воркер может заблокировать очередь, но так как тут идет проверка на пустоту очереди,
//то вряд-ли кто то обидется
//возварщается стандарный ответ
//шатдаун, клоз -> новый цикл
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
