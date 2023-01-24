#ifndef SHARED_TCS_H
#define SHARED_TCS_H

#include <string>
#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#define BUF_SIZE 1024

struct tcs {
   uint8_t limits;//<0> upper, <1> lower , <2> right, <3> left, <4> launch, <5> AZ-0, <6> ALT-0
   uint8_t devices;//<0> alt moving, <1> az moving, //carfull in python, a struct in C is 8-bytes align
   double alt;
   double az;
};

class shared_tcs
{
public:
    shared_tcs(uint shm_key);
    ~shared_tcs();
    struct tcs *shmp;
private:
	void clear_shared_memory();
    int get_shared_memory();
    int remove_shared_memory();
    size_t PAGE_SIZE;
    uint shm_key;
    int shmid;

};

#endif // SHARED_TCS_H