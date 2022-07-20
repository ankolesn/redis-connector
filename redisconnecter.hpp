//
// Created by ankolesn on 20.07.22.
//

#ifndef REDIS_CONNECTOR_REDISCONNECTER_HPP
#define REDIS_CONNECTOR_REDISCONNECTER_HPP
#include <string>
#include <iostream>
#include <vector>
#include <thread>

namespace redis {
    using std::string;
    class RedisConnecter {
    private:
        string ip = "127.0.0.1";
        int port = 6379;
        std::vector<std::thread> thread_pool;
    public:
        RedisConnecter() = default;

        RedisConnecter(string &ip, int port);

        void read_from();

        void write_to(const string &key, const string &value);
    };
    void do_work(const string &key, const string &value){
        std::cout << key << " " << value << std::endl;
    }
}

#endif //REDIS_CONNECTOR_REDISCONNECTER_HPP
