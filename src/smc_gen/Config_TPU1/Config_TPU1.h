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
* File Name        : Config_TPU1.h
* Component Version: 1.12.0
* Device(s)        : R5F572NDHxFB
* Description      : This file implements device driver for Config_TPU1.
***********************************************************************************************************************/

#ifndef CFG_Config_TPU1_H
#define CFG_Config_TPU1_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_tpu.h"

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define TPU1_PCLK_COUNTER_DIVISION         (1)
#define _176F_TGRA1_VALUE                  (0x176FU) /* TGRA1 value */
#define _176F_TGRB1_VALUE                  (0x176FU) /* TGRB1 value */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_Config_TPU1_Create(void);
void R_Config_TPU1_Create_UserInit(void);
void R_Config_TPU1_Start(void);
void R_Config_TPU1_Stop(void);
/* Start user code for function. Do not edit comment generated here */
void R_Config_TPU1_SetPwmDuty(uint8_t duty);
uint8_t R_Config_TPU1_GetPwmDuty(void);
/* End user code. Do not edit comment generated here */
#endif
