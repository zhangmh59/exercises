#include <iostream>

#include <SDL.h>

#include <geGL/geGL.h>
#include <geGL/StaticCalls.h>

using namespace ge::gl;

int main(int argc, char* argv[])
{

	SDL_Init(SDL_INIT_EVERYTHING);//init. video

	//create window
	auto window = SDL_CreateWindow("SDL_Window",500,500,1000,500,SDL_WINDOW_OPENGL);

	//create opengl context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	auto context = SDL_GL_CreateContext(window);

	//SDL_DestroyWindow(SDL_Window * window);
	
	ge::gl::init();
	/*
	using GLCLEAR = void(*)(uint32_t);
	using GLCLEARCOLOR = void(*)(float, float, float, float);


	GLCLEAR      glClear = (GLCLEAR)SDL_GL_GetProcAddress("glClear");
	GLCLEARCOLOR glClearColor = (GLCLEARCOLOR)SDL_GL_GetProcAddress("glClearColor");
	*/

	bool running = true;
	//main loop
	while (running)
	{
		//event handling
		SDL_Event event;

		//event loop
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				running = false;
		}

		#define GL_COLOR_BUFFER_BIT     0x00004000
		glClearColor(0.0f, 1.0f, 0.0f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(window);
	}




	SDL_GL_DeleteContext(context);

	SDL_DestroyWindow(window);

	return 0;
}
