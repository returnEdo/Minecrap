#pragma once

#include <string>

namespace Minecrap
{


namespace Graphics
{


class Texture
{
	private:

	unsigned int	mId;

	public:

	void init(const std::string& tAddress);

	void bind(void);
};


}

}
