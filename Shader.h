//
// Created by ACER on 4.2.2026 г..
//

#ifndef RAWENGINE_SHADER_H
#define RAWENGINE_SHADER_H
#include <string>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


class Shader
{
public:

    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    void Use() const;

    void setFloat(const std::string& name, float value) const;
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;

private:
    static void CheckCompileErrors(unsigned int shader, const std::string& type);
};


#endif //RAWENGINE_SHADER_H