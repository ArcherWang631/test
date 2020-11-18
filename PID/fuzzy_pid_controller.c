/**
 ******************************************************************************
 * @file    fuzzy_pid_controller.c
 * @author  ArcherWang
 * @version V1.2.2
 * @date    20-July-2020
 * @brief
 ******************************************************************************
 * @attention
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "fuzzy_pid_controller.h"



/* KP-模糊规则表 */
FuzzyType_e FUZZY_TAB_KP[V_MAX][V_MAX]={
    V_PB,  V_PB,  V_PM,  V_PM, V_PS,  V_ZO,  V_ZO,
    V_PB,  V_PB,  V_PM,  V_PM, V_PS,  V_ZO,  V_ZO,
    V_PB,  V_PB,  V_PM,  V_PM, V_PS,  V_ZO,  V_ZO,
    V_PB,  V_PB,  V_PM,  V_PM, V_PS,  V_ZO,  V_ZO,
    V_PB,  V_PB,  V_PM,  V_PM, V_PS,  V_ZO,  V_ZO,
    V_PB,  V_PB,  V_PM,  V_PM, V_PS,  V_ZO,  V_ZO,
    V_PB,  V_PB,  V_PM,  V_PM, V_PS,  V_ZO,  V_ZO
};

/* KI-模糊规则表 */
FuzzyType_e FUZZY_TAB_KI[V_MAX][V_MAX]={
    V_PB,  V_PB,  V_PM,  V_PM, V_PS,  V_ZO,  V_ZO,
    V_PB,  V_PB,  V_PM,  V_PM, V_PS,  V_ZO,  V_ZO,
    V_PB,  V_PB,  V_PM,  V_PM, V_PS,  V_ZO,  V_ZO,
    V_PB,  V_PB,  V_PM,  V_PM, V_PS,  V_ZO,  V_ZO,
    V_PB,  V_PB,  V_PM,  V_PM, V_PS,  V_ZO,  V_ZO,
    V_PB,  V_PB,  V_PM,  V_PM, V_PS,  V_ZO,  V_ZO,
    V_PB,  V_PB,  V_PM,  V_PM, V_PS,  V_ZO,  V_ZO
};

/* KD-模糊规则表 */
FuzzyType_e FUZZY_TAB_KD[V_MAX][V_MAX]={
    V_PB,  V_PB,  V_PM,  V_PM, V_PS,  V_ZO,  V_ZO,
    V_PB,  V_PB,  V_PM,  V_PM, V_PS,  V_ZO,  V_ZO,
    V_PB,  V_PB,  V_PM,  V_PM, V_PS,  V_ZO,  V_ZO,
    V_PB,  V_PB,  V_PM,  V_PM, V_PS,  V_ZO,  V_ZO,
    V_PB,  V_PB,  V_PM,  V_PM, V_PS,  V_ZO,  V_ZO,
    V_PB,  V_PB,  V_PM,  V_PM, V_PS,  V_ZO,  V_ZO,
    V_PB,  V_PB,  V_PM,  V_PM, V_PS,  V_ZO,  V_ZO
};






Fuzzy_PID_Handle_t Fuzzy_PID={
    .sE={
        .Super={
            .hStart=-2000,
            .hStop=2000,
        },
    },
    .sEC={
        .Super={
            .hStart=-3000,
            .hStop=3000,
        },
    },
    .FuzzyKp={
        .hStart=-100,
        .hStop=100,
    },
    .FuzzyKi={
        .hStart=-100,
        .hStop=100,
    },
    .FuzzyKd={
        .hStart=-100,
        .hStop=100,
    },
};



void FuzzyBase_Init(FuzzyBase_Handle_t *pHandle)
{
    pHandle->hRange = pHandle->hStop - pHandle->hStart;
    
    pHandle->hInterval = pHandle->hRange / (V_MAX-1);
    
    for (int n = 0; n < V_MAX; ++n)
    {
        if (n == (V_MAX - 1))
        {
            pHandle->aSector[n] = pHandle->hStart + n * (pHandle->hInterval) + pHandle->hRange % (V_MAX-1);
        }
        else
        {
            pHandle->aSector[n] = pHandle->hStart + n * (pHandle->hInterval);
        }
    }
}

void Fuzzy_Init(Fuzzy_Handle_t *pHandle)
{
    FuzzyBase_Handle_t *pSuper = &pHandle->Super;
    
    FuzzyBase_Init(pSuper);
    
    pHandle->hTriSlope = (int16_t)(FUZZY_Q15_MAX / pSuper->hInterval);
    
    pHandle->sData.eLowerType  = V_MAX;
    pHandle->sData.eUpperType  = V_MAX;
    
    pHandle->sData.hLowerRatio = 0;
    pHandle->sData.hUpperRatio = 0;
    
}


void Fuzzy_PID_Init(Fuzzy_PID_Handle_t *pHandle)
{
    Fuzzy_Init(&pHandle->sE);
    Fuzzy_Init(&pHandle->sEC);
    FuzzyBase_Init(&pHandle->FuzzyKp);
    FuzzyBase_Init(&pHandle->FuzzyKi);
    FuzzyBase_Init(&pHandle->FuzzyKd);
}



Fuzzy_Data_t Fuzzy_Speculate(Fuzzy_Handle_t *pHandle, int16_t hVar)
{
    uint8_t bIndex;
    int32_t wTmpValue;
    
    FuzzyBase_Handle_t *pSuper = &pHandle->Super;
    
    if (hVar <= pSuper->aSector[V_NB])
    {
        hVar = pSuper->aSector[V_NB];
    }
    else if (hVar >= pSuper->aSector[V_PB])
    {
        hVar = pSuper->aSector[V_PB];
    }
    
    bIndex = (hVar-pSuper->aSector[0]) / (pSuper->hInterval);
    
    if (bIndex >= V_PB)
    {
        bIndex = V_PB - 1;
    }
    
    pHandle->sData.eLowerType= (FuzzyType_e)bIndex;
    pHandle->sData.eUpperType= (FuzzyType_e)(bIndex + 1);
    
    wTmpValue = (int32_t)(hVar - pSuper->aSector[bIndex]) * (int32_t)(pHandle->hTriSlope);
    
    if (wTmpValue > INT16_MAX)
    {
        pHandle->sData.hUpperRatio = INT16_MAX;
        pHandle->sData.hLowerRatio = 0;
    }
    else if(wTmpValue==0)
    {
        pHandle->sData.hUpperRatio = 0;
        pHandle->sData.hLowerRatio = INT16_MAX;
    }
    else
    {
        pHandle->sData.hUpperRatio=wTmpValue;
        pHandle->sData.hLowerRatio=FUZZY_Q15_MAX-wTmpValue;
    }
    
    return pHandle->sData;
    
}




int main()
{
    
    Fuzzy_PID_Init(&Fuzzy_PID);
    
    
    return 0;
}
