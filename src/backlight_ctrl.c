/**
 * @file LCDバックライト制御インタフェース定義
 * @author Cosmosweb Co.,Ltd. 2024
 */
#include <r_smc_entry.h>
#include "backlight_ctrl.h"
/**
 * @brief PWM Duty 要求値
 */
static uint8_t s_required_duty;
/**
 * @brief PWM Duty 実際の適用値
 *        1msec毎に、backlight_ctrl_update()にて s_required_duty になるように1ずつ変更する。
 */
static uint8_t s_actual_duty;

/**
 * @brief バックライト制御出力を初期化する。
 */
void backlight_ctrl_init(void)
{
    s_required_duty = 0u;
    s_actual_duty = 0u;
    PORT6.PODR.BIT.B7 = 0u; // back light = OFF.

    R_Config_TPU1_SetPwmDuty(s_actual_duty);
    R_Config_TPU1_Start(); // TPU1 (LCD back light control)動作開始
    R_Config_TPU2_Start(); // TPU2 (1msec interval process)動作開始

    return ;
}

/**
 * @brief バックライト制御出力を終了する。
 */
void backlight_ctrl_fini(void)
{
    R_Config_TPU2_Stop();
    R_Config_TPU1_Stop();
    PORT6.PODR.BIT.B7 = 0u; // back light = OFF.

    return ;
}

/**
 * @brief バックライト制御出力を更新する。
 */
void backlight_ctrl_update(void)
{
    if (s_actual_duty != s_required_duty)
    {
        if (s_actual_duty < s_required_duty) // 現在の設定値 < 要求値？
        {
            s_actual_duty++;
        }
        else if (s_actual_duty > s_required_duty) // 現在の設定値 > 要求値？
        {
            s_actual_duty--;
        }
        R_Config_TPU1_SetPwmDuty(s_actual_duty);
    }

    return ;
}

/**
 * @brief バックライトのPWM値を設定する。
 *        dutyが設定可能値範囲外の場合には何もしない。
 * @param duty PWM値(0-255)
 */
void backlight_ctrl_set_duty(int duty)
{
    if ((duty >= 0) && (duty <= 255))
    {
        s_required_duty = (uint8_t)(duty);
    }

    return ;
}

/**
 * @brief バックライトのPWM値を取得する。
 * @return PWM値(0-255)
 */
int backlight_ctrl_get_duty(void)
{
    return (int)(s_required_duty);
}

