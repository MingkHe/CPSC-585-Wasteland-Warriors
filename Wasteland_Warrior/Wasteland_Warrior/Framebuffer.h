#include "glad/glad.h"
#include <GLFW/glfw3.h>

struct Framebuffer {
	GLuint id;
	GLuint colorTextureID;
	GLuint depthTextureID;
};

GLuint createEmptyTexture(int width, int height, GLenum format, GLenum type);
GLuint createEmptyTexture(int width, int height, GLenum format, GLenum type, GLenum format2);

Framebuffer createFramebuffer(int width, int height);
