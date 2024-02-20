/**
 * @file LCD制御インタフェース定義
 * @author Cosmosweb Co.,Ltd. 2024
 */
#include <stddef.h>
#include <string.h>

#include <platform.h>
#include <r_glcdc_rx_if.h>
#include <r_glcdc_rx_pinset.h>

#include "utils.h"
#include "lcd_ctrl.h"

/**
 * LCD タイミングパラメータ
 * スマートコンフィギュレータで、RX72N Envision Kitのボード情報を読み込み、
 * GLCDCを有効化したとき、デフォルトで入っていた設定値を流用した。
 */
#define LCD_H_FRONT_PORCH (17)
#define LCD_H_SYNC_WIDTH  (25)
#define LCD_H_BACK_PORCH (62)
#define LCD_H_ACTIVE (480)
#define LCD_H_SYNC_POLARITY (GLCDC_SIGNAL_POLARITY_LOACTIVE)
#define LCD_V_FRONT_PORCH (8)
#define LCD_V_SYNC_WIDTH (1)
#define LCD_V_BACK_PORCH (7)
#define LCD_V_SYNC_POLARITY (GLCDC_SIGNAL_POLARITY_LOACTIVE)
#define LCD_DE_POLARITY (GLCDC_SIGNAL_POLARITY_HIACTIVE)
#define LCD_V_ACTIVE (272)
/**
 * LCDのDATA信号を同期させるクロックエッジ
 */
#define LCD_DATA_SYNC_EDGE (GLCDC_SIGNAL_SYNC_EDGE_RISING)


#define FRAME_BUF_LINE_SIZE (((LCD_H_ACTIVE + 63) / 64) * 64)
#define FRAME_BUF_LINE_COUNT (LCD_V_ACTIVE + 1)

/**
 * @brief LCD設定
 */
static glcdc_cfg_t s_lcd_config;
/**
 * @brief 切り替え用データ
 */
static glcdc_runtime_cfg_t s_layer1_cfg;
/**
 * @brief フレームバッファ1
 */
static uint8_t s_frame_buf1[FRAME_BUF_LINE_SIZE * FRAME_BUF_LINE_COUNT] __attribute__((aligned(64)));
/**
 * @brief フレームバッファ2
 */
static uint8_t s_frame_buf2[FRAME_BUF_LINE_SIZE * FRAME_BUF_LINE_COUNT] __attribute__((aligned(64)));
/**
 * @brief 表示中フレームバッファのアドレス
 */
static uint8_t *s_front_frame_buf;
/**
 * @brief CLUT (いわゆるカラーパレット)
 */
static uint32_t s_clut[256];
/**
 * @brief フレームバッファスワップ要求有無
 */
static bool s_is_swap_buffer_required;
/**
 * @brief 初回コールバックフラグ
 *        GLCDCはリセット解除後、初回のみ意図しない通知が行われる。
 */
static bool s_is_callbacked;

static void wait_vspos(void);
static void on_lcd_event_proc(void *arg);

/**
 * @brief LCD制御を初期化する。
 */
