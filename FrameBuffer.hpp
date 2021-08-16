#pragma once


#include <GL/glew.h>

namespace Minecrap
{

namespace Graphics
{

enum class Attachment: int
{
	COLOR = GL_RGB,
	DEPTH = GL_DEPTH_COMPONENT
};

class FrameBuffer
{
	private:

	unsigned int m_id;

	unsigned int m_colorid	{ 0 };
	unsigned int m_depthid	{ 0 };
	
	template <Attachment t_attachment>
	unsigned int generateTexture(void) const;

	public:
	
	~FrameBuffer(void);
	
	void init(void);
	
	void bind(bool t_bind = true);

	inline unsigned int getColorTextureId(void)	{ return m_colorid; }
	inline unsigned int getDepthTextureId(void)	{ return m_depthid; }
};	

}

}
