#include <DBASEV/visibility.h>
#include <DBASEV/collision_avoidance.h>
#include <DBASEV/communication.h>
#include <DBASEV/drone_control.h>


int size;

#define NUM_THREADS 2
void * thread_func2(void *arg)
{
    /*send data with queue*/
    tbb::concurrent_queue<const char*>* cq = static_cast<tbb::concurrent_queue<const char*>*>(arg);
    for (int i = 0; i < 10000; i++) {
        const char* msg = "message success";
        const char* tmp;
        while (cq->try_pop(tmp)) {
        // Do something with x
        size--;
        printf("queue size is %d\n",size);
    }
        cq->push(msg); // push item into the concurrent queue
        size++;
        printf("queue size is %d\n",size);
    }
    return 0;

}

void * thread_func1(void *arg)
{   
    /*get data from queue*/
    tbb::concurrent_queue<const char*>* cq = static_cast<tbb::concurrent_queue<const char*>*>(arg);
    const char* msg;
    for (int i = 0; i < 10000; i++) {
        if (cq->try_pop(msg)) { // try to pop item from the concurrent queue
            size--;
            printf("queue size is %d\n",size);
            printf("%s\n",msg);
        } else {
            printf("Concurrent queue is empty\n");
        }
    }

    
    //func = "embedd";
    //callPython(src, func, 1, 1);   
    
    return 0;
}

int main()
{
    void *status;
    int thr_id;
    time_t begin = clock();
    tbb::concurrent_queue<const char*> cq; // concurrent queue
    
    // 각각의 스레드를 생성
    pthread_t threads[NUM_THREADS];

    //첫번째 스레드 생성
    thr_id = pthread_create(&threads[0], NULL, &thread_func1, (void *)&cq);
    if(thr_id < 0){
        perror("failure create thread");
    }
    // 두번째 스레드 생성
    thr_id = pthread_create(&threads[1], NULL, &thread_func2, (void *)&cq);
    if(thr_id < 0){
        perror("failure create thread");
    }
   

    // 각각의 스레드가 종료될 때까지 대기
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], &status);
    }

    //모든 스레드 종료시 메인스레드 기능
    printf("all of threads are dead");

    return 0;
}