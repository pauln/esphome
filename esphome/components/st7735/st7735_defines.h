#pragma once

#include "esphome/core/component.h"
#include "esphome/components/spi/spi.h"
#include "esphome/components/display/display_buffer.h"
#include "esphome/components/bufferex_base/bufferex_base.h"

namespace esphome {
namespace st7735 {
static const uint8_t ST_CMD_DELAY = 0x80;  // special signifier for command lists

static const uint8_t ST77XX_NOP = 0x00;
static const uint8_t ST77XX_SWRESET = 0x01;
static const uint8_t ST77XX_RDDID = 0x04;
static const uint8_t ST77XX_RDDST = 0x09;

static const uint8_t ST77XX_SLPIN = 0x10;
static const uint8_t ST77XX_SLPOUT = 0x11;
static const uint8_t ST77XX_PTLON = 0x12;
static const uint8_t ST77XX_NORON = 0x13;

static const uint8_t ST77XX_INVOFF = 0x20;
static const uint8_t ST77XX_INVON = 0x21;
static const uint8_t ST77XX_DISPOFF = 0x28;
static const uint8_t ST77XX_DISPON = 0x29;
static const uint8_t ST77XX_CASET = 0x2A;
static const uint8_t ST77XX_RASET = 0x2B;
static const uint8_t ST77XX_RAMWR = 0x2C;
static const uint8_t ST77XX_RAMRD = 0x2E;

static const uint8_t ST77XX_PTLAR = 0x30;
static const uint8_t ST77XX_TEOFF = 0x34;
static const uint8_t ST77XX_TEON = 0x35;
static const uint8_t ST77XX_MADCTL = 0x36;
static const uint8_t ST77XX_COLMOD = 0x3A;

static const uint8_t ST77XX_MADCTL_MY = 0x80;
static const uint8_t ST77XX_MADCTL_MX = 0x40;
static const uint8_t ST77XX_MADCTL_MV = 0x20;
static const uint8_t ST77XX_MADCTL_ML = 0x10;
static const uint8_t ST77XX_MADCTL_RGB = 0x00;

static const uint8_t ST77XX_RDID1 = 0xDA;
static const uint8_t ST77XX_RDID2 = 0xDB;
static const uint8_t ST77XX_RDID3 = 0xDC;
static const uint8_t ST77XX_RDID4 = 0xDD;

// Some register settings
static const uint8_t ST7735_MADCTL_BGR = 0x08;

static const uint8_t ST7735_MADCTL_MH = 0x04;

static const uint8_t ST7735_FRMCTR1 = 0xB1;
static const uint8_t ST7735_FRMCTR2 = 0xB2;
static const uint8_t ST7735_FRMCTR3 = 0xB3;
static const uint8_t ST7735_INVCTR = 0xB4;
static const uint8_t ST7735_DISSET5 = 0xB6;

static const uint8_t ST7735_PWCTR1 = 0xC0;
static const uint8_t ST7735_PWCTR2 = 0xC1;
static const uint8_t ST7735_PWCTR3 = 0xC2;
static const uint8_t ST7735_PWCTR4 = 0xC3;
static const uint8_t ST7735_PWCTR5 = 0xC4;
static const uint8_t ST7735_VMCTR1 = 0xC5;

static const uint8_t ST7735_PWCTR6 = 0xFC;

static const uint8_t ST7735_GMCTRP1 = 0xE0;
static const uint8_t ST7735_GMCTRN1 = 0xE1;

// clang-format off
static const uint8_t PROGMEM
  BCMD[] = {                        // Init commands for 7735B screens
    18,                             // 18 commands in list:
    ST77XX_SWRESET,   ST_CMD_DELAY, //  1: Software reset, no args, w/delay
      50,                           //     50 ms delay
    ST77XX_SLPOUT,    ST_CMD_DELAY, //  2: Out of sleep mode, no args, w/delay
      255,                          //     255 = max (500 ms) delay
    ST77XX_COLMOD,  1+ST_CMD_DELAY, //  3: Set color mode, 1 arg + delay:
      0x05,                         //     16-bit color
      10,                           //     10 ms delay
    ST7735_FRMCTR1, 3+ST_CMD_DELAY, //  4: Frame rate control, 3 args + delay:
      0x00,                         //     fastest refresh
      0x06,                         //     6 lines front porch
      0x03,                         //     3 lines back porch
      10,                           //     10 ms delay
    ST77XX_MADCTL,  1,              //  5: Mem access ctl (directions), 1 arg:
      0x08,                         //     Row/col addr, bottom-top refresh
    ST7735_DISSET5, 2,              //  6: Display settings #5, 2 args:
      0x15,                         //     1 clk cycle nonoverlap, 2 cycle gate
                                    //     rise, 3 cycle osc equalize
      0x02,                         //     Fix on VTL
    ST7735_INVCTR,  1,              //  7: Display inversion control, 1 arg:
      0x0,                          //     Line inversion
    ST7735_PWCTR1,  2+ST_CMD_DELAY, //  8: Power control, 2 args + delay:
      0x02,                         //     GVDD = 4.7V
      0x70,                         //     1.0uA
      10,                           //     10 ms delay
    ST7735_PWCTR2,  1,              //  9: Power control, 1 arg, no delay:
      0x05,                         //     VGH = 14.7V, VGL = -7.35V
    ST7735_PWCTR3,  2,              // 10: Power control, 2 args, no delay:
      0x01,                         //     Opamp current small
      0x02,                         //     Boost frequency
    ST7735_VMCTR1,  2+ST_CMD_DELAY, // 11: Power control, 2 args + delay:
      0x3C,                         //     VCOMH = 4V
      0x38,                         //     VCOML = -1.1V
      10,                           //     10 ms delay
    ST7735_PWCTR6,  2,              // 12: Power control, 2 args, no delay:
      0x11, 0x15,
    ST7735_GMCTRP1,16,              // 13: Gamma Adjustments (pos. polarity), 16 args + delay:
      0x09, 0x16, 0x09, 0x20,       //     (Not entirely necessary, but provides
      0x21, 0x1B, 0x13, 0x19,       //      accurate colors)
      0x17, 0x15, 0x1E, 0x2B,
      0x04, 0x05, 0x02, 0x0E,
    ST7735_GMCTRN1,16+ST_CMD_DELAY, // 14: Gamma Adjustments (neg. polarity), 16 args + delay:
      0x0B, 0x14, 0x08, 0x1E,       //     (Not entirely necessary, but provides
      0x22, 0x1D, 0x18, 0x1E,       //      accurate colors)
      0x1B, 0x1A, 0x24, 0x2B,
      0x06, 0x06, 0x02, 0x0F,
      10,                           //     10 ms delay
    ST77XX_CASET,   4,              // 15: Column addr set, 4 args, no delay:
      0x00, 0x02,                   //     XSTART = 2
      0x00, 0x81,                   //     XEND = 129
    ST77XX_RASET,   4,              // 16: Row addr set, 4 args, no delay:
      0x00, 0x02,                   //     XSTART = 1
      0x00, 0x81,                   //     XEND = 160
    ST77XX_NORON,     ST_CMD_DELAY, // 17: Normal display on, no args, w/delay
      10,                           //     10 ms delay
    ST77XX_DISPON,    ST_CMD_DELAY, // 18: Main screen turn on, no args, delay
      255 },                        //     255 = max (500 ms) delay

