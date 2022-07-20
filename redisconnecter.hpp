//
// Created by ankolesn on 20.07.22.
//

#ifndef REDIS_CONNECTOR_REDISCONNECTER_HPP
#define REDIS_CONNECTOR_REDISCONNECTER_HPP
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include "concurrent-queue/queue.hpp"
#include <sw/redis++/redis++.h>

namespace redis {
    using std::string;
    class RedisConnecter {
    private:
        string ip = "127.0.0.1";
        int port = 6379;
        //std::vector<std::thread> thread_pool;
        const uint16_t thread_num = std::thread::hardware_concurrency();
        Concurrent_queue<std::pair<string, string>> cq;
        sw::redis::ConnectionOptions opts1;
        std::unique_ptr<sw::redis::Redis> r;

sw::redis::Subscriber* sub;
    public:
        RedisConnecter();
        RedisConnecter(string &ip, int port, sw::redis::Redis r, sw::redis::Subscriber sub);

        void read_from();

        void write_to(const string &key, const string &value);

        void run();

        void run_command(const string &cmd);

        void do_work(){
            std::pair<string, string> p;
            while(true){
                cq.wait_and_pop(p);
                if (p.first == "cmd"){
                    run_command(p.second);
                }
                std::cout << p.first << " " << p.second << std::endl;
            }
        }
    };
}

#endif //REDIS_CONNECTOR_REDISCONNECTER_HPP
