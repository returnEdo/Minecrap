#include "FrameBuffer.hpp"

#include "Macros.hpp"
#include "WindowManager.hpp"

extern Minecrap::WindowManager g_windowManager;


namespace Minecrap
{

namespace Graphics
{

template <Attachment t_attachment>
unsigned int FrameBuffer::generateTexture(void) const
{
	static unsigned int s_counter = 0;

	unsigned int l_id;

	glGenTextures(1, &l_id);
	glActiveTexture(GL_TEXTURE0 + s_counter);
	glBindTexture(GL_TEXTURE_2D, l_id);

	glTexImage2D(GL_TEXTURE_2D, 0, (int)t_attachment, g_windowManager.mWidth, g_windowManager.mHeight, 0, (int)t_attachment, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	s_counter++;

	return l_id;
}

FrameBuffer::~FrameBuffer(void)
{
	glDeleteFramebuffers(1, &m_id);
}

void FrameBuffer::init(void)
{
	glGenFramebuffers(1, &m_id);
	bind();

	unsigned int m_colorid = generateTexture<Attachment::COLOR>();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorid, 0);
	unsigned int m_depthid = generateTexture<Attachment::DEPTH>();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthid, 0);
	
	// check attachments are ok
	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Frame buffer not complete. Check your attachments!!");

	bind(0);
}

void FrameBuffer::bind(bool t_bind)
{
	glBindFramebuffer(GL_FRAMEBUFFER, (t_bind? m_id: 0));

	if (t_bind)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_colorid);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_depthid);
	}
}

}

}
