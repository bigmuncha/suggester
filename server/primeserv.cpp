#include "primeserv.h"


PrimeServ::PrimeServ()
    :str_count(0)
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
    //file.clear();
    //file.seekg(0, file.beg);
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
    std::cout <<len-1 << '\n';
    std::string buf;
    std::string result;
    //std::cout <<file.beg << '\n';
    file.seekg(0);
    file.clear();
    std::map<int,std::string> Store;

    char stringa[1024];
    std::cout <<"here\n";
    for(int i=0; i <str_count; i++){
        getline(file,buf,'\n');
        if(buf.substr(0,len-1) == request.substr(0,len-1)){
            //std::cout <<"|"<< buf.substr(0,len-1)<< "| "
              //        <<"|"<< request.substr(0,len-1) <<"|\n";
            if(regex_match(buf,m,re)){
                int number = std::stoi(m[2]);
                Store[number] = m[1];
//                std::cout <<"Match|" <<m[0] << "|\n";
            }else{
                std::cerr <<"NO matches\n";
            }
            //result = result + buf+'\n' ;
        }
    }
    for(const auto &a:Store){
        std::cout << a.second << " " << a.first <<"\n";
        result = result + a.second + '\n';
    }

    /*for(int i =0; i < str_count; i++){
        //std::cout <<"here\n";
        std::memset(buf,0,len);
        file.getline(buf, len);
        std::cout <<buf;
        if (buf == request.c_str()){
            file.getline(stringa,1024,'\n');
            std::cout <<stringa << '\n';
            result = result + stringa;
        }
    }*/


    return result;
}

void PrimeServ::quickstart(){
    this->start([this](){this->newWorker();});
}
