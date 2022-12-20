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
	shmp->cnt=0;
	shmp->complete = 0;
	shmp->launch_lim_switch;
	shmp->rm_left_lim=1;
	shmp->rm_right_lim=1;
	shmp->tmax6_upper_lim=1;
	shmp->tmax6_lower_lim=1;
	shmp->xzero_lim=1;
	shmp->yzero_lim=1;
	shmp->dec = 0.0;
	shmp->ra = 0.0;
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

void shared_tcs::write_msg(std::string txt)
{
    memset(shmp->message,0,1024);
    if (txt.size()>=1024)
    {   int i=0;
        for (auto &c:txt)
        {
            shmp->message[i] = c;
            i++;
        }
        shmp->cnt = 1024;
    }
    else {
        shmp->cnt = txt.size();
        strcpy(shmp->message,txt.c_str());
    }
}