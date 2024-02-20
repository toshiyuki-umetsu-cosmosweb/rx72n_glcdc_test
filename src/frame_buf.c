/**
 * @file フレームバッファ
 * @author Cosmosweb Co.,Ltd. 2024
 */
#include <stddef.h>
#include "frame_buf.h"

/**
 * @brief フレームバッファを初期化する。
 * @param pbuf フレームバッファオブジェクト
 * @param bufp バッファアドレス
 * @param h_size 水平サイズ
 * @param line_size 1ラインあたりの倍と数
 * @param line_count バッファのライン数
 */
void frame_buf_init(frame_buf_t *pbuf, uint8_t *bufp, uint16_t h_size, uint16_t line_size, uint16_t line_count)
{
    pbuf->bufp = bufp;
    pbuf->h_size = h_size;
    pbuf->line_size = line_size;
    pbuf->line_count = line_count;
    return ;
}

/**
 * @brief フレームバッファに点をうつ
 * @param pbuf フレームバッファ
 * @param x x位置
 * @param y y位置
 * @param color 色
 */
void frame_buf_plot(frame_buf_t *pbuf, int x, int y, color_t color)
{
    if ((pbuf != NULL) && (x >= 0) && (x < pbuf->h_size)
            && (y >= 0) && (y < pbuf->line_count))
    {
        uint32_t offset = y * pbuf->line_size + x;
        pbuf->bufp[offset] = color;
    }

    return ;
}


/**
 * @brief フレームバッファの指定領域を、指定色で塗りつぶす
 * @param pbuf フレームバッファ
 * @param x x位置
 * @param y y位置
 * @param width 幅
 * @param height 高さ
 * @param color 色
 */
void frame_buf_fill_rect(frame_buf_t *pbuf, int x, int y, int width, int height,
        color_t color)
{
    int yend = y + height;
    int xend = x + width;

    for (int ypos = y; ypos < yend; ypos++)
    {
        for (int xpos = x; xpos < xend; xpos++)
        {
            frame_buf_plot(pbuf, xpos, ypos, color);
        }
    }

    return ;
}
