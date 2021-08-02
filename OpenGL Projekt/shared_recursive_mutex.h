#pragma once

#include <shared_mutex>

using std::shared_mutex;

class shared_recursive_mutex : public shared_mutex {

private:
    std::atomic<std::thread::id> owner;
    int count;

public:
    void lock(void) {
        std::thread::id this_id = std::this_thread::get_id();
        if(owner == this_id) {
            // recursive locking
            count++;
        }
        else {
            // normal locking
            shared_mutex::lock();
            owner = this_id;
            count = 1;
        }
    }
    void unlock(void) {
        if(count > 1) {
            // recursive unlocking
            count--;
        }
        else {
            // normal unlocking
            owner = std::thread::id();
            count = 0;
            shared_mutex::unlock();
        }
    }
};