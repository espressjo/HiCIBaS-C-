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
   int cnt;
   uint8_t limits;
   double ra;
   double dec;
   int complete;
   char message[BUF_SIZE];
};

class shared_tcs
{
public:
    shared_tcs(uint shm_key);
    ~shared_tcs();
    struct tcs *shmp;
    void write_msg(std::string txt);
private:
	void clear_shared_memory();
    int get_shared_memory();
    int remove_shared_memory();
    size_t PAGE_SIZE;
    uint shm_key;
    int shmid;

};

#endif // SHARED_TCS_H