#pragma once

#include <cstdint>
#include <cmath>

#include "vec3.hpp"

namespace Minecrap
{

// Window manager
constexpr int C_WINDOW_WIDTH	= 1080;
constexpr int C_WINDOW_HEIGHT	= 720;

const Math::vec3 C_BK_COLOR {0.2f, 0.2f, 0.2f};


// Rendering Components
constexpr float C_NEAR_PLANE 	= 0.5f;
constexpr float C_FAR_PLANE 	= 500.0f;
constexpr float C_FOV		= M_PI / 2.0f;
constexpr float C_ASPECT_RATIO	= static_cast<float>(C_WINDOW_WIDTH) / static_cast<float>(C_WINDOW_HEIGHT); 


// Chunks
constexpr int C_CHUNK_SIDE	= 16;
constexpr int C_CHUNK_VOLUME	= C_CHUNK_SIDE * C_CHUNK_SIDE * C_CHUNK_SIDE;
constexpr int C_CHUNK_TO_LOAD	= 12;


// Controller
constexpr float C_DELTA_POSITION 	= 0.5f;
constexpr float C_DELTA_ANGLE		= 0.025f;
constexpr float C_OFFSET_ANGLE		= M_PI / 16.0f;

// Debugger
constexpr int C_AVERAGE_WINDOW_WIDTH	= 20;

// ChunkManager
constexpr int C_MAX_CHUNKS 	= 256;

}
