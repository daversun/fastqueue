#include <fastqueue.h>
#include <chrono>
#include <numeric>
#include <unistd.h>
#include <signal.h>
#define DATA_LEN 64
uint8_t data[DATA_LEN] = {"hello_world"};
uint32_t consumer_num = 6, producer_num = 6, p[64] = {0}, c[64] = {0};
FastQueue* fastqueue = NULL;
std::chrono::steady_clock::time_point start;
void sigInt(int signo){
    
    auto total_time = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count(); 
    uint64_t total_producer_objects = std::accumulate(p, p + producer_num, 0);
    uint64_t total_consumer_objects = std::accumulate(c, c + consumer_num, 0);
    uint64_t data_bytes = producer_num * total_producer_objects * sizeof(Object) * DATA_LEN;
   
    uint32_t page_size = sysconf(_SC_PAGESIZE);
    std::cout<<"----------------------------------------------------------------------------------------------------------------------------"<< std::endl; 
    std::cout<<"producer thread num:"<< producer_num <<" consumer thread num: " << consumer_num << std::endl << std::endl;
    std::cout<<"total_time: " << total_time << " seconds\n" << std::endl;
    std::cout<<"total_producer_num: " << total_producer_objects << " objects\n" << std::endl;
    std::cout<<"toatl_consumer_num: " << total_consumer_objects << " objects\n" << std::endl;
    std::cout<<"page_size:" << page_size << " bytes\n" << std::endl;
    std::cout<<"object_size:" << sizeof(Object)  + DATA_LEN << " bytes\n" << std::endl;
    std::cout<<"total_size:" << data_bytes << " bytes\n" << std::endl;
    
    std::cout<<"ops:" << (total_producer_objects + total_consumer_objects) * 1.0 / total_time << " operations/second\n" <<std::endl;
    std::cout<<"throught:" << data_bytes * 1.0 / total_time / 1024 / 1024 / 1024 << " G/second\n" << std::endl; 
    std::cout<<"----------------------------------------------------------------------------------------------------------------------------"<< std::endl; 
    exit(0);
}

void* producer_fun(void* params){

    Bucket* bucket = fastqueue->enqueue();
    Object obj(data, (int)DATA_LEN);
    
    uint64_t* num = (uint64_t*)params;
    bool flag;

    while(true){
        flag = bucket->store(obj);
        if(!flag){
            bucket->releaseBucket();
            bucket = fastqueue->enqueue();
            continue;
        }
	    (*num)++;
    }
   
}

void* consumer_fun(void* params){
    uint64_t* num = (uint64_t*)params;
    int index = 0;
    Bucket* bucket;

    while(true){
       
        bucket = fastqueue->dequeue();
        std::vector<Object*> objs = bucket->getObjs();
        //std::cout<< objs.size() << std::endl;
        for(int i = 0; i < objs.size(); ++i){
            (*num)++;
        }
        bucket->releaseBucket();    
   }
 
   
}

void performace_test(){
     uint32_t  bucket_num = 16, pages_per_bucket = 10;
    pthread_t producer[32], consumer[32];
    
    
    fastqueue = new FastQueue(bucket_num, bucket_num);
    
    start = std::chrono::steady_clock::now();


    for(int i = 0; i < producer_num; ++i){
        pthread_create(&producer[i], NULL, producer_fun, &p[i]);
    }

    for(int i = 0; i < consumer_num; ++i){
        pthread_create(&consumer[i], NULL, consumer_fun, &c[i]);
    }

    for(int i = 0; i < producer_num; ++i){
        pthread_join(producer[i], NULL);
    }

    for(int i = 0; i < consumer_num; ++i){
        pthread_join(consumer[i], NULL);
    }


}
int main(){

    signal(SIGINT, sigInt);
    performace_test();
    return 0;
}
