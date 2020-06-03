#pragma once

#include <iostream>

class RenderContext
{
public:
	RenderContext();
	~RenderContext();

	int width;
	int height;
	uint32_t* frameBuffer;
	float* depthBuffer;
};

