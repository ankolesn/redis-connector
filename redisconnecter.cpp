//
// Created by ankolesn on 20.07.22.
//

#include "redisconnecter.hpp"
#include <sw/redis++/redis++.h>
#include <sw/redis++/redis.h>
#include <iostream>

using namespace sw::redis;
namespace redis {
    RedisConnecter::RedisConnecter(string &ip, int port):ip(ip), port(port){
        const uint16_t num = std::thread::hardware_concurrency();
        thread_pool = std::vector<std::thread> (num);
    }

    void RedisConnecter::read_from() {
        sw::redis::ConnectionOptions opts1;
        opts1.host = ip;
        opts1.port = port;
        opts1.socket_timeout = std::chrono::milliseconds(100);
        auto redis = sw::redis::Redis(opts1);

        auto sub = redis.subscriber();

        sub.on_message([](std::string channel, std::string msg) {
        });

        sub.on_pmessage([](std::string pattern, std::string channel, std::string msg) {
        });

        sub.on_meta([](sw::redis::Subscriber:: MsgType type, sw::redis::OptionalString channel, long long num) {
        });

        sub.subscribe("channel1");
        sub.subscribe({"channel2", "channel3"});

        sub.psubscribe("pattern1*");

        while (true) {
            try {
                sub.consume();
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
}