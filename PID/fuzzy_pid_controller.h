/**
  ******************************************************************************
  * @file    fuzzy_pid_controller.h
  * @author  ArcherWang
  * @version V1.2.2
  * @date    20-July-2020
  * @brief    
  ******************************************************************************
  * @attention
  *
  */

/* Includes ------------------------------------------------------------------*/

#include "stdint.h"
#include "stdlib.h"

#define FUZZY_Q15_MAX ((int32_t)(32768))
#define FUZZY_Q15_POW 15

/* 隶属度值 */
typedef enum
{
    V_NB = 0x0,
    V_NM,
    V_NS,
    V_ZO,
    V_PS,
    V_PM,
    V_PB,
    V_MAX,
} FuzzyType_e;

typedef struct
{
    FuzzyType_e  eLowerType;
    FuzzyType_e  eUpperType;
    int16_t      hLowerRatio;
    int16_t      hUpperRatio;
}Fuzzy_Data_t;


/* 模糊表 */
typedef struct
{
    int16_t hStart;
    int16_t hStop;
    uint16_t hRange;
    int16_t hInterval;
    int16_t aSector[V_MAX];
} FuzzyBase_Handle_t;

/* 模糊逻辑 */
typedef struct
{
    FuzzyBase_Handle_t Super;
    int16_t hTriSlope;
    Fuzzy_Data_t   sData;
} Fuzzy_Handle_t;

/* 模糊逻辑 */
typedef struct
{
    Fuzzy_Handle_t sE;
    Fuzzy_Handle_t sEC;
    FuzzyBase_Handle_t FuzzyKp;
    FuzzyBase_Handle_t FuzzyKi;
    FuzzyBase_Handle_t FuzzyKd;

} Fuzzy_PID_Handle_t;

void FuzzyBase_Init(FuzzyBase_Handle_t *pHandle);
void Fuzzy_Init(Fuzzy_Handle_t *pHandle);
