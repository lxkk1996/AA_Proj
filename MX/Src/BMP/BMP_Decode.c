#include "BMP_Decode.h"

uint8_t BMP_Chack(BMP_HEARD* bh)
{
    if(bh->fh.fType != 0x4D42) return 1;
    else if(bh->ih.iSize != 40) return 2;
    else if(bh->ih.iCompression != 0) return 3;
    else if((bh->ih.iBitCount != 1) && (bh->ih.iBitCount != 16)) return 4;
    else if(bh->ih.iHeight < 0) return 5;
    return 0;
}

uint16_t BMP_GetImageDepth(BMP_HEARD* bh)
{
    return bh->ih.iBitCount;
}

uint8_t BMP_GetScan(BMP_HEARD* bh)
{
    if(bh->ih.iHeight > 0) return 1;
    else return 0;
}

uint32_t BMP_GetOffSet(BMP_HEARD* bh)
{
    return bh->fh.fOffSet;
}

int32_t BMP_GetHigh(BMP_HEARD* bh)
{
    return bh->ih.iHeight;
}

uint32_t BMP_GetWidth(BMP_HEARD* bh)
{
    return bh->ih.iWidth;
}