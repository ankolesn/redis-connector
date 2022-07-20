#include <iostream>
#include <sw/redis++/redis++.h>
#include "redisconnecter.hpp"
#include <thread>

int main() {
    redis::RedisConnecter r;
    r.run();
    //while(true);
//    std::thread tr_for_write(&redis::RedisConnecter::read_from, r);
//    r.write_to("qwe","hello");
    return 0;
}
