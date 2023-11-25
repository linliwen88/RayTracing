// -------------------------------------------------------------
// Sourced from https://learnopengl.com/Getting-started/Shaders
// -------------------------------------------------------------
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include <vector>

#include "sphere.h"

class shader
{
public:
    unsigned int ID;
    // constructor generates the shader on the fly
    shader(const char* vertexPath, const char* fragmentPath);

    // activate the shader
    inline void use() const
    {
        glUseProgram(ID);
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    inline void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    inline void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    inline void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    inline void setVec3(const std::string& name, glm::vec3& vec) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
    }

    inline void setMat4(const std::string& name, glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setWorld(const std::string& name, std::vector<std::shared_ptr<hittable>>& world) const;



private:
    // utility function for checking shader compilation/linking errors.
    void checkCompileErrors(unsigned int shader, std::string type);
};