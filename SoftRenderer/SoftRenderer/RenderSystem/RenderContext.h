#pragma once

#include <iostream>
#include <vector>

class RenderContext
{
public:
	RenderContext();
	~RenderContext();

	int width;
	int height;
	uint32_t* frameBuffer;
	std::vector < float > depthBuffer;
};

