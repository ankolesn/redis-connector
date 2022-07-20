//
// Created by ankolesn on 20.07.22.
//

#include "redisconnecter.hpp"
#include <sw/redis++/redis++.h>
#include <sw/redis++/redis.h>
#include <iostream>

using namespace sw::redis;
namespace redis {

    void RedisConnecter::read_from() {


        sub->on_message([this](std::string channel, std::string msg) {
            cq.push(std::make_pair(channel, msg));
        });

        sub->on_pmessage([this](std::string pattern, std::string channel, std::string msg) {
            cq.push(std::make_pair(channel, msg));
        });

        sub->on_meta([](sw::redis::Subscriber:: MsgType type, sw::redis::OptionalString channel, long long num) {
        });

        sub->subscribe("cmd");
        sub->subscribe({"channel2", "channel3"});

        sub->psubscribe("pattern1*");

        while (true) {
            try {
                sub->consume();
            } catch (const Error &err) {
            }
        }

    }

    void RedisConnecter::write_to(const string &key, const string &value) {
        ConnectionOptions opts1;
        opts1.host = "127.0.0.1";
        opts1.port = 6379;
        opts1.socket_timeout = std::chrono::milliseconds(100);

        auto redis1 = Redis(opts1);

// sub1's socket_timeout is 100ms.
        auto sub1 = redis1.publish(key, value);

    }

    RedisConnecter::RedisConnecter(){
        opts1.host = ip;
        opts1.port = port;

        opts1.socket_timeout = std::chrono::milliseconds(100);
        r = std::make_unique<sw::redis::Redis>( (opts1));
        static auto t_tmp = r->subscriber();
        sub = &t_tmp;
    }

    void RedisConnecter::run() {
        std::vector<std::thread> threads;
        threads.emplace_back(std::thread(&RedisConnecter::read_from, this));
        for (auto i = 1; i < thread_num; ++i) {
            threads.emplace_back(std::thread(&RedisConnecter::do_work,this));
        }
        threads[0].detach();
        for (auto &&t: threads) {
            if(t.joinable()){
                t.join();
            }
        }
    }

    void RedisConnecter::run_command(const string &cmd) {
        if (cmd == "exit"){
            exit(0);
        }
        if (cmd.starts_with("add_channel ")){

        }
    }
}