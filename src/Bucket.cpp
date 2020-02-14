#include <bucket.h>
#include <string.h>
#include <vector>
#include <object.hpp>
#include <iostream>


Bucket::Bucket(uint8_t* base, uint32_t bucket_size):base(base), bucket_size(bucket_size){
    current = base;
    pthread_mutex_init(&bucket_mutex, NULL);
}

pthread_mutex_t& Bucket::get_mutex(){
    return bucket_mutex;
}

void Bucket::clear(){
    current = base;
    objs.clear();
}

bool Bucket::store(const Object& obj){
    
;
    uint32_t space =bucket_size - (current - base);

    if(space < sizeof(Object) + obj.len){
        return false;
    }

    Object* store_obj = new(current)Object(NULL, obj.len); 
    memcpy(current + sizeof(Object), obj.addr, obj.len);
    store_obj->addr = current + sizeof(Object);
    current = current + sizeof(Object) + obj.len;
    objs.push_back(store_obj);
    return true;
}

std::vector<Object*>& Bucket::getObjs(){
    return objs;
}

Object* Bucket::getObj(int index){
    if(index < objs.size())return objs[index];
    else return NULL;
}

void Bucket::releaseBucket(){
    pthread_mutex_unlock(&bucket_mutex);
}



