#include "FileIO.hpp"

#include <fstream>

#include "Macros.hpp"


namespace Minecrap
{


namespace IO
{



namespace Misc
{

std::string readRaw(const std::string& tAddress)
{
        std::ifstream lFile(tAddress);

        ASSERT(lFile.is_open(), "Could not open file!!");

        std::string lOutput = "";
        std::string lLine;

        while (getline(lFile, lLine))
        {
                lOutput += lLine;
                lOutput += "\n";
        }

        lFile.close();
        
        return lOutput;
};

}

}

}