void lcd_ctrl_init(void)
{
    PORTB.PODR.BIT.B3 = 0; // スタンバイ出力設定

    s_is_swap_buffer_required = false;
    s_front_frame_buf = s_frame_buf1;
    s_is_callbacked = false;

    memset(s_clut, 0, sizeof(s_clut));
    // ANSI Color Codes
    s_clut[0] = 0xFF000000u; // Black(0,0,0)
    s_clut[1] = 0xFFFF0000u; // Red(255,0,0)
    s_clut[2] = 0xFF00FF00u; // Green(0,255,0)
    s_clut[3] = 0xFFFFFF00u; // Yellow(255,255,0)
    s_clut[4] = 0xFF0000FFu; // Blue(0,0,255)
    s_clut[5] = 0xFFFF00FFu; // Purple(255,0,255)
    s_clut[6] = 0xFF00FFFFu; // Cyan(0,255,255)
    s_clut[7] = 0xFFFFFFFFu; // White(255,255,255)


    memset(&s_lcd_config, 0, sizeof(s_lcd_config));
    s_lcd_config.output.htiming.back_porch = LCD_H_BACK_PORCH;
    s_lcd_config.output.htiming.sync_width = LCD_H_SYNC_WIDTH;
    s_lcd_config.output.htiming.front_porch = LCD_H_FRONT_PORCH;
    s_lcd_config.output.htiming.display_cyc = LCD_H_ACTIVE;
    s_lcd_config.output.vtiming.back_porch = LCD_V_BACK_PORCH;
    s_lcd_config.output.vtiming.sync_width = LCD_V_SYNC_WIDTH;
    s_lcd_config.output.vtiming.front_porch = LCD_V_FRONT_PORCH;
    s_lcd_config.output.vtiming.display_cyc = LCD_V_ACTIVE;
    s_lcd_config.output.hsync_polarity = LCD_H_SYNC_POLARITY;
    s_lcd_config.output.data_enable_polarity = LCD_DE_POLARITY;
    s_lcd_config.output.clksrc = GLCDC_CLK_SRC_INTERNAL; // PLL = 240MHz
    s_lcd_config.output.clock_div_ratio = GLCDC_PANEL_CLK_DIVISOR_24; // 24分周 -> 240/24 = 10MHz
    s_lcd_config.output.format = GLCDC_OUT_FORMAT_16BITS_RGB565; // 16bit
    s_lcd_config.output.sync_edge = LCD_DATA_SYNC_EDGE;
    s_lcd_config.output.bg_color.argb = 0x00000000u; // 背景色(bit31-24:-, bit23-16:R, bit15-8:G, bit7-0:B)

    // TCON2 -> 32pin HSYNC
    // TCON0 -> 33pin VSYNC
    // TCON3 -> 34pin DE
    s_lcd_config.output.tcon_hsync = GLCDC_TCON_PIN_2;
    s_lcd_config.output.tcon_vsync = GLCDC_TCON_PIN_0;
    s_lcd_config.output.tcon_de = GLCDC_TCON_PIN_3;

#ifdef __RX_LITTLE_ENDIAN__
    s_lcd_config.output.endian = GLCDC_ENDIAN_LITTLE;
#else
    s_lcd_config.output.endian = GLCDC_ENDIAN_BIG;
#endif

    s_lcd_config.output.color_order = GLCDC_COLOR_ORDER_RGB;
    s_lcd_config.output.correction_proc_order = GLCDC_BRIGHTNESS_CONTRAST_TO_GAMMA; // 輝度/コントラスト補正後にガンマ補正

    s_lcd_config.p_callback = on_lcd_event_proc;

    // フレームバッファ1
    s_layer1_cfg.input.format = GLCDC_IN_FORMAT_CLUT8; // CLUT(8)
    s_layer1_cfg.input.p_base = (uint32_t*)(s_front_frame_buf);
    s_layer1_cfg.input.bg_color.argb = 0xFF000000u; // 背景色(bit31-24:-, bit23-16:R, bit15-8:G, bit7-0:B)
    s_layer1_cfg.input.hsize = LCD_H_ACTIVE;
    s_layer1_cfg.input.vsize = LCD_V_ACTIVE;
    s_layer1_cfg.input.offset = FRAME_BUF_LINE_SIZE;
    s_layer1_cfg.input.frame_edge = false; // フレームのエッジ表示しない
    s_layer1_cfg.input.coordinate.x = 0;
    s_layer1_cfg.input.coordinate.y = 0;
    s_layer1_cfg.blend.blend_control = GLCDC_BLEND_CONTROL_NONE;
    s_layer1_cfg.blend.visible = true; // 画像表示
    s_layer1_cfg.blend.frame_edge = false; // 枠表示しない
    s_layer1_cfg.blend.fixed_blend_value = 0xFF;
    s_layer1_cfg.blend.fade_speed = 0x00;
    s_layer1_cfg.blend.start_coordinate.x = 0;
    s_layer1_cfg.blend.start_coordinate.y = 0;
    s_layer1_cfg.blend.end_coordinate.x = LCD_H_ACTIVE;
    s_layer1_cfg.blend.end_coordinate.y = LCD_V_ACTIVE;
    s_layer1_cfg.chromakey.enable = false; // クロマキー無効
    s_layer1_cfg.chromakey.before.argb = 0x00000000u;
    s_layer1_cfg.chromakey.after.argb = 0x00000000u;

    memcpy(&(s_lcd_config.input[0]), &(s_layer1_cfg.input), sizeof(glcdc_input_cfg_t));
    memcpy(&(s_lcd_config.blend[0]), &(s_layer1_cfg.blend), sizeof(glcdc_blend_t));
    memcpy(&(s_lcd_config.chromakey[0]), &(s_layer1_cfg.chromakey), sizeof(glcdc_chromakey_t));
    s_lcd_config.clut[0].enable = 1; // CLUT更新
    s_lcd_config.clut[0].p_base = s_clut;
    s_lcd_config.clut[0].start = 0;
    s_lcd_config.clut[0].size = 8; // 8色

#if GLCDC_FRAME_LAYER_NUM >= 2
    for (int n = 1; n < GLCDC_FRAME_LAYER_NUM; n++)
    {
        // フレームバッファN
        s_lcd_config.input[n].format = GLCDC_IN_FORMAT_CLUT8;
        s_lcd_config.input[n].p_base = NULL;
        s_lcd_config.input[n].bg_color.argb = 0x00000000u; // 背景色(bit31-24:-, bit23-16:R, bit15-8:G, bit7-0:B)
        s_lcd_config.input[n].hsize = 0;
        s_lcd_config.input[n].vsize = 0;
        s_lcd_config.input[n].offset = FRAME_BUF_LINE_SIZE;
        s_lcd_config.input[n].frame_edge = false; // フレームのエッジ表示しない
        s_lcd_config.input[n].coordinate.x = 0;
        s_lcd_config.input[n].coordinate.y = 0;
        s_lcd_config.blend[n].blend_control = GLCDC_BLEND_CONTROL_NONE;
        s_lcd_config.blend[n].visible = false; // フレームバッファ2画像表示しない
        s_lcd_config.blend[n].frame_edge = false; // 枠表示しない
        s_lcd_config.blend[n].fixed_blend_value = 0xFF;
        s_lcd_config.blend[n].fade_speed = 0x00;
        s_lcd_config.blend[n].start_coordinate.x = 0;
        s_lcd_config.blend[n].start_coordinate.y = 0;
        s_lcd_config.blend[n].end_coordinate.x = LCD_H_ACTIVE;
        s_lcd_config.blend[n].end_coordinate.y = LCD_V_ACTIVE;
        s_lcd_config.chromakey[n].enable = false; // クロマキー無効
        s_lcd_config.chromakey[n].before.argb = 0x00000000u;
        s_lcd_config.chromakey[n].after.argb = 0x00000000u;
        s_lcd_config.clut[n].enable = 1; // CLUT更新
        s_lcd_config.clut[n].p_base = s_clut;
        s_lcd_config.clut[n].start = 0;
        s_lcd_config.clut[n].size = 8; // 8色
    }
#endif

    s_lcd_config.output.dithering.dithering_on = true;// 10bitデータを8bitに丸める時の処理をする(単純切り捨てじゃない)
    s_lcd_config.output.dithering.dithering_mode = GLCDC_DITHERING_MODE_ROUND_OFF; // 0捨て1入れモード

    s_lcd_config.output.brightness.enable = false; // 輝度補正無効
    s_lcd_config.output.gamma.enable = false; // ガンマ補正無効

    s_lcd_config.detection.vpos_detect = true;
    s_lcd_config.detection.gr1uf_detect = false;
    s_lcd_config.detection.gr2uf_detect = false;
    s_lcd_config.interrupt.vpos_enable = true;
    s_lcd_config.interrupt.gr1uf_enable = false;
    s_lcd_config.interrupt.gr2uf_enable = false;

    glcdc_err_t ret = R_GLCDC_Open(&s_lcd_config);
    if (ret != GLCDC_SUCCESS)
    {
        printf("R_GLCDC_Open() failure. ret=%d\n", ret);
    }
    else
    {
        R_GLCDC_PinSet();
    }

    return ;
}


