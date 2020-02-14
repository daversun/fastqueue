#include <fastqueue.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

FastQueue::FastQueue(uint32_t num_buckets, uint32_t pages_per_bucket):num_buckets(num_buckets), pages_per_bucket(pages_per_bucket), distance(distance){
    
    

    // get the page_size of current system
    uint32_t huge_page_size = sysconf(_SC_PAGESIZE);
    
    if ((base = (uint8_t*) mmap(NULL, pages_per_bucket * num_buckets * huge_page_size,
                                      PROT_WRITE | PROT_READ,
                                      MAP_PRIVATE| MAP_ANONYMOUS, -1, 0)) == MAP_FAILED) {
                perror("mmap error");
                exit(0);
    }
    

    head = tail = 0;
    pthread_mutex_init(&global_mutex, NULL);
    pthread_cond_init(&full_cond, NULL);
    pthread_cond_init(&empty_cond, NULL);

    for(int i = 0; i < num_buckets; ++i){
         buckets.push_back(new Bucket(base + i * pages_per_bucket * huge_page_size, pages_per_bucket * huge_page_size));
         //printf("%d\n", base + i * pages_per_bucket * huge_page_size);
    }
}

bool FastQueue::empty(){
    if(head == tail)return true;
    return false;
}

bool FastQueue::full(){
    if((tail + 1) % num_buckets == head){
        return true;
    }
    return false;
}

Bucket* FastQueue::enqueue(){
    int bucket_index;
    pthread_mutex_lock(&global_mutex);                         //lock global mutex to avoid  
    
    while(full()){
        pthread_cond_wait(&full_cond, &global_mutex);          //the thread block, if the queue is full
    }
    
    bucket_index = tail;
  
    pthread_mutex_lock(&(buckets[tail]->get_mutex()));           //lock the bukcet to avoid the comprtition between enqueue and dequeue threads
    
    tail = (tail + 1) % num_buckets;
    
    pthread_mutex_unlock(&global_mutex);                       //Here, may lead long time block, if the bucket is not consumed.
    
    pthread_cond_signal(&empty_cond);

    buckets[bucket_index]->clear();
    
    return buckets[bucket_index];
}

Bucket* FastQueue::dequeue(){
    int bucket_index;
    pthread_mutex_lock(&global_mutex);                          //lock global mutex to avoid  
    
    while(empty()){
        pthread_cond_wait(&empty_cond, &global_mutex);          //the thread block if the queue is empty
    }
    
    
    bucket_index = head;
    
    pthread_mutex_lock(&(buckets[head]->get_mutex()));            //lock the bukcet to avoid the comprtition between enqueue and dequeue threads
  
    head = (head + 1) % num_buckets;
    
    pthread_mutex_unlock(&global_mutex);                        //Here, may lead long time block, if the bucket is not consumed.
    
    pthread_cond_signal(&full_cond);

 
    return buckets[bucket_index];
}



FastQueue::~FastQueue(){
    uint32_t huge_page_size = sysconf(_SC_PAGESIZE);
    if (munmap(base, pages_per_bucket * num_buckets * huge_page_size)){
        perror("munmap");
        exit(0);
    }
}