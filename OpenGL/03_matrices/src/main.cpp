//#include <geGL/windows_OpenGLFunctionLoader.h>

#include <iostream>
#include <vector>
#include <SDL.h>

#include <geGL/geGL.h>
#include <geGL/StaticCalls.h>
#include <cmath>

using namespace ge::gl;

GLuint createShader(GLenum type, std::string const&src)
{
	GLuint id = glCreateShader(type);
	char const* const srcs[] = {
		src.data()
	};
	glShaderSource(id,1,srcs,nullptr);
	glCompileShader(id);

	GLint status;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE)
	{
		GLint len;
		glGetShaderiv(id,GL_INFO_LOG_LENGTH,&len);

		char* buffer = new char[len];
		//auto message = std::string('',len);

		glGetShaderInfoLog(id,len,nullptr,buffer);

		auto msg = "shader compilation failed" + std::string(buffer);

		delete[]buffer;

		throw std::runtime_error(msg);
	}

	return id;

}

GLuint createProgram(std::vector<GLuint> const&shaders)
{
	GLuint prg = glCreateProgram();

	for(auto const&shader:shaders)
		glAttachShader(prg,shader);

	glLinkProgram(prg);

	GLint status;
	glGetProgramiv(prg, GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		GLint len;
		glGetProgramiv(prg, GL_INFO_LOG_LENGTH, &len);

		char* buffer = new char[len];
		//auto message = std::string('',len);

		glGetProgramInfoLog(prg, len, nullptr, buffer);

		auto msg = "program Linking failed"+std::string(buffer);

		delete[]buffer;

		throw std::runtime_error(msg);
	}

	return prg;
}

int main(int argc, char* argv[])
{
	//init. video
	SDL_Init(SDL_INIT_EVERYTHING);

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

	out vec3 vColor;

	uniform vec2 position = vec2(0);
	uniform vec2 scale    = vec2(1);
	uniform float alpha   = 0.f;

	void main()
	{
		vColor = vec3(1,0,0);
		if(gl_VertexID == 0){vColor = vec3(1,0,0);gl_Position = vec4( vec2(position) + vec2(			0,		  0)*scale  ,0,1);}
		if(gl_VertexID == 1){vColor = vec3(0,1,0);gl_Position = vec4( vec2(position) + vec2(+cos(alpha),+sin(alpha))*scale  ,0,1);}
		if(gl_VertexID == 2){vColor = vec3(0,0,1);gl_Position = vec4( vec2(position) + vec2(-sin(alpha),+cos(alpha))*scale  ,0,1);}

	}
	

	).";

	std::string const fsSrc = R".(
	
	#version 460

	//location specify the id of the buffer
	layout(location = 0)out vec4 fColor;
	
	in vec3 vColor;

	uniform float saturation = 0.5f;//饱和度

	float intensity(vec3 color)
	{
		return (color.x + color.y +color.z)/3;

	 }	

	void main()
	{
		
		vec3 finalColor = vColor*saturation + vec3(intensity(vColor))*(1-saturation);
		
		fColor = vec4(finalColor,0); //white
		//fColor = vec4(1,1,1,1);
	}
	

	).";


	GLuint prg;
	prg = createProgram({
		createShader(GL_VERTEX_SHADER,vsSrc),
		createShader(GL_FRAGMENT_SHADER,fsSrc)
		});

	//locations
	GLuint saturationLocation =  glGetUniformLocation(prg,"saturation");
	GLuint positionLocation	  =  glGetUniformLocation(prg,"position");
	GLuint scaleLocation	  =  glGetUniformLocation(prg,"scale");
	GLuint alphaLocation      =  glGetUniformLocation(prg, "alpha");

	float saturation  =  0.5f;
	float position[2] =  {0.f,0.f};
	float scale[2]    =  {1.f,1.f};
	float alpha       =  0.f;

	bool running = true;
	//main loop
	while (running)
	{
		//event handling
		SDL_Event event;

		//event loop
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)running = false;
			if (event.type = SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_z)saturation += 0.1;
				if (event.key.keysym.sym == SDLK_x)saturation -= 0.1;
				glProgramUniform1f(prg, saturationLocation, saturation);
					//std::cout << "saturation:" << saturation << std::endl;
				
				if (event.key.keysym.sym == SDLK_w)position[1] += 0.01;
				if (event.key.keysym.sym == SDLK_s)position[1] -= 0.01;
				if (event.key.keysym.sym == SDLK_a)position[0] -= 0.01;
				if (event.key.keysym.sym == SDLK_d)position[0] += 0.01;
				glProgramUniform2fv(prg, positionLocation,1,position);

				if (event.key.keysym.sym == SDLK_t)scale[1] += 0.01;
				if (event.key.keysym.sym == SDLK_g)scale[1] -= 0.01;
				if (event.key.keysym.sym == SDLK_f)scale[0] -= 0.01;
				if (event.key.keysym.sym == SDLK_h)scale[0] += 0.01;
				glProgramUniform2fv(prg, scaleLocation, 1, scale);

				if (event.key.keysym.sym == SDLK_i)alpha += 0.003;
				if (event.key.keysym.sym == SDLK_k)alpha -= 0.003;
				glProgramUniform1f(prg, alphaLocation, alpha);
			}	
				

		}

		#define GL_COLOR_BUFFER_BIT     0x00004000
		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);//clear buffer

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
