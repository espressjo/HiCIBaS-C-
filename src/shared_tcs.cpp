
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
	
	tcs_tel->limswitch=0;
	tcs_tel->devices=0;
	tcs_tel->alt = 0.0;
	tcs_tel->az = 0.0;
	tcs_tel->moteur_1 = 0;
	tcs_tel->moteur_2 = 0;
	tcs_tel->T1 = 0;
	tcs_tel->T2 = 0;
	tcs_tel->T3 = 0;
	tcs_tel->T4 = 0;
	tcs_tel->T5 = 0;
	tcs_tel->T6 = 0;
	tcs_tel->H1 = 0;
	tcs_tel->H2 = 0;
	tcs_tel->H3 = 0;
	tcs_tel->H4 = 0;
	tcs_tel->H5 = 0;
	tcs_tel->H6 = 0;
	tcs_tel->RA = 0;
	tcs_tel->DEC = 0;
	tcs_tel->r_scripts = 0;
	tcs_tel->s_scripts = 0;
	tcs_tel->moteur = 0;
	
}
int shared_tcs::get_shared_memory()
{

    shmid = shmget(static_cast<int>(shm_key), PAGE_SIZE, 0644|IPC_CREAT);
    if (shmid == -1) {
          perror("Shared memory");
          return 1;
       }
    tcs_tel = (telemetry*)shmat(shmid, NULL, 0);
    if (tcs_tel == (void *) -1) {
          perror("Shared memory attach");
          return 1;
       }
    return 0;
}
int shared_tcs::remove_shared_memory()
{
    if (shmdt(tcs_tel) == -1) {
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

