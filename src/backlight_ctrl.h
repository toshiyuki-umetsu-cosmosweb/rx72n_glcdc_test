/**
 * @file LCDバックライト制御インタフェース宣言
 * @author Cosmosweb Co.,Ltd. 2024
 */

#ifndef BACKLIGHT_CTRL_H_
#define BACKLIGHT_CTRL_H_

void backlight_ctrl_init(void);
void backlight_ctrl_fini(void);
void backlight_ctrl_update(void); // Called by TPU2 interrupt.
void backlight_ctrl_set_duty(int duty);
int backlight_ctrl_get_duty(void);

#endif /* BACKLIGHT_CTRL_H_ */
