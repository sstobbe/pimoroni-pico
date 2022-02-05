#include "hardware/spi.h"
#include "hardware/sync.h"
#include "pico/binary_info.h"

#include "libraries/pico_display/pico_display.hpp"

using namespace pimoroni;

PicoDisplay *display = nullptr;


extern "C" {
#include "pico_display.h"

#define NOT_INITIALISED_MSG     "Cannot call this function, as picodisplay is not initialised. Call picodisplay.init(<bytearray>) first."

mp_obj_t picodisplay_buf_obj;

mp_obj_t picodisplay_init(mp_obj_t buf_obj) {
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf_obj, &bufinfo, MP_BUFFER_RW);
    picodisplay_buf_obj = buf_obj;
    if(display == nullptr)
        display = new PicoDisplay((uint16_t *)bufinfo.buf);
    display->init();
    return mp_const_none;
}

mp_obj_t picodisplay_get_width() {
    return mp_obj_new_int(PicoDisplay::WIDTH);
}

mp_obj_t picodisplay_get_height() {
    return mp_obj_new_int(PicoDisplay::HEIGHT);
}

mp_obj_t picodisplay_update() {
    if(display != nullptr)
        display->update();
    else
        mp_raise_msg(&mp_type_RuntimeError, NOT_INITIALISED_MSG);

    return mp_const_none;
}

mp_obj_t picodisplay_flip() {
    if(display != nullptr)
        display->flip();
    else
        mp_raise_msg(&mp_type_RuntimeError, NOT_INITIALISED_MSG);

    return mp_const_none;
}

mp_obj_t picodisplay_set_backlight(mp_obj_t brightness_obj) {
    if(display != nullptr) {
        float brightness = mp_obj_get_float(brightness_obj);

        if(brightness < 0 || brightness > 1.0f)
            mp_raise_ValueError("brightness out of range. Expected 0.0 to 1.0");
        else
            display->set_backlight((uint8_t)(brightness * 255.0f));
    }
    else
        mp_raise_msg(&mp_type_RuntimeError, NOT_INITIALISED_MSG);

    return mp_const_none;
}

mp_obj_t picodisplay_set_led(mp_obj_t r_obj, mp_obj_t g_obj, mp_obj_t b_obj) {
    if(display != nullptr) {
        int r = mp_obj_get_int(r_obj);
        int g = mp_obj_get_int(g_obj);
        int b = mp_obj_get_int(b_obj);

        if(r < 0 || r > 255)
            mp_raise_ValueError("r out of range. Expected 0 to 255");
        else if(g < 0 || g > 255)
            mp_raise_ValueError("g out of range. Expected 0 to 255");
        else if(b < 0 || b > 255)
            mp_raise_ValueError("b out of range. Expected 0 to 255");
        else
            display->set_led(r, g, b);
    }
    else
        mp_raise_msg(&mp_type_RuntimeError, NOT_INITIALISED_MSG);

    return mp_const_none;
}

mp_obj_t picodisplay_is_pressed(mp_obj_t button_obj) {
    bool buttonPressed = false;
    
    if(display != nullptr) {
        int buttonID = mp_obj_get_int(button_obj);
        switch(buttonID) {
        case 0:
            buttonPressed = display->is_pressed(PicoDisplay::A);
            break;

        case 1:
            buttonPressed = display->is_pressed(PicoDisplay::B);
            break;

        case 2:
            buttonPressed = display->is_pressed(PicoDisplay::X);
            break;

        case 3:
            buttonPressed = display->is_pressed(PicoDisplay::Y);
            break;

        default:
            mp_raise_ValueError("button not valid. Expected 0 to 3");
            break;
        }
    }
    else
        mp_raise_msg(&mp_type_RuntimeError, NOT_INITIALISED_MSG);

    return buttonPressed ? mp_const_true : mp_const_false;
}

