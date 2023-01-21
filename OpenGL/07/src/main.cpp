//#include <geGL/windows_OpenGLFunctionLoader.h>

#include <iostream>
#include <vector>
#include <SDL.h>

#include <geGL/geGL.h>
#include <geGL/StaticCalls.h>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/mat3x3.hpp>
#include<glm/gtx/transform.hpp>

using namespace ge::gl;

GLuint createShader(GLenum type, std::string const& src)
{
	GLuint id = glCreateShader(type);
	char const* const srcs[] = {
		src.data()
	};
	glShaderSource(id, 1, srcs, nullptr);
	glCompileShader(id);

	GLint status;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE)
	{
		GLint len;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);

		char* buffer = new char[len];
		//auto message = std::string('',len);

		glGetShaderInfoLog(id, len, nullptr, buffer);

		auto msg = "shader compilation failed" + std::string(buffer);

		delete[]buffer;

		throw std::runtime_error(msg);
	}

	return id;

}

GLuint createProgram(std::vector<GLuint> const& shaders)
{
	GLuint prg = glCreateProgram();

	for (auto const& shader : shaders)
		glAttachShader(prg, shader);

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

		auto msg = "program Linking failed" + std::string(buffer);

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

	int windowWidth = 1000;
	int windowHeight = 1000;
	auto window = SDL_CreateWindow("SDL_Window", 0, 0, windowWidth, windowHeight, SDL_WINDOW_OPENGL);

	//create opengl context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	auto context = SDL_GL_CreateContext(window);

	//SDL_DestroyWindow(SDL_Window * window);

	ge::gl::init();

	//shader program
	// 





	//empty vertex array object(empty vertex puller setting)


	GLuint vbo;
	glCreateBuffers(1, &vbo);

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 color;
	};

	std::vector<Vertex>vertices;

	uint32_t nx = 20;
	uint32_t ny = 20;

	for (uint32_t iy = 0;iy < ny;++iy)
	{
		for (uint32_t ix = 0;ix < nx;++ix)
		{
			Vertex vertex;

			float xangleNorm = (float)ix / (float)(nx);
			float yangleNorm = (float)iy / (float)(ny - 1);
			vertex.color = glm::vec3(xangleNorm, yangleNorm, 0);

			float xangle = xangleNorm * glm::pi<float>() * 2.f;
			float yangle = yangleNorm * glm::pi<float>();

			float x = glm::cos(xangle) * glm::sin(yangle);
			float y =				   - glm::cos(yangle);
			float z = glm::sin(xangle) * glm::sin(yangle);

			vertex.position = glm::vec3(x, y, z);

			vertices.push_back(vertex);
		}
	}

	glNamedBufferData(vbo, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_COPY);


	struct Face
	{
		uint32_t triangleA[3];
		uint32_t triangleB[3];

	};



	//std::vector<int>elements;
	std::vector<Face>faces;

	for (uint32_t iy = 0;iy < ny-1;++iy)
	{
		for (uint32_t ix = 0;ix < nx;++ix)
		{
			Face face;

			face.triangleA[0] = (iy    ) * nx + (ix         );
			face.triangleA[1] = (iy    ) * nx + ((ix + 1)%nx);
			face.triangleA[2] = (iy + 1) * nx + (ix         );

			face.triangleB[0] = (iy + 1) * nx + (ix    );
			face.triangleB[1] = (iy	   ) * nx + ((ix + 1)%nx);
			face.triangleB[2] = (iy + 1) * nx + ((ix + 1)%nx);

			faces.push_back(face);
		}
	}

	//for (auto& f : faces)
	//	f.print();

	GLuint ebo;
	glCreateBuffers(1, &ebo);
	glNamedBufferData(ebo, sizeof(Face) * faces.size(), faces.data(), GL_DYNAMIC_COPY);


	GLuint vao;
	glCreateVertexArrays(1, &vao);

	glEnableVertexArrayAttrib(vao, 0);
	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayVertexBuffer(vao, 0, vbo, sizeof(float) * 0, sizeof(float) * 6);
	glVertexArrayAttribBinding(vao, 0, 0);

	glEnableVertexArrayAttrib(vao, 2);
	glVertexArrayAttribFormat(vao, 2, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayVertexBuffer(vao, 2, vbo, sizeof(float) * 3, sizeof(float) * 6);
	glVertexArrayAttribBinding(vao, 2, 2);

	glVertexArrayElementBuffer(vao, ebo);

	std::string const vsSrc = R".(
	
	//GLSL

	#version 460
	
	layout(location = 0)in vec3 position;
	layout(location = 2)in vec3 color;

	out vec3 vColor;

	uniform mat4 modelMatrix	  = mat4(1);
	uniform mat4 viewMatrix		  = mat4(1);
	uniform mat4 projectionMatrix = mat4(1);

	void main()
	{
      
		vColor = color;
		gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position,1);
		//gl_Position =  modelMatrix * vec4(position,1);

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
	GLuint saturationLocation       = glGetUniformLocation(prg, "saturation");
	GLuint modelMatrixLocation      = glGetUniformLocation(prg, "modelMatrix");
	GLuint viewMatrixLocation       = glGetUniformLocation(prg, "viewMatrix");
	GLuint projectionMatrixLocation = glGetUniformLocation(prg, "projectionMatrix");

	float saturation = 0.5f;
	glm::vec3 position = glm::vec3(0.3f);
	//float position[2] = { 0.f,0.f };
	float scale[2] = { 1.f,1.f };
	float alpha = 0.f;

	float camXAngle = 0.f;
	float camYAngle = 0.f;
	float camDistance = 3.f;

	bool wireframe = false;

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

				if (event.key.keysym.sym == SDLK_w)position[2] += 0.01;
				if (event.key.keysym.sym == SDLK_s)position[2] -= 0.01;
				if (event.key.keysym.sym == SDLK_a)position[0] -= 0.01;
				if (event.key.keysym.sym == SDLK_d)position[0] += 0.01;

				if (event.key.keysym.sym == SDLK_SPACE)position[1] -= 0.01;
				if (event.key.keysym.sym == SDLK_LSHIFT)position[1] += 0.01;

				if (event.key.keysym.sym == SDLK_t)scale[1] += 0.01;
				if (event.key.keysym.sym == SDLK_g)scale[1] -= 0.01;
				if (event.key.keysym.sym == SDLK_f)scale[0] -= 0.01;
				if (event.key.keysym.sym == SDLK_h)scale[0] += 0.01;

				if (event.key.keysym.sym == SDLK_m)wireframe = !wireframe;

				if (event.key.keysym.sym == SDLK_i)alpha += 0.003;
				if (event.key.keysym.sym == SDLK_k)alpha -= 0.003;

				glm::mat4 modelMatrix;

				//auto T = glm::translate(glm::mat4(1.f), glm::vec3(position[0], position[1], 0.f));
				auto T = glm::translate(glm::mat4(1.f), position);
				auto S = glm::scale    (glm::mat4(1.f), glm::vec3(scale[0], scale[1], 1.f));
				auto R = glm::rotate   (glm::mat4(1.f), alpha, glm::vec3(0.f, 0.f, 1.f));
				modelMatrix = T * R * S;
				glProgramUniformMatrix4fv(prg, modelMatrixLocation, 1, GL_FALSE, (float*)&modelMatrix);
			}
			if (event.type == SDL_MOUSEMOTION)
			{
				//std::cerr << "xrel:" << event.motion.xrel << std::endl;
				//std::cerr << "yrel:" << event.motion.yrel << std::endl;

				if (event.motion.state & SDL_BUTTON_LMASK )
				{
					camXAngle += event.motion.xrel * 0.01f;
					camYAngle += event.motion.yrel * 0.01f;
				}
				if (event.motion.state & SDL_BUTTON_RMASK)
				{
					camDistance += event.motion.yrel * 0.1f;
				}
			}

		}

		auto projectionMatrix = glm::perspective(glm::half_pi<float>(), (float)windowWidth / (float)windowHeight, 0.1f, 1000.f);
		
		//upload this camera to the GPU
		glProgramUniformMatrix4fv(prg, projectionMatrixLocation, 1, GL_FALSE, (float*)&projectionMatrix);

		glm::vec3 camPosition;
		camPosition.x = camDistance * glm::cos(camXAngle) * glm::cos(camYAngle);
		camPosition.y = camDistance * glm::sin(camYAngle);
		camPosition.z = camDistance * glm::sin(camXAngle) * glm::cos(camYAngle);



		auto viewMatrix = glm::lookAt(camPosition,glm::vec3(0,0,0),glm::vec3(0,1,0));
		glProgramUniformMatrix4fv(prg, viewMatrixLocation, 1, GL_FALSE, (float*)&viewMatrix);

		glEnable(GL_DEPTH_TEST);
		#define GL_COLOR_BUFFER_BIT     0x00004000
		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//clear buffer

		glBindVertexArray(vao);

		glPointSize(10);

		if(wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glUseProgram(prg);

		//glDrawArrays(GL_TRIANGLES,0,3);
		glDrawElements(GL_TRIANGLES, nx * ny * 2 * 3, GL_UNSIGNED_INT, nullptr);
		//glDrawArrays(GL_TRIANGLES,0, nx * ny );

		glBindVertexArray(0);
		SDL_GL_SwapWindow(window);
	}




	//SDL_GL_DeleteContext(context);

	//SDL_DestroyWindow(window);

	return 0;
}