/**
 * @brief LCD制御を初期化する。
 */
void lcd_ctrl_fini(void)
{
    R_GLCDC_Close();

    return ;
}

/**
 * @brief LCDのスタンバイモードON/OFFを設定する。
 * @param is_standby スタンバイモードにする場合にはtrue, それ以外はfalse.
 */
void lcd_ctrl_set_standby(bool is_standby)
{
    if (lcd_ctrl_is_standby() == is_standby) // 変更なし？
    {
        return ;
    }

    PORTB.PODR.BIT.B3 = (is_standby) ? 0 : 1;
    if (is_standby)
    {
        PORTB.PODR.BIT.B3 = 0;
        R_GLCDC_Control(GLCDC_CMD_STOP_DISPLAY, NULL);
        wait_vspos();
    }
    else
    {
        R_GLCDC_Control(GLCDC_CMD_START_DISPLAY, NULL);
        PORTB.PODR.BIT.B3 = 1; //
        wait_vspos();
    }

    return ;
}

/**
 * @brief LCDがスタンバイモードかどうかを取得する。
 * @return スタンバイモードの場合にはtrue, それ以外はfalse.
 */
bool lcd_ctrl_is_standby(void)
{
    return PORTB.PODR.BIT.B3 == 0;
}

/**
 * @brief バックバッファを得る。
 *        スワップ要求を出して、スワップ完了するまではバックバッファを取得できない。
 * @param pbuf バックバッファを取得するフレームバッファオブジェクト
 * @return 成功した場合にはtrue, 失敗した場合にはfalse.
 */
