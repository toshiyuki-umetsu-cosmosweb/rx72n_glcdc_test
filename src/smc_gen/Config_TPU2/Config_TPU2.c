/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name        : Config_TPU2.c
* Component Version: 1.12.0
* Device(s)        : R5F572NDHxFB
* Description      : This file implements device driver for Config_TPU2.
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "Config_TPU2.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_TPU2_Create
* Description  : This function initializes the TPU2 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TPU2_Create(void)
{
    /* Release TPU channel 2 from stop state */
    MSTP(TPU2) = 0U;

    /* Stop TPU channel 2 counter */
    TPUA.TSTR.BIT.CST2 = 0U;

    /* Set TGI2A interrupt priority level */
    ICU.SLIBXR138.BYTE = 0x18U;
    IPR(PERIB, INTB138) = _02_TPU_PRIORITY_LEVEL2;

    /* TPU channel 2 is used as normal mode */
    TPUA.TSYR.BIT.SYNC2 = 0U;
    TPU2.TCR.BYTE = _02_TPU_PCLK_16 | _08_TPU_CKEG_IT_R | _20_TPU_CKCL_A;
    TPU2.TIER.BYTE = _01_TPU_TGIEA_ENABLE | _00_TPU_TGIEB_DISABLE | _00_TPU_TCIEV_DISABLE | _00_TPU_TTGE_DISABLE | 
                     _40_TPU_TIER_DEFAULT;
    TPU2.TIOR.BYTE = _00_TPU_IOA_DISABLE | _00_TPU_IOB_DISABLE;
    TPU2.TGRA = _0EA5_TGRA2_VALUE;
    TPU2.TGRB = _0176_TGRB2_VALUE;

    R_Config_TPU2_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_TPU2_Start
* Description  : This function starts the TPU2 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TPU2_Start(void)
{
    /* Enable TGI2A interrupt in ICU */
    IEN(PERIB, INTB138) = 1U;
    
    /* Start TPU channel 2 counter */
    TPUA.TSTR.BIT.CST2 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_TPU2_Stop
* Description  : This function stops the TPU2 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TPU2_Stop(void)
{
    /* Disable TGI2A interrupt in ICU */
    IEN(PERIB, INTB138) = 0U;
    
    /* Stop TPU channel 2 counter */
    TPUA.TSTR.BIT.CST2 = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
