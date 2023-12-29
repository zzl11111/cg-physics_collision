
#ifndef _SHADER_H_
#define _SHADER_H_
#include "utility.h"
#include <fstream>
#include <sstream>


class Shader {
public:
    Shader() = default;
    /**
     * init shader with shader files
     */
    Shader(const std::string& vsPath, const std::string& fsPath);
    Shader(const std::string& vsPath, const std::string& gsPath, const std::string& fsPath);
    void init(const std::string& vsPath, const std::string& fsPath);
    void init(const std::string& vsPath, const std::string& gsPath, const std::string& fsPath);

    /**
     * init shader with shader codes
     */
    void initWithCode(const std::string& vs, const std::string& fs);
    /**
     * get shader code from a file
     */
    void initWithCode(const std::string& vs, const std::string& gs, const std::string& fs);

    static std::string getCodeFromFile(const std::string& path);
    /**
     * use shader
     */

    void use() const;
    /**
     * get a uniform variable's location according to its name
     */
    GLint getUniformLocation(const std::string& name) const;
    /**
     * set value of uniform variables
     */
    void setInt(const std::string& name, GLint value) const;
    void setFloat(const std::string& name, GLfloat value) const;
    void setMat3(const std::string& name, const glm::mat3& value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
private:
    GLuint id = 0;
};
#endif
Shader::Shader(const std::string& vsPath, const std::string& fsPath)
{
    init(vsPath, fsPath);
}
Shader::Shader(const std::string& vsPath, const std::string& gsPath, const std::string& fsPath) {
    init(vsPath, gsPath, fsPath);
}

void Shader::init(const std::string& vsPath, const std::string& fsPath)
{
    initWithCode(getCodeFromFile(vsPath), getCodeFromFile(fsPath));
}
void Shader::init(const std::string& vsPath, const std::string& gsPath, const std::string& fsPath) {
    initWithCode(getCodeFromFile(vsPath), getCodeFromFile(gsPath), getCodeFromFile(fsPath));

}
void Shader::initWithCode(const std::string& vs, const std::string& fs)
{
    // TODO: filling this function to set shaders to OpenGL
    unsigned int vertexShader, fragmentShader;
    int success;
    char Log[512];
    // the log operation
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // create the vs_shaderr
    const char* vs_source = vs.c_str();
    glShaderSource(vertexShader, 1, &vs_source, NULL);
    // bind the source to shader
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    // check the compile success
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, Log);
        std::cerr << "ERROR::COMPILE::VERTEX_SHADER:" << Log << std::endl;
        exit(-1);
    }
    // compile the fragment_shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* fs_source = fs.c_str();
    glShaderSource(fragmentShader, 1, &fs_source, NULL);
    // bind the code to fragmentShader
    glCompileShader(fragmentShader);
    // compile the fs
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, Log);
        std::cerr << "ERROR::COMPILE::FRAGMENT_SHADER: " << Log << std::endl;
        exit(-1);
    }
    // create the program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // attach all the shader and finally link the shader
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, Log);
        std::cerr << "ERROR::LINK::PROGRAM:" << Log << std::endl;
        exit(-1);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    id = shaderProgram;
}
void Shader::initWithCode(const std::string& vs, const std::string& gs, const std::string& fs)
{
    // TODO: filling this function to set shaders to OpenGL
    unsigned int vertexShader, fragmentShader, geometryShader;
    int success;
    char Log[512];
    // the log operation
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // create the vs_shader
    const char* vs_source = vs.c_str();
    glShaderSource(vertexShader, 1, &vs_source, NULL);
    // bind the source to shader
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    // check the compile success
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, Log);
        std::cerr << "ERROR::COMPILE::VERTEX_SHADER:" << Log << std::endl;
        exit(-1);
    }
    // compile the fragment_shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* fs_source = fs.c_str();
    glShaderSource(fragmentShader, 1, &fs_source, NULL);
    // bind the code to fragmentShader
    glCompileShader(fragmentShader);
    // compile the fs
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, Log);
        std::cerr << "ERROR::COMPILE::FRAGMENT_SHADER: " << Log << std::endl;
        exit(-1);
    }

    geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    const char* gs_source = gs.c_str();
    glShaderSource(geometryShader, 1, &gs_source, NULL);
    glCompileShader(geometryShader);
    glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(geometryShader, 512, NULL, Log);
        std::cerr << "ERROR::COMPILE::GEOMETRY_SHADER: " << Log << std::endl;
        exit(-1);
    }
    // create the program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glAttachShader(shaderProgram, geometryShader);
    // attach all the shader and finally link the shader
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, Log);
        std::cerr << "ERROR::LINK::PROGRAM:" << Log << std::endl;
        exit(-1);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    id = shaderProgram;
}
std::string Shader::getCodeFromFile(const std::string& path)
{
    std::string code;
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        file.open(path);
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        code = stream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr<<("File Error: ") + std::string(e.what());
    }
    return code;
}

void Shader::use() const { glUseProgram(id); }

GLint Shader::getUniformLocation(const std::string& name) const
{
    return glGetUniformLocation(id, name.c_str());
}

void Shader::setInt(const std::string& name, GLint value) const
{
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string& name, GLfloat value) const
{
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setMat3(const std::string& name, const glm::mat3& value) const
{
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE,
        glm::value_ptr(value));
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE,
        glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
}
