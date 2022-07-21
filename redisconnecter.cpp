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


        sub->on_message([this](const std::string &channel, const std::string &msg) {
            cq.push(std::make_pair(channel, msg));
        });

        sub->on_pmessage([this](const std::string &pattern, const std::string &channel, const std::string &msg) {
            cq.push(std::make_pair(channel, msg));
        });

        sub->subscribe("cmd");

        while (true) {
            try {
                sub->consume();
            } catch (const Error &err) {
            }
        }

    }

    void RedisConnecter::write_to(const string &key, const string &value) {
        auto redis1 = Redis(opts1);
        auto pub1 = redis1.publish(key, value);

    }

    RedisConnecter::RedisConnecter() {
        opts1.host = ip;
        opts1.port = port;

        opts1.socket_timeout = std::chrono::milliseconds(100);
        r = std::make_unique<sw::redis::Redis>((opts1));
        static auto t_tmp = r->subscriber();
        sub = &t_tmp;
    }

    void RedisConnecter::run() {
        std::vector<std::thread> threads;
        threads.emplace_back(std::thread(&RedisConnecter::read_from, this));
        for (auto i = 1; i < thread_num; ++i) {
            threads.emplace_back(std::thread(&RedisConnecter::do_work, this));
        }
        threads[0].detach();
        for (auto &&t: threads) {
            if (t.joinable()) {
                t.join();
            }
        }
    }

    void RedisConnecter::run_command(const string &cmd) {
        if (cmd == "exit") {
            exit(0);
        }
        if (cmd.starts_with("add_channel ")) {
            string name = cmd.substr(12);
            sub->subscribe(name);
        }
        if (cmd.starts_with("remove_channel ")) {
            string name = cmd.substr(15);
            sub->unsubscribe();
        }
        if (cmd == "stop") {
            is_stopped = true;
        }
        if (cmd == "resume") {
            is_stopped = false;
        }
    }
}