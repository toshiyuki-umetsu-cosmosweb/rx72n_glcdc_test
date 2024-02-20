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
* File Name        : Config_TPU1_user.c
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
static uint8_t s_duty;
static uint8_t s_init_value;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_TPU1_Create_UserInit
* Description  : This function adds user code after initializing the TPU1 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_TPU1_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
    s_duty = 0u;
    s_init_value = 0u;
    TPU1.TGRA = TPU1.TGRB + 1u;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_TPU1_tgi1a_interrupt
* Description  : This function is TGI1A interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_TPU1_tgi1a_interrupt(void)
{
    /* Start user code for r_Config_TPU1_tgi1a_interrupt. Do not edit comment generated here */
    PORT6.PODR.BIT.B7 = 0;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_TPU1_tgi1b_interrupt
* Description  : This function is TGI1B interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_TPU1_tgi1b_interrupt(void)
{
    /* Start user code for r_Config_TPU1_tgi1b_interrupt. Do not edit comment generated here */
    PORT6.PODR.BIT.B7 = s_init_value;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
void R_Config_TPU1_SetPwmDuty(uint8_t duty)
{
    uint32_t period = (uint32_t)(TPU1.TGRB);
    uint32_t tgra = period * (uint32_t)(duty) / 255uL;
    if (tgra <= 0uL)
    {
        s_init_value = 0u;
        TPU1.TGRA = (uint16_t)(period + 1u); // Do not compare match.
    }
    else
    {
        s_init_value = 1u;
        if (tgra >= period)
        {
            TPU1.TGRA = (uint16_t)(period + 1u); // Do not compare match.
        }
        else
        {
            TPU1.TGRA = (uint16_t)(tgra);
        }
    }

    s_duty = duty;

    return ;
}
uint8_t R_Config_TPU1_GetPwmDuty(void)
{
    return s_duty;
}
/* End user code. Do not edit comment generated here */