mp_obj_t picodisplay_set_pen(mp_uint_t n_args, const mp_obj_t *args) {
    if(display != nullptr) {
        switch(n_args) {
        case 1: {
                int p = mp_obj_get_int(args[0]);

                if(p < 0 || p > 0xffff)
                    mp_raise_ValueError("p is not a valid pen.");
                else
                    display->set_pen(p);
            } break;

        case 3: {
                int r = mp_obj_get_int(args[0]);
                int g = mp_obj_get_int(args[1]);
                int b = mp_obj_get_int(args[2]);

                if(r < 0 || r > 255)
                    mp_raise_ValueError("r out of range. Expected 0 to 255");
                else if(g < 0 || g > 255)
                    mp_raise_ValueError("g out of range. Expected 0 to 255");
                else if(b < 0 || b > 255)
                    mp_raise_ValueError("b out of range. Expected 0 to 255");
                else
                    display->set_pen(r, g, b);
            } break;

        default: {
                char *buffer;
                buffer = (char*)malloc(100);
                sprintf(buffer, "function takes 1 or 3 (r,g,b) positional arguments but %d were given", n_args);
                mp_raise_TypeError(buffer);
            } break;
        }
    }
    else
        mp_raise_msg(&mp_type_RuntimeError, NOT_INITIALISED_MSG);

    return mp_const_none;
}

mp_obj_t picodisplay_create_pen(mp_obj_t r_obj, mp_obj_t g_obj, mp_obj_t b_obj) {
    int pen = 0;
    
    if(display != nullptr) {
        int r = mp_obj_get_int(r_obj);
        int g = mp_obj_get_int(g_obj);
        int b = mp_obj_get_int(b_obj);
        
        if(r < 0 || r > 255)
            mp_raise_ValueError("r out of range. Expected 0 to 255");
        else if(g < 0 || g > 255)
            mp_raise_ValueError("g out of range. Expected 0 to 255");
        else if(b < 0 || b > 255)
            mp_raise_ValueError("b out of range. Expected 0 to 255");
        else
            pen = display->create_pen(r, g, b);
    }
    else
        mp_raise_msg(&mp_type_RuntimeError, NOT_INITIALISED_MSG);

    return mp_obj_new_int(pen);
}

mp_obj_t picodisplay_set_clip(mp_uint_t n_args, const mp_obj_t *args) {
    (void)n_args; //Unused input parameter, we know it's 4

    if(display != nullptr) {
        int x = mp_obj_get_int(args[0]);
        int y = mp_obj_get_int(args[1]);
        int w = mp_obj_get_int(args[2]);
        int h = mp_obj_get_int(args[3]);

        Rect r(x, y, w, h);
        display->set_clip(r);
    }
    else
        mp_raise_msg(&mp_type_RuntimeError, NOT_INITIALISED_MSG);

    return mp_const_none;
}

mp_obj_t picodisplay_remove_clip() {
    if(display != nullptr)
        display->remove_clip();
    else
        mp_raise_msg(&mp_type_RuntimeError, NOT_INITIALISED_MSG);

    return mp_const_none;
}

mp_obj_t picodisplay_clear() {
    if(display != nullptr)
        display->clear();
    else
        mp_raise_msg(&mp_type_RuntimeError, NOT_INITIALISED_MSG);

    return mp_const_none;
}

mp_obj_t picodisplay_pixel(mp_obj_t x_obj, mp_obj_t y_obj) {
    if(display != nullptr) {
        int x = mp_obj_get_int(x_obj);
        int y = mp_obj_get_int(y_obj);

        Point p(x, y);
        display->pixel(p);
    }
    else
        mp_raise_msg(&mp_type_RuntimeError, NOT_INITIALISED_MSG);

    return mp_const_none;
}

