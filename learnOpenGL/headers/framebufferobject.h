#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

class Framebuffer {
public:
	Framebuffer(int x,int y) : width(x), height(y)
	{
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	}

	void ColorAttach(GLint internalformat,GLenum format,GLenum type = GL_UNSIGNED_BYTE)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glGenTextures(1, &colorTexture);
		glBindTexture(GL_TEXTURE_2D, colorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void DepthAttach()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glGenRenderbuffers(1, &renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	const unsigned int GetTexture() const
	{
		return colorTexture;
	}
	void Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER,fbo);
	}

	void Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	bool Check()
	{
		if (glCheckFramebufferStatus(fbo) == GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "The framebuffer is complete\n";
			return true;
		}
		std::cout << "The framebuffer is complete\n";
		return false;
	}
private:
	int width;
	int height;
	unsigned int fbo;
	unsigned int depthTexture;
	unsigned int renderbuffer;
	unsigned int colorTexture;

};


