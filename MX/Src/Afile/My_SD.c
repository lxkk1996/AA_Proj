#include "My_SD.h"

void SDIO_ChangeDiv(uint32_t Div)
{
    uint32_t tmpreg = 0;
      /* Set SDMMC configuration parameters */
    tmpreg |= (SDIO_CLOCK_EDGE_RISING           |\
               SDIO_CLOCK_BYPASS_DISABLE         |\
               SDIO_CLOCK_POWER_SAVE_DISABLE      |\
               SDIO_BUS_WIDE_4B             |\
               SDIO_HARDWARE_FLOW_CONTROL_ENABLE |\
               Div
               ); 
    
    /* Write to SDMMC CLKCR */
    MODIFY_REG(SDIO->CLKCR, CLKCR_CLEAR_MASK, tmpreg);
}

void SDIO_ReadDiv()
{
    SDIO_ChangeDiv(0);
}

void SDIO_WriteDiv()
{
    SDIO_ChangeDiv(2);
}

FRESULT scan_files (
    char* path        /* Start node to be scanned (***also used as work area***) */
)
{
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;


    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
                res = scan_files(path);                    /* Enter the directory */
                if (res != FR_OK) break;
                path[i] = 0;
            } else {                                       /* It is a file. */
                printf("%s/%s\n", path, fno.fname);
            }
        }
        f_closedir(&dir);
    }

    return res;
}