mp_obj_t picodisplay_pixel_span(mp_obj_t x_obj, mp_obj_t y_obj, mp_obj_t l_obj) {
    if(display != nullptr) {
        int x = mp_obj_get_int(x_obj);
        int y = mp_obj_get_int(y_obj);
        int l = mp_obj_get_int(l_obj);

        Point p(x, y);
        display->pixel_span(p, l);
    }
    else
        mp_raise_msg(&mp_type_RuntimeError, NOT_INITIALISED_MSG);

    return mp_const_none;
}

mp_obj_t picodisplay_rectangle(mp_uint_t n_args, const mp_obj_t *args) {
    (void)n_args; //Unused input parameter, we know it's 4

    if(display != nullptr) {
        int x = mp_obj_get_int(args[0]);
        int y = mp_obj_get_int(args[1]);
        int w = mp_obj_get_int(args[2]);
        int h = mp_obj_get_int(args[3]);

        Rect r(x, y, w, h);
        display->rectangle(r);
    }
    else
        mp_raise_msg(&mp_type_RuntimeError, NOT_INITIALISED_MSG);

    return mp_const_none;
}

mp_obj_t picodisplay_circle(mp_obj_t x_obj, mp_obj_t y_obj, mp_obj_t r_obj) {
    if(display != nullptr) {
        int x = mp_obj_get_int(x_obj);
        int y = mp_obj_get_int(y_obj);
        int r = mp_obj_get_int(r_obj);

        Point p(x, y);
        display->circle(p, r);
    }
    else
        mp_raise_msg(&mp_type_RuntimeError, NOT_INITIALISED_MSG);

    return mp_const_none;
}

mp_obj_t picodisplay_character(mp_uint_t n_args, const mp_obj_t *args) {
    if(display != nullptr) {
        int c = mp_obj_get_int(args[0]);
        int x = mp_obj_get_int(args[1]);
        int y = mp_obj_get_int(args[2]);

        Point p(x, y);
        if(n_args == 4) {
            int scale = mp_obj_get_int(args[3]);
            display->character((char)c, p, scale);
        }
        else
            display->character((char)c, p);
    }

    return mp_const_none;
}

