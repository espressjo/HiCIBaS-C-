#include "shared_telemetry.h"

shared_telemetry::shared_telemetry(uint shm_key)
{
    shared_telemetry::shm_key = shm_key;
    PAGE_SIZE = static_cast<size_t>(getpagesize());
    get_shared_memory();
	clear_shared_memory();
}
shared_telemetry::~shared_telemetry()
{
    remove_shared_memory();
}
void shared_telemetry::clear_shared_memory()
{
	 shmp->connected=false;
	 shmp->limswitch=0;
	 shmp->cam1=false;
	 shmp->cam2=false;
	 shmp->cam3=false;
	 shmp->TTM=false;
	 shmp->source_calibration=false;
	 shmp->alt_moving=false;
	 shmp->az_moving=false;
	 shmp->az=0.0;
	 shmp->alt=0.0;
	 shmp->moteur_1=0;
	 shmp->moteur_2=0;
	 shmp->T1=0;
	 shmp->T2=0;
	 shmp->T3=0;
	 shmp->T4=0;
	 shmp->T5=0;
	 shmp->T6=0;
	 shmp->H1=0;
	 shmp->H2=0;
	 shmp->H3=0;
	 shmp->H4=0;
	 shmp->H5=0;
	 shmp->H6=0;
	 shmp->RA=0.0;
	 shmp->scripts=0;
	 shmp->DEC=0.0;
}
int shared_telemetry::get_shared_memory()
{

    shmid = shmget(static_cast<int>(shm_key), PAGE_SIZE, 0644|IPC_CREAT);
    if (shmid == -1) {
          perror("Shared memory");
          return 1;
       }
    shmp = (shm_telemetry*)shmat(shmid, NULL, 0);
    if (shmp == (void *) -1) {
          perror("Shared memory attach");
          return 1;
       }
    return 0;
}
int shared_telemetry::remove_shared_memory()
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

