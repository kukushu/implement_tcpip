#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
// 定义一个互斥锁
pthread_mutex_t mutex;
// 共享数据
int shared_data = 0;
// 线程处理函数
void* thread_function(void* arg) {
    // 加锁
    pthread_mutex_lock(&mutex);
    // 业务处理，修改共享数据
    shared_data++;
    printf("Thread %d has increased shared_data to %d\n", *(int*)arg, shared_data);
    // 解锁
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    // 初始化互斥锁，默认属性
    pthread_mutex_init(&mutex, NULL);
    // 创建两个线程
    pthread_t thread1, thread2;
    int thread_id1 = 1;
    int thread_id2 = 2;
    pthread_create(&thread1, NULL, thread_function, &thread_id1);
    pthread_create(&thread2, NULL, thread_function, &thread_id2);

    // 等待线程结束
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // 销毁互斥锁
    pthread_mutex_destroy(&mutex);

    return 0;
}
