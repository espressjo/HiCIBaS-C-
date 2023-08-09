#ifndef HEATER_LOOP_H
#define HEATER_LOOP_H

#include "insthandle.h"
#include "uics.h"
#include "ljack.h"
#include "ui_config_file.h"
#include <iostream>
void heating_loop_t(instHandle *handle);
void heater_cmd(instHandle *handle,cmd *cc);


#endif

/*
    handle->tcs->tcs_tel->T1 = T.TTM;
    handle->tcs->tcs_tel->T2 = T.rm8;
    handle->tcs->tcs_tel->T3 = T.bench;
    handle->tcs->tcs_tel->T4 = T.nutec;
    std::this_thread::sleep_for (std::chrono::seconds(1));
*/
