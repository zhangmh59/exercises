//#include <geGL/windows_OpenGLFunctionLoader.h>

#include <iostream>
#include <vector>
#include <SDL.h>

#include <geGL/geGL.h>
#include <geGL/StaticCalls.h>

using namespace ge::gl;

GLuint createShader(GLenum type, std::string const&src)
{
	GLuint id = glCreateShader(type);
	char const* const srcs[] = {
		src.data()
	};
	glShaderSource(id,1,srcs,nullptr);
	glCompileShader(id);

	return id;

}

GLuint createProgram(std::vector<GLuint> const&shaders)
{
	GLuint prg = glCreateProgram();

	for(auto const&shader:shaders)
		glAttachShader(prg,shader);

	glLinkProgram(prg);

	return prg;
}

int main(int argc, char* argv[])
{

	SDL_Init(SDL_INIT_EVERYTHING);//init. video

	//create window
	auto window = SDL_CreateWindow("SDL_Window",0,0,1000,500,SDL_WINDOW_OPENGL);

	//create opengl context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	auto context = SDL_GL_CreateContext(window);

	//SDL_DestroyWindow(SDL_Window * window);
	
	ge::gl::init();

	//shader program
	// 
	




	//empty vertex array object

	GLuint vao;
	glCreateVertexArrays(1,&vao);

	

	std::string const vsSrc = R".(
	
	//GLSL
	#version 460

	void main()
	{

		if(gl_VertexID == 0)gl_Position = vec4(0,0,0,1);
		if(gl_VertexID == 1)gl_Position = vec4(1,0,0,1);
		if(gl_VertexID == 2)gl_Position = vec4(0,1,0,1);

	}
	

	).";

	std::string const fsSrc = R".(
	
	#version 460

	layout(location = 0)out vec4 fcolor;

	void main()
	{

		fcolor = vec4(1);

	}
	

	).";


	GLuint prg;
	prg = createProgram({
		createShader(GL_VERTEX_SHADER,vsSrc),
		createShader(GL_FRAGMENT_SHADER,fsSrc)
		});


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

		glBindVertexArray(vao);

		glUseProgram(prg);

		glDrawArrays(GL_TRIANGLES,0,3);
		//glDrawElements();

		SDL_GL_SwapWindow(window);
	}




	//SDL_GL_DeleteContext(context);

	//SDL_DestroyWindow(window);

	return 0;
}
