/**
 * @file LCDコントロールコマンド定義
 * @author Cosmosweb Co.,Ltd. 2024
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "backlight_ctrl.h"
#include "lcd_ctrl.h"
#include "command_lcd.h"

/**
 * @brief backlight コマンドを処理する。
 * @param ac 引数の数
 * @param av 引数配列
 */
void cmd_backlight(int ac, char **av)
{
    if (ac >= 2)
    {
        int duty = strtol(av[1], NULL, 0);
        if ((duty < 0) || (duty > 255))
        {
            printf("Invalid duty.\n");
            return ;
        }
        backlight_ctrl_set_duty(duty);
        int actual_duty = backlight_ctrl_get_duty();
        printf("%d\n", actual_duty);
    }
    else
    {
        int duty = backlight_ctrl_get_duty();
        printf("%d\n", duty);
    }

    return ;
}

/**
 * @brief displayコマンドを処理する
 *        display start
 *        display stop
 * @param ac 引数の数
 * @param av 引数配列
 */
void cmd_display(int ac, char **av)
{
    if (ac >= 2)
    {
        if (strcmp(av[1], "start") == 0)
        {
            lcd_ctrl_set_standby(false);
        }
        else if (strcmp(av[1], "stop") == 0)
        {
            lcd_ctrl_set_standby(true);
        }
        else
        {
            printf("usage:\n");
            printf("  display start\n");
            printf("  display stop\n");
            return ;
        }
    }
    else
    {
        printf("usage:\n");
        printf("  display start\n");
        printf("  display stop\n");
    }

    return ;
}
/**
 * @brief fill コマンドを処理する
 *        fill index#
 * @param ac 引数の数
 * @param av 引数配列
 */
void cmd_fill(int ac, char **av)
{
    if (ac == 2)
    {
        int index_color = strtol(av[1], NULL, 0);
        if ((index_color < 0) || (index_color > 255))
        {
            return ;
        }
        frame_buf_t buf;
        if (!lcd_ctrl_get_back_buffer(&buf))
        {
            printf("Could not get back buffer.\n");
            return ;
        }

        uint32_t begin = get_tick_count();
        frame_buf_fill_rect(&buf, 0, 0, buf.h_size, buf.line_count, (color_t)(index_color));
        lcd_ctrl_swap_front_buffer();
        uint32_t elapse = get_tick_count() - begin;
        printf("Done %d msec.\n", elapse);
    }
    else
    {
        printf("usage:\n");
        printf("  fill index#\n");
    }

    return ;
}

/**
 * @brief visible コマンドを処理する
 *        visible 1|0
 *        visible
 * @param ac 引数の数
 * @param av 引数配列
 */
void cmd_visible(int ac, char **av)
{
    if (ac == 2)
    {
        bool is_visible = (strtol(av[1], NULL, 0) != 0);
        lcd_ctrl_set_frame_buffer_visible(is_visible);
    }
    else
    {
        printf("%d\n", (lcd_ctrl_is_frame_buffer_visible() ? 1 : 0));

    }
    return ;
}

/**
 * @brief backcolor コマンドを処理する
 *        backcolor r# g# b#
 *        backcolor
 * @param ac 引数の数
 * @param av 引数配列
 */
void cmd_backcolor(int ac, char **av)
{
    if (ac == 4)
    {
        int r = strtol(av[1], NULL, 0);
        int g = strtol(av[2], NULL, 0);
        int b = strtol(av[3], NULL, 0);
        lcd_ctrl_set_backcolor((uint8_t)(r), (uint8_t)(g), (uint8_t)(b));
    }
    else if (ac == 1)
    {
        uint8_t r, g, b;
        lcd_ctrl_get_backcolor(&r, &g, &b);
        printf("%d %d %d\n", r, g, b);
    }
    else
    {
        printf("usage:\n");
        printf("  backcolor r# g# b#\n");
        printf("  backcolor\n");
    }

    return ;
}

