#include <vector>
#include <stdio.h>
#include <pthread.h>
#include <object.hpp>
#include <pthread.h>

#ifndef BUCKET
#define BUCKET

class Bucket{
	private:
		uint32_t bucket_size;
		uint8_t* base, *current;							// the base address of bucket in the QQ
		pthread_mutex_t bucket_mutex; 					// the bucket's mutex
		std::vector<Object*> objs;						// store obj address	
		
	public:
		Bucket(uint8_t* base, uint32_t bucket_size);	// init Bucket according the given address
		bool store(const Object& obj);
		std::vector<Object*>& getObjs();
		void clear();
		Object* getObj(int index);
		void releaseBucket();
        pthread_mutex_t& get_mutex();
		~Bucket();
};

#endif