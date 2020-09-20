//
//  mc_usart.c
//  test
//
//  Created by archer on 2020/8/17.
//  Copyright © 2020 apple. All rights reserved.
//

#include "mc_usart.h"

#define FRAME_SOF  '<'
#define FRAME_END  '>'


const char ASCII_TABLE[]="0123456789ABCDEF";


char* U16ToChar(uint16_t val,char *pMsg)
{
    *pMsg++=ASCII_TABLE[(val>>12) & 0x0F];
    *pMsg++=ASCII_TABLE[(val>>8) & 0x0F];
    *pMsg++=ASCII_TABLE[(val>>4) & 0x0F];
    *pMsg++=ASCII_TABLE[(val) & 0x0F];
    
    return pMsg;
    
}

char * U8ToChar(uint8_t val,char *pMsg)
{
    *pMsg++=ASCII_TABLE[(val>>4) & 0x0F];
    *pMsg++=ASCII_TABLE[(val) & 0x0F];
    return pMsg;
}

char* DataToChar(char *pData,char *pMsg,uint16_t size)
{
    for(int n=0;n<size;++n)
    {
        *pMsg++=ASCII_TABLE[pData[n]>>4 & 0x0F];
        *pMsg++=ASCII_TABLE[pData[n] & 0x0F];
    }
    return pMsg;
}


void DataPack(char bCmd,char bPara,char *pMsg,char *pData,uint8_t size)
{
    uint8_t CRC=0x00;
    *pMsg++=FRAME_SOF;
    *pMsg++=bCmd;
    *pMsg++=bPara;
    *pMsg++=ASCII_TABLE[(size>>4) & 0x0F];
    *pMsg++=ASCII_TABLE[(size) & 0x0F];
    
    for(int n=0;n<size;++n)
    {
        *pMsg++=ASCII_TABLE[pData[n]>>4 & 0x0F];
        *pMsg++=ASCII_TABLE[pData[n] & 0x0F];
    }
    
    //需要使用c生成CRC校验码
    *pMsg++=FRAME_SOF;
    
    
}



int main()
{
    
    return 0;
}
