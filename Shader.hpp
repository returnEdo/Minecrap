#pragma once

#include <GL/glew.h>
#include <unordered_map>
#include <string>

#include "vec2.hpp"
#include "vec3.hpp"
#include "mat3.hpp"

namespace Minecrap
{


namespace Graphics
{


void checkErrors(unsigned int tHandle, unsigned int tCheckType);
unsigned int compileShader(std::string& tSource, unsigned int tType);

class Shader
{
	private:

	unsigned int 				mId;
	std::unordered_map<std::string, int>	mUniformNameToId;
	
	public:

	void init(const std::string& tVertexAddress, const std::string& tFragmentAddress);
	void destroy(void);
	
	void bind(void) const;

	void findUniformLocations(const std::vector<std::string>& tUniforms);
	void setUniform(const std::string& tUniform, float tValue);
	void setUniform(const std::string& tUniform, Math::vec2& tValue);
	void setUniform(const std::string& tUniform, Math::vec3& tValue);
	void setUniform(const std::string& tUniform, Math::mat3& tValue);
};


}

}
