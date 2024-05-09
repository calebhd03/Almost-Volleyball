#include <cmath>
typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;

u32 color_aqua = 0x00FFFF00;
u32 color_black = 0x00000000;
u32 color_cream = 0x00F0FBFF;
u32 color_grey = 0x00808080;
u32 color_pink = 0x00FF00FF;
u32 color_green = 0x00008000;
u32 color_lime_green = 0x0000FF00;
u32 color_maroon = 0x00000080;
u32 color_navy = 0x00800000;
u32 color_olive_green = 0x00008080;
u32 color_purple = 0x00FF00FF;
u32 color_blue = 0x000000FF;
u32 color_silver = 0x00C0C0C0;
u32 color_teal = 0x00808000;
u32 color_white = 0xffffff;
u32 color_red = 0xff0000;
#define global_variable static
#define internal static

inline int
clamp(int min, int val, int max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

inline double 
distance(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}