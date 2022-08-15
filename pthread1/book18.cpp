// 本程序演示用互斥锁和条件变量实现高速缓存
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<signal.h>
#include<string.h>
#include<vector>

using namespace std;

// 缓存消息队列的结构体
struct st_message{
  int mesgid; // 消息的id
  char message[1024]; // 消息的内容
}stmesg;

vector<struct st_message> vcache; // 用vector容器做缓存

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void incache(int sig); // 生产者，数据入队
void* outcache(void* arg); // 消费者，数据出队线程的主函数

int main(int argc, char* argv[]){
  signal(15, incache); // 接收15的信号，调用生产者函数

  // 创建三个消费者线程
  pthread_t thid1, thid2, thid3;
  pthread_create(&thid1, NULL, outcache, NULL);
  pthread_create(&thid2, NULL, outcache, NULL);
  pthread_create(&thid3, NULL, outcache, NULL);
sleep(2);pthread_cancel(thid1);pthread_cancel(thid2);pthread_cancel(thid3);

  pthread_join(thid1, NULL);
  pthread_join(thid2, NULL);
  pthread_join(thid3, NULL);

}

void incache(int sig){ // 生产者，数据入队
  static int mesgid = 1; // 消息计数器
  struct st_message stmesg; // 消息内容
  memset(&stmesg, 0, sizeof(struct st_message));

  pthread_mutex_lock(&mutex);

  // 生产数据，放入队列
  stmesg.mesgid = mesgid++; vcache.push_back(stmesg);
  stmesg.mesgid = mesgid++; vcache.push_back(stmesg);

  pthread_mutex_unlock(&mutex);

  pthread_cond_broadcast(&cond); // 发送条件信号，激活所有线程。
}

void thcleanup(void* arg){
  // 这里释放关闭文件，断开网络连接，回滚数据库事务，释放锁等
  printf("cleanup ok.\n");
  pthread_mutex_unlock(&mutex);
}

void* outcache(void* arg){ // 消费者，数据出队线程的主函数
  pthread_cleanup_push(thcleanup, NULL);
  struct st_message stmesg;
  while(true){
    pthread_mutex_lock(&mutex); // 给缓存队列加锁
    // 如果缓存队列为空，等待，用while防止虚假唤醒
    while(vcache.size() == 0){
      pthread_cond_wait(&cond, &mutex);
    }

    // 从缓存队列中获取第一条记录
    memcpy(&stmesg, &vcache[0], sizeof(struct st_message));
    vcache.erase(vcache.begin());

    pthread_mutex_unlock(&mutex);

    // 以下是业务处理代码
    printf("phid=%ld, mesgid=%d\n", pthread_self(), stmesg.mesgid);
    usleep(100);
  }
  pthread_cleanup_pop(1); // 清理函数出栈
}