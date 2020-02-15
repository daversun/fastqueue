# Background
## As we know, we study a single thread queue：
![single queue](https://images.gitee.com/uploads/images/2020/0215/132741_06d30e3d_5064364.jpeg)
## Single thread queue has two disadvantages:
```
1. If we want to support multi-thread, we need a lot of cost due to mutex
2. Even though a single queue support multi-thread, it can't support high concurrence and throught
```

# Sollution
## To solve the problems above, implement a high concurrence and throught <u>fastqueue</u>
![FastQueue](https://images.gitee.com/uploads/images/2020/0215/155747_b3fabb29_5064364.png)
## Main Idea
```
1. Using large page memory to reduce the times of memory access  
    As we know, os will buffer some items located in page tables in the TLB to reduce the times of memory access and reduce the latency of bus 

2. Using Coarse-grained lock to make parallelism      
    we use two kind lock: global lock and bucket lock. If we want to get a bucket, we first lock the global then lock the bucket. Once we get the bucket, we can insert many objects in the queue so that implement parallelism
```

## Performance Test
Now I just Test Some data where implement the thread bind core. In next time, I will gradually update the result

```
Environment: 
            Os: Centos7 3G MEMORY 
            CPU: 2 Processor and 2 core per Processor

Producer thread num:6 consumer thread num: 6

Total_time: 1.27152 seconds

Total_producer_num: 14217970 objects

Toatl_consumer_num: 14579091 objects

Page_size:4096 bytes

Object_size:80 bytes

Total_size:87355207680 bytes

Ops:2.26478e+07 operations/second

Throught:63.9832 G/second
```
    
