/**
 * @file 表示用フレームバッファ
 * @author Cosmosweb Co.,Ltd. 2024
 */

#ifndef FRAME_BUF_H_
#define FRAME_BUF_H_

#include <stdint.h>

typedef int color_t;

#define IMAGE_FORMAT_INEX_COLOR (1) // インデックスカラー

struct frame_buf {
    uint8_t *bufp; // バッファポインタ
    uint16_t h_size; // 水平ピクセル数
    uint16_t line_size; // ラインサイズ
    uint16_t line_count; // ライン数
};
typedef struct frame_buf frame_buf_t;

void frame_buf_init(frame_buf_t *pbuf, uint8_t *bufp, uint16_t h_size, uint16_t line_size, uint16_t line_count);

void frame_buf_plot(frame_buf_t *pbuf, int x, int y, color_t color);
void frame_buf_fill_rect(frame_buf_t *pbuf, int x, int y, int width, int height,
        color_t color);

#endif /* FRAME_BUF_H_ */
