#pragma once


#include <GL/glew.h>

namespace Minecrap
{

namespace Graphics
{

class FrameBuffer
{
	private:

	unsigned int m_id;

	unsigned int m_colorid;
	unsigned int m_depthid;
	
	void generateTexture(unsigned int& t_id, int t_type);

	public:
	
	
	void init(void);
	void destroy(void);
	
	void bind(bool t_bind = true);
	void prepareForRendering(void);

	inline unsigned int getColorTextureId(void)	{ return m_colorid; }
	inline unsigned int getDepthTextureId(void)	{ return m_depthid; }
};	

}

}
