#include <pthread.h>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <bucket.h>
#ifndef FASTQUEUE
#define FASTQUEUE

class FastQueue{
	private:
		uint8_t* base;									// base address of QQ
		uint32_t num_buckets;							// total buckets in QQ queue
		uint32_t pages_per_bucket;						// pages_num per bucket
		uint32_t head, tail;							// the head, tail pointer of QQ 
		pthread_mutex_t global_mutex;  					// global mutex
		uint32_t distance;								// the time of dequeue
		std::vector<Bucket*> buckets;					// the all buckets allocated
		pthread_cond_t full_cond, empty_cond;			// the communication between threads
	public:
		FastQueue(uint32_t num_buckets, uint32_t pages_per_bucket);
		Bucket* dequeue();
		Bucket* enqueue();
		bool full();
		bool empty();
		~FastQueue();

};

#endif










