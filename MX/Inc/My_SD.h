#ifndef _MY_SD_H
#define _MY_SD_H

#include "main.h"

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "ff.h"

void SDIO_ReadDiv();
void SDIO_WriteDiv();
FRESULT scan_files (
    char* path        /* Start node to be scanned (***also used as work area***) */
);
#endif