  RCMD1[] = {                       // 7735R init, part 1 (red or green tab)
    15,                             // 15 commands in list:
    ST77XX_SWRESET,   ST_CMD_DELAY, //  1: Software reset, 0 args, w/delay
      150,                          //     150 ms delay
    ST77XX_SLPOUT,    ST_CMD_DELAY, //  2: Out of sleep mode, 0 args, w/delay
      255,                          //     500 ms delay
    ST7735_FRMCTR1, 3,              //  3: Framerate ctrl - normal mode, 3 arg:
      0x01, 0x2C, 0x2D,             //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR2, 3,              //  4: Framerate ctrl - idle mode, 3 args:
      0x01, 0x2C, 0x2D,             //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR3, 6,              //  5: Framerate - partial mode, 6 args:
      0x01, 0x2C, 0x2D,             //     Dot inversion mode
      0x01, 0x2C, 0x2D,             //     Line inversion mode
    ST7735_INVCTR,  1,              //  6: Display inversion ctrl, 1 arg:
      0x07,                         //     No inversion
    ST7735_PWCTR1,  3,              //  7: Power control, 3 args, no delay:
      0xA2,
      0x02,                         //     -4.6V
      0x84,                         //     AUTO mode
    ST7735_PWCTR2,  1,              //  8: Power control, 1 arg, no delay:
      0xC5,                         //     VGH25=2.4C VGSEL=-10 VGH=3 * AVDD
    ST7735_PWCTR3,  2,              //  9: Power control, 2 args, no delay:
      0x0A,                         //     Opamp current small
      0x00,                         //     Boost frequency
    ST7735_PWCTR4,  2,              // 10: Power control, 2 args, no delay:
      0x8A,                         //     BCLK/2,
      0x2A,                         //     opamp current small & medium low
    ST7735_PWCTR5,  2,              // 11: Power control, 2 args, no delay:
      0x8A, 0xEE,
    ST7735_VMCTR1,  1,              // 12: Power control, 1 arg, no delay:
      0x0E,
    ST77XX_INVOFF,  0,              // 13: Don't invert display, no args
    ST77XX_MADCTL,  1,              // 14: Mem access ctl (directions), 1 arg:
      0xC8,                         //     row/col addr, bottom-top refresh
    ST77XX_COLMOD,  1,              // 15: set color mode, 1 arg, no delay:
      0x05 },                       //     16-bit color

