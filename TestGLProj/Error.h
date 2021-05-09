#ifndef ERROR_H
#define ERROR_H
/*
 * Error.h
 * GitHub link: https://github.com/shejan0/BruhComputerGraphicsProject
 * Authors:
 *    Shejan Shuza
 *    Gabriel Vidaurri
 *    Joseph Daau
 *    Tyler Everett
 *    Christopher Urista
 * Description: Inline definitions of checkError and MessageCallback that allows consistent implementations of Error checking across the program
 *
 *
 *
 *
 */
#include <GL/freeglut.h>
inline void checkError(const char* functionName) //inline means it can be defined here and used in any include location
{
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) { //loop until the error codes is removed.
		std::cerr << "GL error 0x" << std::hex<<error << " detected in " << functionName << std::endl;
		switch (error) {
		case GL_INVALID_ENUM:
			std::cerr << "GL_INVALID_ENUM" << std::endl;
			break;
		case GL_INVALID_VALUE:
			std::cerr << "GL_INVALID_VALUE" << std::endl;
			break;
		case GL_INVALID_OPERATION:
			std::cerr << "GL_INVALID_OPERATION" << std::endl;
			break;
		case GL_STACK_OVERFLOW:
			std::cerr << "GL_STACK_OVERFLOW" << std::endl;
			break;
		case GL_STACK_UNDERFLOW:
			std::cerr << "GL_STACK_UNDERFLOW" << std::endl;
			break;
		case GL_OUT_OF_MEMORY:
			std::cerr << "GL_OUT_OF_MEMORY" << std::endl;
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
			break;
		case GL_TABLE_TOO_LARGE:
			std::cerr << "GL_TABLE_TOO_LARGE" << std::endl;
			break;
		}
	}
}
inline void GLAPIENTRY MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam){
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR_ARB ? "** GL ERROR **" : ""),
		type, severity, message);
}
#endif