#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>
#include <stdlib.h>
#include <string.h>

#include "./memory.h"

/**
 * @brief 2D Vector.
 *
 */
typedef struct {
    float x;
    float y;
} vec2_t;

/**
 * @brief RGB Color.
 *
 */
typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} color_t;

/**
 * @brief 2C02 color palette look-up table for the NES.
 *
 */
static const color_t COLOR_PALETTE[64] = {
    {0x55, 0x55, 0x55}, {0x00, 0x17, 0x73}, {0x00, 0x07, 0x86},
    {0x2e, 0x05, 0x78}, {0x59, 0x02, 0x4d}, {0x72, 0x00, 0x11},
    {0x6e, 0x00, 0x00}, {0x4c, 0x08, 0x00}, {0x17, 0x1b, 0x00},
    {0x00, 0x2a, 0x00}, {0x00, 0x31, 0x00}, {0x00, 0x2e, 0x08},
    {0x00, 0x26, 0x45}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00}, {0xa5, 0xa5, 0xa5}, {0x00, 0x57, 0xc6},
    {0x22, 0x3f, 0xe5}, {0x6e, 0x28, 0xd9}, {0xae, 0x1a, 0xa6},
    {0xd2, 0x17, 0x59}, {0xd1, 0x21, 0x07}, {0xa7, 0x37, 0x00},
    {0x63, 0x51, 0x00}, {0x18, 0x67, 0x00}, {0x00, 0x72, 0x00},
    {0x00, 0x73, 0x31}, {0x00, 0x6a, 0x84}, {0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0xfe, 0xff, 0xff},
    {0x2f, 0xa8, 0xff}, {0x5d, 0x81, 0xff}, {0x9c, 0x70, 0xff},
    {0xf7, 0x72, 0xff}, {0xff, 0x77, 0xbd}, {0xff, 0x7e, 0x75},
    {0xff, 0x8a, 0x2b}, {0xcd, 0xa0, 0x00}, {0x81, 0xb8, 0x02},
    {0x3d, 0xc8, 0x30}, {0x12, 0xcd, 0x7b}, {0x0d, 0xc5, 0xd0},
    {0x3c, 0x3c, 0x3c}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
    {0xfe, 0xff, 0xff}, {0xa4, 0xde, 0xff}, {0xb1, 0xc8, 0xff},
    {0xcc, 0xbe, 0xff}, {0xf4, 0xc2, 0xff}, {0xff, 0xc5, 0xea},
    {0xff, 0xc7, 0xc9}, {0xff, 0xcd, 0xaa}, {0xef, 0xd6, 0x96},
    {0xd0, 0xe0, 0x95}, {0xb3, 0xe7, 0xa5}, {0x9f, 0xea, 0xc3},
    {0x9a, 0xe8, 0xe6}, {0xaf, 0xaf, 0xaf}, {0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00},
};

/**
 * @brief 2D Display.
 *
 */
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

    memory_t bitmap;
    vec2_t size;
} display_t;

/**
 * @brief Create a 2D display.
 *
 * @param width  Logical width of the display.
 * @param height Logical height of the display.
 * @param title  Title of the window.
 * @return display_t*
 */
display_t *create_display(unsigned width, unsigned height, const char *title);

/**
 * @brief Destroy a 2D display.
 *
 * @param display
 */
void destroy_display(display_t *display);

/**
 * @brief Refresh the display.
 *
 * @param display
 */
void refresh_display(display_t *display);

/**
 * @brief Fill the display with a color.
 *
 * @param display
 * @param color
 */
void render_fill(display_t *display, color_t color);

/**
 * @brief Draw a pixel at a position.
 *
 * @param display
 * @param position
 * @param color
 */
void render_pixel(display_t *display, vec2_t position, color_t color);

#endif
