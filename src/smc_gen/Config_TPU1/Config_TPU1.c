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
* File Name        : Config_TPU1.c
* Component Version: 1.12.0
* Device(s)        : R5F572NDHxFB
* Description      : This file implements device driver for Config_TPU1.
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
#include "Config_TPU1.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_TPU1_Create
* Description  : This function initializes the TPU1 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TPU1_Create(void)
{
    /* Release TPU channel 1 from stop state */
    MSTP(TPU1) = 0U;

    /* Stop TPU channel 1 counter */
    TPUA.TSTR.BIT.CST1 = 0U;

    /* Set TGI1A interrupt priority level */
    ICU.SLIBR144.BYTE = 0x14U;
    IPR(PERIB, INTB144) = _07_TPU_PRIORITY_LEVEL7;

    /* Set TGI1B interrupt priority level */
    ICU.SLIBXR135.BYTE = 0x15U;
    IPR(PERIB, INTB135) = _07_TPU_PRIORITY_LEVEL7;

    /* TPU channel 1 is used as normal mode */
    TPUA.TSYR.BIT.SYNC1 = 0U;
    TPU1.TCR.BYTE = _00_TPU_PCLK_1 | _40_TPU_CKCL_B;
    TPU1.TIER.BYTE = _01_TPU_TGIEA_ENABLE | _02_TPU_TGIEB_ENABLE | _00_TPU_TCIEV_DISABLE | _00_TPU_TTGE_DISABLE | 
                     _40_TPU_TIER_DEFAULT;
    TPU1.TIOR.BYTE = _00_TPU_IOA_DISABLE | _00_TPU_IOB_DISABLE;
    TPU1.TGRA = _176F_TGRA1_VALUE;
    TPU1.TGRB = _176F_TGRB1_VALUE;

    R_Config_TPU1_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_TPU1_Start
* Description  : This function starts the TPU1 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TPU1_Start(void)
{
    /* Enable TGI1A interrupt in ICU */
    IEN(PERIB, INTB144) = 1U;
    
    /* Enable TGI1B interrupt in ICU */
    IEN(PERIB, INTB135) = 1U;
    
    /* Start TPU channel 1 counter */
    TPUA.TSTR.BIT.CST1 = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_TPU1_Stop
* Description  : This function stops the TPU1 channel counter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TPU1_Stop(void)
{
    /* Disable TGI1A interrupt in ICU */
    IEN(PERIB, INTB144) = 0U;
    
    /* Disable TGI1B interrupt in ICU */
    IEN(PERIB, INTB135) = 0U;
    
    /* Stop TPU channel 1 counter */
    TPUA.TSTR.BIT.CST1 = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
