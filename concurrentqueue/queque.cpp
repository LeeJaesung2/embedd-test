#include <iostream>
#include <thread>
#include <tbb/concurrent_priority_queue.h>

using namespace concurrency;

void *producer(concurrent_priority_queue<int>& q) {
    for (int i = 0; i < 10; ++i) {
        q.push(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void *consumer(concurrent_priority_queue<int>& q) {
    while (true) {
        int value;
        if (q.try_pop(value)) {
            std::cout << "Popped value: " << value << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

int main() {
    concurrent_priority_queue<int> q;

    pthread_create(&threads[0], NULL, &producer, &(ref(q)));
    pthread_create(&threads[0], NULL, &consumer, &(ref(q)));

    producer_thread.join();
    consumer_thread.join();

    return 0;
}
