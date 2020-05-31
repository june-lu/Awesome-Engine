#include "SDLInterface.h"

SDLInterface::SDLInterface(const char* windowName, int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		LogError("Could not initialize SDL");
	}

	CreateWindow(windowName, width, height);
	CreateRenderer();
}


SDLInterface::~SDLInterface()
{
}

void SDLInterface::CreateWindow(const char* windowName, int width, int height)
{
	screenWidth = width;
	screenHeight = height;
	mainWindow = SDL_CreateWindow(windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
	if (mainWindow == NULL)
	{
		LogError("Could not create window");
		Quit(1);
	}


}
void SDLInterface::DestoryWindow(SDL_Window* window)
{
	SDL_DestroyWindow(window);
}

void SDLInterface::LogError(const char* msg)
{
	std::cout << ("%s : %s", msg, SDL_GetError()) << std::endl;
}

void SDLInterface::Quit(int code)
{
	SDL_Quit();
	exit(code);
}

void SDLInterface::handleEvents()
{
	// Our SDL event placeholder.  
	SDL_Event event;
	//Grab all the events off the queue.  
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_KEYDOWN:
			handleKeyDownEvents(&event.key.keysym);
			break;
		case SDL_QUIT:
			// Handle quit requests (like Ctrl-c).  
			Quit(0);
			break;
		default:
			break;
		}
	}
}

void SDLInterface::Render()
{
	SDL_RenderPresent(renderer);
}

void SDLInterface::handleKeyDownEvents(SDL_Keysym* keysym)
{
	switch (keysym->sym)
	{
	case SDLK_ESCAPE:
		Quit(0);
		break;
	case SDLK_F1:
		ToggleFullscreen();
		break;
	default:
		break;
	}
}

void SDLInterface::ToggleFullscreen()
{
	if (windowed)
	{
		int result = SDL_SetWindowFullscreen(mainWindow, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
		if (result < 0) return;
		windowed = false;
	}
	else
	{
		SDL_SetWindowSize(mainWindow, screenWidth, screenHeight);
		windowed = true;
	}

}

void SDLInterface::DrawPixel(Color* color, int posX, int posY)
{
	SetDrawColor(color);
	SDL_RenderDrawPoint(renderer, posX, posY);
}

void SDLInterface::CreateRenderer()
{
	renderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
}

void SDLInterface::SetDrawColor(Color* color)
{
	SDL_SetRenderDrawColor(renderer, color->GetUintR(), color->GetUintG(), color->GetUintB(), color->GetUintA());
}

void SDLInterface::RenderClear(Color* color)
{
	SetDrawColor(color);
	SDL_RenderClear(renderer);
}

//Bresenham’s Line Drawing Algorithm
void SDLInterface::DrawLine(Color* color, int x0, int y0, int x1, int y1)
{
	//1 attempt
	/*for (float t = 0.0; t < 1.0; t += 0.01)
	{
		int x = x0 + (x1 - x0) * t;
		int y = y0 + (y1 - y0) * t;
		DrawPixel(color, x, y);
	}*/

	//2 attempt
	/*for (int x = x0; x <= x1; x++)
	{
		float t = Mathf::Abs((x - x0) / (float)(x1 - x0));
		int y = y0 + (y1 - y0) * t;
		DrawPixel(color, x, y);
	}*/

	//3 attempt
	bool steep = false;
	if (Mathf::Abs(x1 - x0) < Mathf::Abs(y1 - y0))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}

	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;

	//4 attempt
	//通过误差提高运行效率
	int dyError = Mathf::Abs(dy * 2);
	int error = 0;

	float y = y0;
	for (int x = x0; x <= x1; x++)
	{
		/*float t = Mathf::Abs((x - x0) / (float)(x1 - x0));
		int y = y0 + (y1 - y0) * t;*/

		if (steep)
			DrawPixel(color, y, x);
		else
			DrawPixel(color, x, y);

		error += dyError;
		if (error > dx)
		{
			y += (y1 > y0) ? 1 : -1;
			error -= 2 * dx;
		}
	}
}

void SDLInterface::DrawLine(Color* color, Vector2f start, Vector2f end)
{
	SDLInterface::DrawLine(color, start.x, start.y, end.x, end.y);
}

//Draw Triangle by line sweeping
void SDLInterface::DrawTriangleByLineSweeping(Color* color, Vector2f* pts)
{
	// sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) 
	Vector2f pos1 = pts[0];
	Vector2f pos2 = pts[1];
	Vector2f pos3 = pts[2];


	if (pos1.y > pos2.y) std::swap(pos1, pos2);
	if (pos1.y > pos3.y) std::swap(pos1, pos3);
	if (pos2.y > pos3.y) std::swap(pos2, pos3);

	/*SDLInterface::DrawLine(&Color::white, pos1, pos2);
	SDLInterface::DrawLine(&Color::white, pos2, pos3);
	SDLInterface::DrawLine(&Color::white, pos3, pos1);*/

	int total_height = pos3.y - pos1.y;
	int segment_height; float beta; Vector2f B;
	for (int y = pos1.y; y <= pos3.y; y++)
	{
		bool second_half = y > pos2.y - pos1.y || pos2.y == pos1.y;
		float alpha = (float)(y - pos1.y) / total_height;
		Vector2f A = pos1 + (pos3 - pos1)*alpha;
		segment_height = second_half ? pos3.y - pos2.y : pos2.y - pos1.y;
		beta = (float)(y - (second_half ? pos2.y : pos1.y)) / segment_height;
		B = (second_half ? pos2 + (pos3 - pos2)*beta : pos1 + (pos2 - pos1)*beta);

		if (A.x > B.x) std::swap(A, B);
		for (int x = A.x; x <= B.x; x++)
		{
			DrawPixel(color, x, y);
		}
	}
}

void SDLInterface::DrawTriangleByBarycentricCoordinates(Color* color, Vector2f* pts)
{
	Vector2f boxMin(screenWidth - 1, screenHeight - 1);
	Vector2f boxMax(0, 0);
	Vector2f clamp(screenWidth - 1, screenHeight - 1);

	for (int i = 0; i < 3; i++)
	{
		boxMin.x = Mathf::Max(0, Mathf::Min(boxMin.x, pts[i].x));
		boxMin.y = Mathf::Max(0, Mathf::Min(boxMin.y, pts[i].y));

		boxMax.x = Mathf::Min(clamp.x, Mathf::Max(boxMax.x, pts[i].x));
		boxMax.y = Mathf::Min(clamp.y, Mathf::Max(boxMax.y, pts[i].y));
	}

	Vector2f pos;
	for (pos.x = boxMin.x; pos.x < boxMax.x; pos.x++)
	{
		for (pos.y = boxMin.y; pos.y < boxMax.y; pos.y++)
		{
			Vector3f bcCoord = Vector3f::Barycentric(pts, pos);
			if (bcCoord.x < 0 || bcCoord.y < 0 || bcCoord.z < 0)continue;

			DrawPixel(color, pos.x, pos.y);
		}
	}
}