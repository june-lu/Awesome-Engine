#pragma once

#include <iostream>
#include <vector>
#include "Base/Texture.h"

class RenderContext
{
public:
	RenderContext();
	~RenderContext();

	int width;
	int height;
	std::vector < uint32_t> frameBuffer;
	std::vector < float > depthBuffer;
	Texture texture;
};

