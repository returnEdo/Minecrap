#include "FrameBuffer.hpp"

#include "Macros.hpp"
#include "WindowManager.hpp"

extern Minecrap::WindowManager g_windowManager;


namespace Minecrap
{

namespace Graphics
{

void FrameBuffer::generateTexture(unsigned int& t_id, int t_type)
{
	glGenTextures(1, &t_id);
	glBindTexture(GL_TEXTURE_2D, t_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, t_type, g_windowManager.mWidth, g_windowManager.mHeight, 0, t_type, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBuffer::destroy(void)
{
	glDeleteFramebuffers(1, &m_id);
}

void FrameBuffer::init(void)
{
	generateTexture(m_colorid, GL_RGBA);
	generateTexture(m_depthid, GL_DEPTH_COMPONENT);
	
	glGenFramebuffers(1, &m_id);
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorid, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthid, 0);

	ASSERT((GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus(GL_FRAMEBUFFER)),
		"FrameBuffer is not complete!! Check your attachments!!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bind(bool t_bind)
{
	glBindFramebuffer(GL_FRAMEBUFFER, (t_bind? m_id: 0));
}

void FrameBuffer::prepareForRendering(void) 
{
	bind();
	
	// Activate the textures

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_colorid);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_depthid);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

}

}