bool lcd_ctrl_get_back_buffer(frame_buf_t *pbuf)
{
    if (s_is_swap_buffer_required)
    {
        return NULL;
    }

    uint8_t *back_buf = (s_front_frame_buf == s_frame_buf1) ? s_frame_buf2 : s_frame_buf1;
    frame_buf_init(pbuf, back_buf, LCD_H_ACTIVE, FRAME_BUF_LINE_SIZE, LCD_V_ACTIVE);

    return true;
}

/**
 * @brief フロントバッファを更新する。
 */
void lcd_ctrl_swap_front_buffer(void)
{
    s_is_swap_buffer_required = true;
}

/**
 * @brief フレームバッファを表示するかどうかを設定する。
 * @param is_visible フレームバッファを表示する場合にはtrue, それ以外はfalse.
 */
void lcd_ctrl_set_frame_buffer_visible(bool is_visible)
{
    s_layer1_cfg.blend.visible = is_visible;
    R_GLCDC_LayerChange(GLCDC_FRAME_LAYER_1, &s_layer1_cfg);

    return ;
}

/**
 * @brief フレームバッファを表示するかどうかを取得する。
 * @return フレームバッファを表示する場合にはtrue, それ以外はfalse.
 */
bool lcd_ctrl_is_frame_buffer_visible(void)
{
    return s_layer1_cfg.blend.visible;
}

/**
 * @brief 背景色を設定する。
 * @param r R値
 * @param g G値
 * @param b B値
 */
void lcd_ctrl_set_backcolor(uint8_t r, uint8_t g, uint8_t b)
{
    glcdc_color_t color;
    color.argb = (0xFF000000u)
            | ((uint32_t)(r) << 16u)
            | ((uint32_t)(g) << 8u)
            | ((uint32_t)(b) << 0u);
    R_GLCDC_Control(GLCDC_CMD_CHANGE_BG_COLOR, &color);

    memcpy(&(s_lcd_config.output.bg_color), &color, sizeof(glcdc_color_t));

    return ;
}
/**
 * @brief 背景色を取得する。
 * @param r R値を取得する変数
 * @param g G値を取得する変数
 * @param b B値を取得する変数
 */
void lcd_ctrl_get_backcolor(uint8_t *r, uint8_t *g, uint8_t *b)
{
    uint32_t argb = s_lcd_config.output.bg_color.argb;
    if (r != NULL)
    {
        (*r) = (uint8_t)((argb >> 16) & 0xFFu);
    }
    if (g != NULL)
    {
        (*g) = (uint8_t)((argb >> 8) & 0xFFu);
    }
    if (b != NULL)
    {
        (*b) = (uint8_t)((argb >> 0) & 0xFFu);
    }
    return ;
}

/**
 * @brief VPOS検知待ちをする
 *        VPOS検知するか、1秒経過すると制御を返す。
 */
static void wait_vspos(void)
{
    glcdc_status_t status;

    uint32_t begin = get_tick_count();
    while ((get_tick_count() - begin) < 1000u) // 処理開始後、1秒未満？
    {
        if (R_GLCDC_GetStatus(&status) == GLCDC_SUCCESS)
        {
            if (status.state_vpos == GLCDC_DETECTED)
            {
                break;
            }
        }
    }

    return ;
}


/**
 * @brief LCDイベントコールバック
 * @param arg
 */
static void on_lcd_event_proc(void *arg)
{
    if (!s_is_callbacked)
    {
        // 初回の呼び出しは無視する。
        s_is_callbacked = true;
    }
    else
    {
        glcdc_callback_args_t *p = (glcdc_callback_args_t*)(arg);
        switch (p->event)
        {
            case GLCDC_EVENT_LINE_DETECTION: // VPOS?
            {
                if (s_is_swap_buffer_required)
                {
                    s_front_frame_buf = (s_front_frame_buf == s_frame_buf1) ? s_frame_buf2 : s_frame_buf1;
                    s_layer1_cfg.input.p_base = (uint32_t*)(s_front_frame_buf);
                    R_GLCDC_LayerChange(GLCDC_FRAME_LAYER_1, &s_layer1_cfg);
                    s_is_swap_buffer_required = false;
                }
                break;
            }
            case GLCDC_EVENT_GR1_UNDERFLOW: // グラフィクス1 アンダーフロー
            {
                break;
            }
            case GLCDC_EVENT_GR2_UNDERFLOW: // グラフィクス2 アンダーフロー
            {
                break;
            }
            default:
            {
                // do nothing.
                break;
            }
        }
    }

    return ;
}
