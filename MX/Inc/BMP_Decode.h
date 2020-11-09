#ifndef _BMP_DECODE_H
#define _BMP_DECODE_H

#include "stdint.h"

#pragma pack(2)
typedef struct
{
    uint16_t fType;      //0000h 2Bytes，必须为"BM"，即0x424D 才是Windows位图文件
    uint32_t fSize;      //0002h 4Bytes，整个BMP文件的大小
    uint16_t fRes1;      //0006h 2Bytes，保留，为0
    uint16_t fRes2;      //0008h 2Bytes，保留，为0
    uint32_t fOffSet;    //000Ah 4Bytes，文件起始位置到图像像素数据的字节偏移量
}BMP_FILE_HEADER;

#pragma pack(2)
typedef struct
{
    uint32_t    iSize;           //000Eh 4Bytes，INFOHEADER结构体大小，存在其他版本INFOHEADER，用作区分
    uint32_t    iWidth;          //0012h 4Bytes，图像宽度（以像素为单位）
    int32_t     iHeight;         //0016h 4Bytes，图像高度，+：图像存储顺序为Bottom2Top，-：Top2Bottom
    uint16_t    iPlanes;         //001Ah 2Bytes，图像数据平面，BMP存储RGB数据，因此总为1
    uint16_t    iBitCount;       //001Ch 2Bytes，图像像素位数1, 4, 8, 16, 32
    uint32_t    iCompression;    //001Eh 4Bytes，0：不压缩，1：RLE8，2：RLE4...
    uint32_t    iSizeImage;      //0022h 4Bytes，4字节对齐的图像数据大小
    int32_t     iXPelsPerMeter;  //0026h 4Bytes，用象素/米表示的水平分辨率
    int32_t     iYPelsPerMeter;  //002Ah 4Bytes，用象素/米表示的垂直分辨率
    uint32_t    iClrUsed;        //002Eh 4Bytes，实际使用的调色板索引数，0：使用所有的调色板索引
    uint32_t    iClrImportant;   //0032h 4Bytes，重要的调色板索引数，0：所有的调色板索引都重要
}BMP_INFO_HEARD;

typedef struct
{
    union
    {
        uint32_t RGBColor;
        struct
        {
            uint8_t rgbBlue;
            uint8_t rgbGreen;
            uint8_t rgbRed;
            uint8_t rgbRes;
        };
    };
}BMP_PALETTE;

typedef struct
{
    BMP_FILE_HEADER fh;
    BMP_INFO_HEARD ih;
}BMP_HEARD;//54 Byte

uint8_t BMP_Chack(BMP_HEARD* bh);
uint16_t BMP_GetImageDepth(BMP_HEARD* bh);
uint8_t BMP_GetScan(BMP_HEARD* bh);
uint32_t BMP_GetOffSet(BMP_HEARD* bh);
#endif