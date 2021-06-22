#include "ColorRGBA.h"


Vec4 ColorRGBA::RED = { 1.0f, 0.0f, 0.0f, 1.0f };
Vec4 ColorRGBA::BLUE = { 0.0f, 0.0f, 1.0f, 1.0f };
Vec4 ColorRGBA::CYAN = { 0.0f, 1.0f, 1.0f, 1.0f };
Vec4 ColorRGBA::GREEN = { 0.0f, 1.0f, 0.0f, 1.0f };
Vec4 ColorRGBA::YELLOW = RED + GREEN;
Vec4 ColorRGBA::MAGENTA = RED + BLUE;
Vec4 ColorRGBA::WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };
Vec4 ColorRGBA::BLACK = { 0.0f, 0.0f, 0.0f, 1.0f };
Vec4 ColorRGBA::ORANGE = { 1.0f, 0.5f, 0.0f, 1.0f };
Vec4 ColorRGBA::PURPLE = { 0.5f, 0.0f, 1.0f, 1.0f };