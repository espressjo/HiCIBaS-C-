#include "shared_tcs.h"

shared_tcs::shared_tcs(uint shm_key)
{
    shared_tcs::shm_key = shm_key;
    PAGE_SIZE = static_cast<size_t>(getpagesize());
    get_shared_memory();
	clear_shared_memory();
}
shared_tcs::~shared_tcs()
{
    remove_shared_memory();
}
void shared_tcs::clear_shared_memory()
{
	shmp->limits=0;
	shmp->devices=0;
	shmp->alt = 0.0;
	shmp->az = 0.0;
	//memcpy(shmp->message,0,BUF_SIZE);
}
int shared_tcs::get_shared_memory()
{

    shmid = shmget(static_cast<int>(shm_key), PAGE_SIZE, 0644|IPC_CREAT);
    if (shmid == -1) {
          perror("Shared memory");
          return 1;
       }
    shmp = (tcs*)shmat(shmid, NULL, 0);
    if (shmp == (void *) -1) {
          perror("Shared memory attach");
          return 1;
       }
    return 0;
}
int shared_tcs::remove_shared_memory()
{
    if (shmdt(shmp) == -1) {
          perror("shmdt");
          return 1;
       }
        //remove / destroy segment after last user disconnect
       if (shmctl(shmid, IPC_RMID, 0) == -1) {
          perror("shmctl");
          return 1;
       }
   return 0;
}

