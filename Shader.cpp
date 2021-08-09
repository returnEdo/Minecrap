#include "Shader.hpp"

#include "FileIO.hpp"
#include "Macros.hpp"


namespace Minecrap
{


namespace Graphics
{

void checkErrors(unsigned int tHandle, unsigned int tCheckType)
{
	int lCompiled;
	char lLog[512];

	glGetShaderiv(tHandle, tCheckType, &lCompiled);
	if (!lCompiled)
	{
		glGetShaderInfoLog(tHandle, 512, nullptr, lLog);
		ASSERT(false, std::string(lLog));
	}
};

// Returns handle to compiled shader
unsigned int compileShader(std::string& tSource, unsigned int tType)
{
	unsigned int lHandle = glCreateShader(tType);	

	const char* lSource = tSource.c_str();
	
	glShaderSource(lHandle, 1, &lSource, nullptr);
	glCompileShader(lHandle);
	
	Graphics::checkErrors(lHandle, GL_COMPILE_STATUS);

	return lHandle;
};

// Create complete program
void Shader::init(const std::string& tVertexAddress, const std::string& tFragmentAddress)
{

	std::string lVertexSource 	= IO::Misc::readRaw(tVertexAddress);
	std::string lFragmentSource	= IO::Misc::readRaw(tFragmentAddress);

	unsigned int lVertexHandle 	= Graphics::compileShader(lVertexSource,	GL_VERTEX_SHADER);
	unsigned int lFragmentHandle 	= Graphics::compileShader(lFragmentSource,	GL_FRAGMENT_SHADER);
	
	mId = glCreateProgram();

	glAttachShader(mId, lVertexHandle);
	glAttachShader(mId, lFragmentHandle);
	glLinkProgram(mId);
	
	Graphics::checkErrors(mId, GL_LINK_STATUS);
	
	glDeleteShader(lVertexHandle);
	glDeleteShader(lFragmentHandle);
};

void Shader::findUniformLocations(const std::vector<std::string>& tUniforms)
{
	for (auto lString: tUniforms)
	{
		mUniformNameToId[lString] = glGetUniformLocation(mId, lString.c_str());
	}
};

void Shader::setUniform(const std::string& tUniform, float tValue)
{
	glUniform1f(mUniformNameToId[tUniform], tValue);
};

void Shader::setUniform(const std::string& tUniform, Math::vec2& tValue)
{
	glUniform2fv(mUniformNameToId[tUniform], 1, (float*)&tValue);
};

void Shader::setUniform(const std::string& tUniform, Math::vec3& tValue)
{
	glUniform3fv(mUniformNameToId[tUniform], 1, (float*)&tValue);
};

void Shader::setUniform(const std::string& tUniform, Math::mat3& tValue)
{
	glUniformMatrix3fv(mUniformNameToId[tUniform], 1, GL_FALSE, (float*)&tValue);	
};

void Shader::destroy(void)
{
	glDeleteProgram(mId);
};

void Shader::bind(void) const
{	
	glUseProgram(mId);
};


}

}
