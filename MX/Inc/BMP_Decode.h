#ifndef _BMP_DECODE_H
#define _BMP_DECODE_H

#include "stdint.h"

#pragma pack(2)
typedef struct
{
    uint16_t fType;      //0000h 2Bytes������Ϊ"BM"����0x424D ����Windowsλͼ�ļ�
    uint32_t fSize;      //0002h 4Bytes������BMP�ļ��Ĵ�С
    uint16_t fRes1;      //0006h 2Bytes��������Ϊ0
    uint16_t fRes2;      //0008h 2Bytes��������Ϊ0
    uint32_t fOffSet;    //000Ah 4Bytes���ļ���ʼλ�õ�ͼ���������ݵ��ֽ�ƫ����
}BMP_FILE_HEADER;

#pragma pack(2)
typedef struct
{
    uint32_t    iSize;           //000Eh 4Bytes��INFOHEADER�ṹ���С�����������汾INFOHEADER����������
    uint32_t    iWidth;          //0012h 4Bytes��ͼ���ȣ�������Ϊ��λ��
    int32_t     iHeight;         //0016h 4Bytes��ͼ��߶ȣ�+��ͼ��洢˳��ΪBottom2Top��-��Top2Bottom
    uint16_t    iPlanes;         //001Ah 2Bytes��ͼ������ƽ�棬BMP�洢RGB���ݣ������Ϊ1
    uint16_t    iBitCount;       //001Ch 2Bytes��ͼ������λ��1, 4, 8, 16, 32
    uint32_t    iCompression;    //001Eh 4Bytes��0����ѹ����1��RLE8��2��RLE4...
    uint32_t    iSizeImage;      //0022h 4Bytes��4�ֽڶ����ͼ�����ݴ�С
    int32_t     iXPelsPerMeter;  //0026h 4Bytes��������/�ױ�ʾ��ˮƽ�ֱ���
    int32_t     iYPelsPerMeter;  //002Ah 4Bytes��������/�ױ�ʾ�Ĵ�ֱ�ֱ���
    uint32_t    iClrUsed;        //002Eh 4Bytes��ʵ��ʹ�õĵ�ɫ����������0��ʹ�����еĵ�ɫ������
    uint32_t    iClrImportant;   //0032h 4Bytes����Ҫ�ĵ�ɫ����������0�����еĵ�ɫ����������Ҫ
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