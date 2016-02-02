#include "screen.h"

#include "../config.h"
#if defined(F_EXAMPLES) && defined(F_SPI) && defined(F_PCD8544)

#include "state.h"
#include "../pcd8544_tiny_font.h"
#include "../pcd8544.h"
#include "../pcd8544_font.h"
#include "../pcd8544bitmaps.h"
#include "../pcd8544drawing.h"
#include "../pcd8544_calcfont.h"

uint8_t print_socket_status_in_notification_area(uint8_t color)
{
	uint8_t x = pcd8544_print_str(1, 1, "SCKT: ", &tiny_font, color, ALIGN_LEFT);
	if(current_state.is_socket_on)
	x = pcd8544_print_str(x, 1, "ON", &tiny_font, color, ALIGN_LEFT);
	else
	x = pcd8544_print_str(x, 1, "OFF", &tiny_font, color, ALIGN_LEFT);
	return x;
}

uint8_t print_temp_in_notification_area(float temp, uint8_t color)
{
	int8_t num_after_dot = (int16_t)(temp * 10) % 10;
	if(num_after_dot < 0)
	num_after_dot = -num_after_dot;
	uint8_t len = pcd8544_get_str_length_in_px("TEMP: ", &tiny_font);
	len += pcd8577_print_calc_style_num_length_in_px(temp) + 1 + pcd8577_print_calc_style_num_length_in_px(num_after_dot);
	uint8_t x = PCD8544_WIDTH - len - 3;
	
	x = pcd8544_print_str(x, 1, " TEMP: ", &tiny_font, color, ALIGN_LEFT);
	x = pcd8577_print_calc_style_num(x + 1, 3, current_state.temperature, color);
	x = pcd8577_print_calc_style_num(x , 3, CALC_FONT_DOT, color);
	x = pcd8577_print_calc_style_num(x , 3, num_after_dot, color);
	
	return len + 3;
}

void redraw_notification_area()
{
	pcd8544_fill_rect(0, 0, 12, PCD8544_WIDTH, 1);
	uint8_t len = print_temp_in_notification_area(-25.7, 0);
	uint8_t x = print_socket_status_in_notification_area(0);
	
	if(current_state.is_auto_on)
	{
		len = PCD8544_WIDTH - x - len;
		
		pcd8544_draw_bitmap( x + len / 2 - 5, 1, auto_on, 10, 10, 0);
	}
}

void draw_button(uint8_t x, uint8_t y, const byte* icon, uint8_t w, uint8_t h, uint8_t pressed)
{
	pcd8544_fill_rounded_rect(y, x, y + h - 3, x + w, 3, 0);
	if(pressed)
		pcd8544_fill_rounded_rect(y, x, y + h - 3, x + w, 3, 1);
	else
		pcd8544_draw_rounded_rect(y, x, y + h - 3, x + w, 3, 1);
	
	pcd8544_draw_bitmap( x, y , icon, w, h, pressed ? 0 : 1);
}

#endif // F_EXAMPLES