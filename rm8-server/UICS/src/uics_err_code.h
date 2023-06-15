#ifndef UICS_ERR_CODE_H
#define UICS_ERR_CODE_H

typedef enum {
    uicsCMD_ERR_UNKNOWN_KEYW = 1,
    uicsCMD_ERR_VALUE,
    uicsCMD_ERR_PARAM_FORMAT,
    uicsCMD_ERR_PARAM_VALUE,
    uicsCMD_ERR_NOT_SUPPORTED,
    uicsCMD_ERR_PARAM_UNKNOWN,
}uics_cmd_err;
#endif // UICS_ERR_CODE_H
