/**
 * @file LCD表示器制御インタフェース
 * @author Cosmosweb Co.,Ltd. 2024
 */

#ifndef LCD_CTRL_H_
#define LCD_CTRL_H_

#include <stdbool.h>
#include "frame_buf.h"

void lcd_ctrl_init(void);
void lcd_ctrl_fini(void);


void lcd_ctrl_set_standby(bool is_standby);
bool lcd_ctrl_is_standby(void);

bool lcd_ctrl_get_back_buffer(frame_buf_t *pbuf);
void lcd_ctrl_swap_front_buffer(void);
void lcd_ctrl_set_frame_buffer_visible(bool is_visible);
bool lcd_ctrl_is_frame_buffer_visible(void);
void lcd_ctrl_set_backcolor(uint8_t r, uint8_t g, uint8_t b);
void lcd_ctrl_get_backcolor(uint8_t *r, uint8_t *g, uint8_t *b);

#endif /* LCD_CTRL_H_ */
