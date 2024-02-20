/**
 * @file LCDコントロールコマンド宣言
 * @author Cosmosweb Co.,Ltd. 2024
 */

#ifndef COMMAND_LCD_H_
#define COMMAND_LCD_H_

void cmd_backlight(int ac, char **av);
void cmd_display(int ac, char **av);
void cmd_fill(int ac, char **av);
void cmd_visible(int ac, char **av);
void cmd_backcolor(int ac, char **av);

#endif /* COMMAND_LCD_H_ */
