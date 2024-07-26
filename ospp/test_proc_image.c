#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <argp.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <sched.h>
#include <linux/fcntl.h>
#include <string.h>
#include <math.h>
#include <lttng/tracepoint.h>

// 定义跟踪点

#include "lock_tracepoint.h"
#define NUM_THREADS 16
#define gettid() syscall(__NR_gettid)

pthread_mutex_t mutex1,mutex2;
pthread_rwlock_t rwlock;
pthread_spinlock_t spinlock;

static struct env {
   bool keytime_test;
   bool lock_test;
   bool resource_test;
   bool syscall_test;
   bool schedule_test;
} env = {
   .keytime_test = false,
   .lock_test = false,
   .resource_test = false,
   .syscall_test = false,
   .schedule_test = false,
};

const char argp_program_doc[] ="To test process_image.\n";

static const struct argp_option opts[] = {
   { "keytime", 'k', NULL, 0, "To test keytime_image" },
   { "lock", 'l', NULL, 0, "To test lock_image" },
   { "resource", 'r', NULL, 0, "To test resource_image" },
   { "syscall", 's', NULL, 0, "To test syscall_image" },
   { "schedule", 'S', NULL, 0, "To test schedule_image" },
   { "all", 'a', NULL, 0, "To test all_image" },
   { NULL, 'h', NULL, OPTION_HIDDEN, "show the full help" },
   {},
};

static error_t parse_arg(int key, char *arg, struct argp_state *state)
{
	switch (key) {
		case 'a':
				env.keytime_test = true;
				env.lock_test = true;
				env.resource_test = true;
				env.syscall_test = true;
            env.schedule_test = true;
				break;
		case 'k':
				env.keytime_test = true;
				break;
		case 'l':
            env.lock_test = true;
            break;
      case 'r':
            env.resource_test = true;
            break;
		case 's':
				env.syscall_test = true;
            break;
      case 'S':
            env.schedule_test = true;
            break;
		case 'h':
				argp_state_help(state, stderr, ARGP_HELP_STD_HELP);
				break;
      default:
         return ARGP_ERR_UNKNOWN;
	}
	
	return 0;
}

pid_t get_tgid() {
    pid_t pid = getpid();
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    
    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("打开 proc/pid/status 文件失败");
        return -1;
    }
    
    char line[256];
    int tgid = -1,cpuid = -1;
    
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "Tgid:", 5) == 0) {
            sscanf(line, "Tgid: %d", &tgid);
            break;
        }
    }
    
    fclose(fp);
    return tgid;
}
void print_time(){
   time_t now ;
   now = time(NULL);
   struct tm *localTime = localtime(&now);
   printf("%04d-%02d-%02d %02d:%02d:%02d\n",
         localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday,
         localTime->tm_hour, localTime->tm_min, localTime->tm_sec);   
}
int set_cpu(int pid){
   int num_cores = sysconf(_SC_NPROCESSORS_ONLN); // 获取可用的 CPU 核心数
   // printf("Available CPU cores: %d\n", num_cores);
   int cpu_id=4;
   cpu_set_t mask;    // CPU 亲和性掩码
   CPU_ZERO(&mask);   // 清空 CPU 亲和性掩码
   CPU_SET(cpu_id, &mask); // 将 CPU 0 加入掩码，可以根据需要修改为其他核心
   int result = sched_setaffinity(pid, sizeof(mask), &mask); // 设置进程的 CPU 亲和性
   if (result == 0) {
       printf("    #PID%d绑定CPU%d：√\n", pid,cpu_id);
   } else {
       perror("绑定cpu失败");
       return -1;
   }
   // 确认当前进程的 CPU 亲和性
   cpu_set_t get_mask;
   CPU_ZERO(&get_mask);
   if (sched_getaffinity(pid, sizeof(get_mask), &get_mask) == -1) {
       perror("Error retrieving CPU affinity");
       return -1;
   }
   printf("    #PID%d 被绑定在以下cpu上: ", pid);
   for (int i = 0; i < num_cores; i++) {
       if (CPU_ISSET(i, &get_mask)) {
           printf("%d ", i);
       }
   }
   printf("\n");
   return cpu_id;   
}
void do_cpu(){
   printf("  #CPU密集型\n");
   double result = 0.0;
   clock_t start_time = clock();
   while ((clock() - start_time) < 5 * CLOCKS_PER_SEC) {
      for (int i = 0; i < 1000000; i++) {
           result += sin(i) * tan(i);
      }
   }      
}
void do_mem(){
   printf("  #MEM 密集型\n");
   long long int total_memory = 3LL* 1024 * 1024;
   char *memory = (char *)malloc(total_memory);
   if (memory == NULL) {
      printf("  #内存分配失败！\n");
      return;
   }
   memset(memory, 'A', total_memory);
   clock_t start_time = clock();
   // while ((clock() - start_time) < 3 * CLOCKS_PER_SEC) {}   
   sleep(3);
   free(memory);
}

