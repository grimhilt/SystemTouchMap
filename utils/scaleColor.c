#include <stdint.h>

uint32_t scaleColor(int min, int max, int value, uint32_t min_color,
                    uint32_t max_color)
{
    // Calculate the range of the input values
    float range_size = (float) (max - min);

    // Calculate the distance between the value and the minimum of the range
    float distance_from_min = (float) (value - min);

    // Calculate the interpolation factor between the two colors
    float factor = distance_from_min / range_size;

    // Extract the red, green, and blue components of the minimum color
    uint8_t min_red = (min_color >> 16) & 0xFF;
    uint8_t min_green = (min_color >> 8) & 0xFF;
    uint8_t min_blue = min_color & 0xFF;

    // Extract the red, green, and blue components of the maximum color
    uint8_t max_red = (max_color >> 16) & 0xFF;
    uint8_t max_green = (max_color >> 8) & 0xFF;
    uint8_t max_blue = max_color & 0xFF;

    // Interpolate between the two colors using the factor
    uint8_t red = (uint8_t) (min_red + factor * (max_red - min_red));
    uint8_t green = (uint8_t) (min_green + factor * (max_green - min_green));
    uint8_t blue = (uint8_t) (min_blue + factor * (max_blue - min_blue));

    uint32_t color = (red << 16) | (green << 8) | blue | 0xFF000000;
    return color;
}