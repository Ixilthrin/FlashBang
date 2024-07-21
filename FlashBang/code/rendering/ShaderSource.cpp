#include "ShaderSource.h"

#include <sstream>
#include <fstream>

using std::ifstream;
using std::stringstream;

ShaderSource::ShaderSource()
{
}


ShaderSource::~ShaderSource()
{
}


string ShaderSource::ReadShaderFromFile(string path)
{
    ifstream stream(path);
    stringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
}