mp_obj_t picodisplay_text(mp_uint_t n_args, const mp_obj_t *args) {
    if(display != nullptr) {
        if(mp_obj_is_str_or_bytes(args[0])) {
            GET_STR_DATA_LEN(args[0], str, str_len);

            std::string t((const char*)str);

            int x = mp_obj_get_int(args[1]);
            int y = mp_obj_get_int(args[2]);
            int wrap = mp_obj_get_int(args[3]);

            Point p(x, y);
            if(n_args == 5) {
                int scale = mp_obj_get_int(args[4]);
                display->text(t, p, wrap, scale);
            }
            else
                display->text(t, p, wrap);
        }
        else if(mp_obj_is_float(args[0])) {
            mp_raise_TypeError("can't convert 'float' object to str implicitly");
        }
        else if(mp_obj_is_int(args[0])) {
            mp_raise_TypeError("can't convert 'int' object to str implicitly");
        }
        else if(mp_obj_is_bool(args[0])) {
            mp_raise_TypeError("can't convert 'bool' object to str implicitly");
        }
        else {
            mp_raise_TypeError("can't convert object to str implicitly");
        }
    }
    else
        mp_raise_msg(&mp_type_RuntimeError, NOT_INITIALISED_MSG);

    return mp_const_none;
}
// Scottie Functions :)
mp_obj_t picodisplay_line(mp_uint_t n_args, const mp_obj_t *args) {
    (void)n_args; //Unused input parameter, we know it's 4
    
    if(display != nullptr) {
        int x1 = mp_obj_get_int(args[0]);
        int y1 = mp_obj_get_int(args[1]);
        int x2 = mp_obj_get_int(args[2]);
        int y2 = mp_obj_get_int(args[3]);

        Point p1(x1, y1);
        Point p2(x2, y2);
        display->line(p1, p2);
    }
    else
        mp_raise_msg(&mp_type_RuntimeError, NOT_INITIALISED_MSG);

    return mp_const_none;
}
mp_obj_t picodisplay_draw(size_t n_args, const mp_obj_t *args)
{
	//st7789_ST7789_obj_t *self			 = MP_OBJ_TO_PTR(args[0]);
	char				 single_char_s[] = {0, 0};
	const char *		 s;

	mp_obj_module_t *hershey = (mp_obj_module_t*)args[0];

	if (mp_obj_is_int(args[1])) {
		mp_int_t c		 = mp_obj_get_int(args[1]);
		single_char_s[0] = c & 0xff;
		s				 = single_char_s;
	} else {
		s = mp_obj_str_get_str(args[1]);
	}

	mp_int_t x = mp_obj_get_int(args[2]);
	mp_int_t y = mp_obj_get_int(args[3]);

	//mp_int_t color = (n_args > 5) ? mp_obj_get_int(args[5]) : WHITE;

	mp_int_t scale = 256;
	if (n_args > 4) {
		if (mp_obj_is_float(args[4])) {
			scale = (mp_int_t)(256.0*mp_obj_float_get(args[4]));
		}
		if (mp_obj_is_int(args[4])) {
			scale = 256*mp_obj_get_int(args[4]);
		}
	}
	
	

	mp_obj_dict_t *	 dict			 = (mp_obj_dict_t*)(hershey->globals);
	mp_obj_t *		 index_data_buff = (mp_obj_t *)mp_obj_dict_get(dict, MP_OBJ_NEW_QSTR(MP_QSTR_INDEX));
	mp_buffer_info_t index_bufinfo;
	mp_get_buffer_raise(index_data_buff, &index_bufinfo, MP_BUFFER_READ);
	uint8_t *index = (uint8_t*)index_bufinfo.buf;
	

	mp_obj_t *		 font_data_buff = (mp_obj_t*)mp_obj_dict_get(dict, MP_OBJ_NEW_QSTR(MP_QSTR_FONT));
	mp_buffer_info_t font_bufinfo;
	mp_get_buffer_raise(font_data_buff, &font_bufinfo, MP_BUFFER_READ);
	int8_t *font = (int8_t*)font_bufinfo.buf;

	int16_t from_x = x;
	int16_t from_y = y;
	int16_t to_x   = x;
	int16_t to_y   = y;
	int16_t pos_x  = x;
	int16_t pos_y  = y;
	bool	penup  = true;
	char	c;
	int16_t ii;

	while ((c = *s++)) {
		if (c >= 32 && c <= 127) {
			ii = (c - 32) * 2;

			int16_t offset = index[ii] | (index[ii + 1] << 8);
			int16_t length = font[offset++];
			int16_t left   = (int) ((scale * (font[offset++] - 0x52) + 128)>>8);
			int16_t right  = (int) ((scale * (font[offset++] - 0x52) + 128)>>8);
			int16_t width  = right - left;

			if (length) {
				int16_t i;
				for (i = 0; i < length; i++) {
					if (font[offset] == ' ') {
						offset += 2;
						penup = true;
						continue;
					}

					int16_t vector_x = (int) ((scale * (font[offset++] - 0x52) + 128)>>8);
					int16_t vector_y = (int) ((scale * (font[offset++] - 0x52) + 128)>>8);

					if (!i || penup) {
						from_x = pos_x + vector_x - left;
						from_y = pos_y + vector_y;
					} else {
						to_x = pos_x + vector_x - left;
						to_y = pos_y + vector_y;

						Point p1(from_x, from_y);
                        Point p2(to_x, to_y);
                        display->line(p1, p2);
						//line(self, from_x, from_y, to_x, to_y, color);
						from_x = to_x;
						from_y = to_y;
					}
					penup = false;
				}
			}
			pos_x += width;
		}
	}

	return mp_const_none;
}
//STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(st7789_ST7789_draw_obj, 5, 7, st7789_ST7789_draw);
}
