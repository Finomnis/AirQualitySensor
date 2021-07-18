#include "Display.hpp"

#include "../pinout.hpp"

U8G2_ST7565_EA_DOGM132_F_4W_HW_SPI Display(U8G2_R0, PINS::DISPLAY_CS, PINS::DISPLAY_DC, PINS::DISPLAY_RESET);