#define BUFFER_SIZE 2*1024*1024
void do_io(){
   printf("  #IO密集型\n");
   const char *command = "rm io_test.txt";
   char buffer[BUFFER_SIZE];
   memset(buffer, 'A', sizeof(buffer));
   for(int i = 0; i < 5; i++){
      FILE *file = fopen("./io_test.txt", "w");
      if (file == NULL) {
         break;
      }
      fwrite(buffer, 1, 2*1024*1024, file);
      fclose(file);
      
      file = fopen("./io_test.txt", "r");
      if (file == NULL) {
         break;
      }
      fread(buffer, 1, 2*1024*1024, file);
      fclose(file);
   }
   sleep(3);
   system(command);
   //printf("\n");
}
void *func(void *args)
{
    int tpid;
    int pid = getpid();
    tpid = gettid();
    int *type = (int*)args;
    printf("\n  #%d.PID:%d TGID:%d\n",*type,tpid,pid);
   //  int cpu_id = set_cpu(tpid);

    switch (*type){
       case 1:
          do_cpu();
          break;
       case 2:
          do_mem();
          break;
       case 3:
          do_io();
          break;
       default:
          sleep(3);
    }
    printf("    #%d退出\n",tpid);
}
void *thread_lock_func1(void *arg) {
   struct timespec ts;
   int pid=getpid(),tid = gettid();
   printf("线程1 tid:%d ...\n",tid);

/*互斥锁的测试*/
   clock_gettime(CLOCK_MONOTONIC, &ts);
   tracepoint(lock_monitor, mutex_lock_start, 
                  tid,&mutex1,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
   if (pthread_mutex_lock(&mutex1) == 0) {
      clock_gettime(CLOCK_MONOTONIC, &ts);
      tracepoint(lock_monitor, mutex_lock_acquired, 
                  tid,&mutex1,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
      printf("tid :%d 申请互斥锁成功,并将持有1s\n",tid);
      sleep(1);
      pthread_mutex_unlock(&mutex1);
      clock_gettime(CLOCK_MONOTONIC, &ts);
      tracepoint(lock_monitor, mutex_lock_released, 
                  tid,&mutex1,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
      printf("tid :%d 互斥锁解锁成功\n",tid);
   }else{
      clock_gettime(CLOCK_MONOTONIC, &ts);
      tracepoint(lock_monitor, mutex_trylock_failed, 
                  tid,&mutex1,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
      printf("tid :%d 申请互斥锁失败\n",tid);
   }   

/*读写锁测试*/
   printf("tid:%d 尝试申请读写锁...\n",tid);
   clock_gettime(CLOCK_MONOTONIC, &ts);
   tracepoint(lock_monitor, rwlock_rdlock_start, 
               tid,&rwlock,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
   if (pthread_rwlock_rdlock(&rwlock) == 0) {
      clock_gettime(CLOCK_MONOTONIC, &ts);
      tracepoint(lock_monitor, rwlock_rdlock_acquired, 
                     tid,&rwlock,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
      printf("tid :%d 申请读写锁-读锁成功,并将持有1s\n",tid);
      sleep(1);
      pthread_rwlock_unlock(&rwlock);
      clock_gettime(CLOCK_MONOTONIC, &ts);
      tracepoint(lock_monitor, rwlock_rdlock_released, 
                     tid,&rwlock,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
      printf("tid :%d 读写锁-读锁解锁成功\n",tid);
   }else{
      clock_gettime(CLOCK_MONOTONIC, &ts);
      tracepoint(lock_monitor, rwlock_rdlock_failed, 
                     tid,&rwlock,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
      printf("tid :%d 申请读写锁-读锁失败\n",tid);
   }   

/*自旋锁测试*/
   printf("tid:%d 尝试申请自旋锁...\n",tid);
   clock_gettime(CLOCK_MONOTONIC, &ts);
   tracepoint(lock_monitor, spinlock_lock_start, 
               tid,&spinlock,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
   if (pthread_spin_lock(&spinlock) == 0) {
      clock_gettime(CLOCK_MONOTONIC, &ts);
      tracepoint(lock_monitor, spinlock_lock_acquired, 
                     tid,&spinlock,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
      printf("tid :%d 申请自旋锁成功,并将持有1s\n",tid);
      sleep(1);
      pthread_spin_unlock(&spinlock);
      clock_gettime(CLOCK_MONOTONIC, &ts);
      tracepoint(lock_monitor, spinlock_lock_released, 
                     tid,&spinlock,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
      printf("tid :%d 自旋锁解锁成功\n",tid);
   }else{
      clock_gettime(CLOCK_MONOTONIC, &ts);
      tracepoint(lock_monitor, spinlock_trylock_failed, 
                     tid,&spinlock,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
      printf("tid :%d 申请自旋锁失败\n",tid);
   } 

   return NULL;
}

void *thread_lock_func2(void *arg) {
   struct timespec ts;
   int pid=getpid(),tid = gettid();
   sleep(1); // Ensure thread 1 acquires the lock first
   printf("线程2 tid:%d ...\n",tid);

/*互斥锁的测试*/
   clock_gettime(CLOCK_MONOTONIC, &ts);   
   tracepoint(lock_monitor, mutex_trylock_start, 
               tid,&mutex1,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
   if (pthread_mutex_trylock(&mutex1) == 0) {
      tracepoint(lock_monitor, mutex_trylock_acquired, 
                    tid,&mutex1,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
      printf("tid :%d 申请互斥锁1成功,并将持有1s\n",tid);
      sleep(1);
      pthread_mutex_unlock(&mutex1);
      clock_gettime(CLOCK_MONOTONIC, &ts);
      tracepoint(lock_monitor, mutex_lock_released, 
                 tid,&mutex1,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
      printf("tid :%d 互斥锁解锁1成功\n",tid);
   } else if(pthread_mutex_trylock(&mutex2) == 0) {
      clock_gettime(CLOCK_MONOTONIC, &ts);
      tracepoint(lock_monitor, mutex_trylock_failed, 
                  tid,&mutex1,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
      printf("tid :%d 申请互斥锁1失败\n",tid);
      printf("tid :%d 尝试申请读写锁2...\n",tid);     
      tracepoint(lock_monitor, mutex_trylock_acquired, 
                    tid,&mutex2,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
      printf("tid :%d 申请互斥锁2成功,并将持有1s\n",tid);
      sleep(1);
      pthread_mutex_unlock(&mutex2);
      clock_gettime(CLOCK_MONOTONIC, &ts);
      tracepoint(lock_monitor, mutex_lock_released, 
                 tid,&mutex2,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
      printf("tid :%d 互斥锁解锁2成功\n",tid);
   } else {
      clock_gettime(CLOCK_MONOTONIC, &ts);
      tracepoint(lock_monitor, mutex_trylock_failed, 
                  tid,&mutex1,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
      tracepoint(lock_monitor, mutex_trylock_failed, 
                  tid,&mutex2,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
      printf("tid :%d 申请互斥锁1失败\n",tid);
      printf("tid :%d 申请互斥锁2失败\n",tid);
   }

/*读写锁测试*/
   printf("tid:%d 尝试申请读写锁...\n",tid);
   clock_gettime(CLOCK_MONOTONIC, &ts);
   tracepoint(lock_monitor, rwlock_wrlock_start, 
               tid,&rwlock,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
   if (pthread_rwlock_trywrlock(&rwlock) == 0) {
      clock_gettime(CLOCK_MONOTONIC, &ts);
      tracepoint(lock_monitor, rwlock_wrlock_acquired, 
                     tid,&rwlock,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
      printf("tid :%d 申请读写锁-写锁成功,并将持有1s\n",tid);
      sleep(1);
      pthread_rwlock_unlock(&rwlock);
      clock_gettime(CLOCK_MONOTONIC, &ts);
      tracepoint(lock_monitor, rwlock_wrlock_released, 
                     tid,&rwlock,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
      printf("tid :%d 读写锁-写锁解锁成功\n",tid);
   }else{
      clock_gettime(CLOCK_MONOTONIC, &ts);
      tracepoint(lock_monitor, rwlock_wrlock_failed, 
                     tid,&rwlock,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
      printf("tid :%d 申请读写锁-写锁失败\n",tid);
   }   
  
/*自旋锁测试*/
   printf("tid:%d 尝试申请自旋锁...\n",tid);
   clock_gettime(CLOCK_MONOTONIC, &ts);
   tracepoint(lock_monitor, spinlock_lock_start, 
               tid,&spinlock,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
   if (pthread_spin_trylock(&spinlock) == 0) {
      clock_gettime(CLOCK_MONOTONIC, &ts);
      tracepoint(lock_monitor, spinlock_trylock_acquired, 
                     tid,&spinlock,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
      printf("tid :%d 申请自旋锁成功,并将持有1s\n",tid);
      sleep(1);
      pthread_spin_unlock(&spinlock);
      clock_gettime(CLOCK_MONOTONIC, &ts);
      tracepoint(lock_monitor, spinlock_lock_released, 
                     tid,&spinlock,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
      printf("tid :%d 自旋锁解锁成功\n",tid);
   }else{
      clock_gettime(CLOCK_MONOTONIC, &ts);
      tracepoint(lock_monitor, spinlock_trylock_failed, 
                     tid,&spinlock,(long long unsigned int)(ts.tv_sec*1000000000+ts.tv_nsec));
      printf("tid :%d 申请自旋锁失败\n",tid);
   } 
   return NULL;
}
// void *cpu_intensive_task(void* arg) {
//     // 获取当前线程的 PID 和 TGID
//     pid_t pid = syscall(SYS_gettid);
//     pid_t tgid = getpid();

//     printf("Thread PID: %d, TGID: %d\n", pid, tgid);

//     // 执行 CPU 密集型任务
//     unsigned long long i, j = 0;
//     for (i = 0; i < 1000000000ULL; i++) {
//         j += i;
//     }
//     return NULL;
// }
int main(int argc, char **argv){
   int pid,stop,tgid;
   int err;
   static const struct argp argp = {
		.options = opts,
		.parser = parse_arg,
		.doc = argp_program_doc,
	};

	err = argp_parse(&argp, argc, argv, 0, NULL, NULL);
	if (err)
		return err;

   pid = getpid();
   tgid = get_tgid();
   printf("test_proc进程的TGID:%d \t PID:%d \t CPU_id:%d\n", tgid,pid,set_cpu(pid));
   // printf("test_proc进程的TGID:%d \t PID:%d\n", tgid,pid);
   printf("输入任意数字继续程序的运行:");
   scanf("%d",&stop);                   // 使用时将其取消注释
   printf("程序开始执行...\n");
   printf("\n");

   if(env.keytime_test){
      printf("KEYTIME_TEST-----------------------------------------------\n\n");

   //fork
      printf("fork逻辑------------------------:\n");      
      printf("fork开始时间");
      print_time();
      pid = fork();
      if(pid < 0){
         printf("fork error\n");
      }else if(pid == 0){
         pid = getpid();
         tgid = get_tgid();
         printf("(fork)子进程pid:%d,睡眠2s后退出\n",pid);
         sleep(2);
         exit(0);
      }else{
         pid = wait(NULL);
         printf("fork结束时间");
         print_time();
         printf("(fork)pid为%d的子进程退出\n\n",pid);
      }

   //vfork
      printf("输入任意数字开始测试vfork:");
      printf("vfork逻辑------------------------:\n");
      printf("vfork开始时间");
      print_time();
      pid = vfork();
      if(pid < 0){
         printf("fork error\n");
      }else if(pid == 0){
         pid = getpid();
         printf("(vfork)子进程pid:%d,睡眠2s后退出\n",pid);
         sleep(2);
         exit(0);
      }else{
         pid = wait(NULL);
         printf("vfork退出时间");
         print_time();
         printf("(vfork)pid为%d的子进程退出\n",pid);
      }
      printf("\n");  

   //pthread_create逻辑: 创建线程并让线程睡眠3秒，以表示它存在的时间
      printf("输入任意数字开始测试 pthread_create:");

      printf("pthread_create逻辑------------------------:\n");
      printf("pthread_create开始时间");
      print_time();    
      pthread_t tid_cpu,tid_mem,tid_io;
      int type_cpu =1,type_mem =2,type_io = 3;
      err = pthread_create(&tid_cpu,NULL,&func,&type_cpu);
      if(err != 0) printf("线程1创建失败\n");
      err = pthread_create(&tid_mem,NULL,&func,&type_mem);
      if(err != 0) printf("线程2创建失败\n");
      err = pthread_create(&tid_io,NULL,&func,&type_io);
      if(err != 0) printf("线程3创建失败\n");
      pthread_join(tid_cpu, NULL);
      pthread_join(tid_mem, NULL);
      pthread_join(tid_io, NULL);   
      printf("pthread_create结束时间");
      print_time();   
      printf("\n");   

   //进程上下CPU逻辑: 利用sleep函数使进程睡眠3秒,即 offCPU 3秒
      printf("输入任意数字开始测试 上下cpu:");
      printf("进程上下CPU逻辑------------------------:\n");
      printf("CPU sleep 开始时间");
      print_time();
      sleep(3);
      printf("CPU sleep 结束时间");
      print_time();
      printf("程序睡眠3s!\n");
      printf("\n");        

   //execve逻辑: 替换当前进程，ARGV和ENVP以NULL指针结束
      // 若出错，返回-1；若成功，不返回
      printf("输入任意数字开始测试 execve:");
      printf("execve开始时间");
      print_time();
      char *argvv[] = { "ls", "-l", NULL };
      char *envp[] = { "PATH=/bin", NULL };
      printf("execve逻辑------------------------:\n");
      execve("/bin/ls", argvv, envp);
      perror("execve");
      printf("\n");    


   //exit逻辑: 可自行输入退出值
      printf("输入任意数字开始测试 exit:");
      printf("exit逻辑------------------------: \n");
      int error_code;
      printf("请输入程序退出的error_code值:");
      scanf("%d",&error_code);
      exit(error_code);
   }

   if(env.lock_test){
      printf("LOCK_TEST--------------------------------------------------\n");
      printf("用户态自旋锁地址:%llu\n",(long long unsigned int)&spinlock);
      printf("用户态互斥锁地址:%llu , %llu\n",(long long unsigned int)&mutex1,(long long unsigned int)&mutex2);
      printf("用户态读写锁地址:%llu\n",(long long unsigned int)&rwlock);
      pthread_spin_init(&spinlock,PTHREAD_PROCESS_PRIVATE);
      pthread_mutex_init(&mutex1, NULL);
      pthread_mutex_init(&mutex2, NULL);
      pthread_rwlock_init(&rwlock,NULL);

      /*创建两个线程,分别争用自旋锁,互斥锁,读写锁*/
      pthread_t t1, t2;
      pthread_create(&t1, NULL, thread_lock_func1, NULL);
      pthread_create(&t2, NULL, thread_lock_func2, NULL);

      /*等待俩个线程运行结束*/
      pthread_join(t1, NULL);
      pthread_join(t2, NULL);

      // Destroy locks
      pthread_mutex_destroy(&mutex1);
      pthread_mutex_destroy(&mutex2);
      pthread_rwlock_destroy(&rwlock);
      pthread_spin_destroy(&spinlock);
      
      printf("\n");
   }

   if(env.resource_test){

   }

   if(env.syscall_test){
      printf("SYSCALL_TEST----------------------------------------------\n");

      // 系统调用序列逻辑(参考 UnixBench 中的 syscall 测试)
      printf("系统调用序列逻辑:\n");
      printf("每调用一次 SYS_getpid 系统调用睡眠 1 s,循环 10 次\n");
      int count = 10;
      while(count){
         syscall(SYS_getpid);
         count--;
         sleep(1);
      }
   }

   if(env.schedule_test){
    // 创建线程
    pthread_t threads[NUM_THREADS];
    int type_cpu =1;
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, &func,&type_cpu);
    }

    // 等待线程完成
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
   }

   return 0;
}