  RCMD2GREEN[] = {                  // 7735R init, part 2 (green tab only)
    2,                              //  2 commands in list:
    ST77XX_CASET,   4,              //  1: Column addr set, 4 args, no delay:
      0x00, 0x02,                   //     XSTART = 0
      0x00, 0x7F+0x02,              //     XEND = 127
    ST77XX_RASET,   4,              //  2: Row addr set, 4 args, no delay:
      0x00, 0x01,                   //     XSTART = 0
      0x00, 0x9F+0x01 },            //     XEND = 159

  RCMD2RED[] = {                    // 7735R init, part 2 (red tab only)
    2,                              //  2 commands in list:
    ST77XX_CASET,   4,              //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,                   //     XSTART = 0
      0x00, 0x7F,                   //     XEND = 127
    ST77XX_RASET,   4,              //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,                   //     XSTART = 0
      0x00, 0x9F },                 //     XEND = 159

  RCMD2GREEN144[] = {               // 7735R init, part 2 (green 1.44 tab)
    2,                              //  2 commands in list:
    ST77XX_CASET,   4,              //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,                   //     XSTART = 0
      0x00, 0x7F,                   //     XEND = 127
    ST77XX_RASET,   4,              //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,                   //     XSTART = 0
      0x00, 0x7F },                 //     XEND = 127

  RCMD2GREEN160X80[] = {            // 7735R init, part 2 (mini 160x80)
    2,                              //  2 commands in list:
    ST77XX_CASET,   4,              //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,                   //     XSTART = 0
      0x00, 0x4F,                   //     XEND = 79
    ST77XX_RASET,   4,              //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,                   //     XSTART = 0
      0x00, 0x9F },                 //     XEND = 159

  RCMD3[] = {                       // 7735R init, part 3 (red or green tab)
    4,                              //  4 commands in list:
    ST7735_GMCTRP1, 16      ,       //  1: Gamma Adjustments (pos. polarity), 16 args + delay:
      0x02, 0x1c, 0x07, 0x12,       //     (Not entirely necessary, but provides
      0x37, 0x32, 0x29, 0x2d,       //      accurate colors)
      0x29, 0x25, 0x2B, 0x39,
      0x00, 0x01, 0x03, 0x10,
    ST7735_GMCTRN1, 16      ,       //  2: Gamma Adjustments (neg. polarity), 16 args + delay:
      0x03, 0x1d, 0x07, 0x06,       //     (Not entirely necessary, but provides
      0x2E, 0x2C, 0x29, 0x2D,       //      accurate colors)
      0x2E, 0x2E, 0x37, 0x3F,
      0x00, 0x00, 0x02, 0x10,
    ST77XX_NORON,     ST_CMD_DELAY, //  3: Normal display on, no args, w/delay
      10,                           //     10 ms delay
    ST77XX_DISPON,    ST_CMD_DELAY, //  4: Main screen turn on, no args w/delay
      100 };                        //     100 ms delay
static const uint8_t ST7735_TFTWIDTH_128 = 128;   // for 1.44 and mini^M
static const uint8_t ST7735_TFTWIDTH_80 = 80;     // for mini^M
static const uint8_t ST7735_TFTHEIGHT_128 = 128;  // for 1.44" display^M
static const uint8_t ST7735_TFTHEIGHT_160 = 160;  // for 1.8" and mini display^M

// some flags for initR() :(
static const uint8_t INITR_GREENTAB = 0x00;
static const uint8_t INITR_REDTAB = 0x01;
static const uint8_t INITR_BLACKTAB = 0x02;
static const uint8_t INITR_144GREENTAB = 0x01;
static const uint8_t INITR_MINI_160X80 = 0x04;
static const uint8_t INITR_HALLOWING = 0x05;
static const uint8_t INITR_18GREENTAB = INITR_GREENTAB;
static const uint8_t INITR_18REDTAB = INITR_REDTAB;
static const uint8_t INITR_18BLACKTAB = INITR_BLACKTAB;


}  // namespace st7735
}  // namespace esphome
