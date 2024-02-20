/***********************************************************************
*
*  FILE        : main.c
*  DATE        : 2024-02-19
*  DESCRIPTION : Main Program
*
*  NOTE:THIS IS A TYPICAL EXAMPLE.
*
***********************************************************************/
#include "r_smc_entry.h"
#include "led.h"
#include "backlight_ctrl.h"
#include "lcd_ctrl.h"
#include "utils.h"
#include "command_io.h"

void main(void);
static void init_peripherals(void);

/**
 * アプリケーションのエントリポイント
 */
void main(void)
{
    init_peripherals();
    led_set(false);
    command_io_init();
    lcd_ctrl_init();
    uint32_t proc_tick = get_tick_count();
    while (1) {
        command_io_update(); // コマンドがあったら処理する
        uint32_t now = get_tick_count();
        if ((now - proc_tick) >= 1000) {
            proc_tick = now;
        }
    }

    lcd_ctrl_fini();
    command_io_fini();

    return ;
}


/**
 * @brief ペリフェラルを初期化する。
 */
static void init_peripherals(void) {
    R_Config_ELC_Start(); // ELC開始
    R_Config_CMTW0_Start(); // タイマーカウンタ開始
    R_Config_TPU0_Start(); // プリスケーラ開始

    backlight_ctrl_init();

    return ;
}
