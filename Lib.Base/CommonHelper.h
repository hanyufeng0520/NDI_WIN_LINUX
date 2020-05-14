#pragma once
#include <vector>
#include <memory>
#include <string>

std::string		GetAppPathA();

//version 1
//http://www.fourcc.org/fccyvrgb.php
#define CLIP(X) ((X) > 255 ? 255 : (X) < 0 ? 0 : X)
#define RGB2Y(R, G, B) CLIP(0.299 * (R) + 0.587 * (G) + 0.114 * (B) )
#define RGB2U(R, G, B) CLIP(-0.169 * (R) - 0.331 * (G) + 0.499 * (B) + 128)
#define RGB2V(R, G, B) CLIP(0.499 * (R) - 0.418 * (G) - 0.0813 * (B) + 128)

// YUV -> RGB
#define YUV2R(Y, U, V) CLIP((298 * (Y - 16) + 516 * (U - 128) + 128) >> 8)
#define YUV2G(Y, U, V) CLIP((298 * (Y - 16) - 100 * (U - 128) - 208 * (V - 128) + 128) >> 8)
#define YUV2B(Y, U, V) CLIP((298 * (Y - 16) + 409 * (V - 128) + 128) >> 8)