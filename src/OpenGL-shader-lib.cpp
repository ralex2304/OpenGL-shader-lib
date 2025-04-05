#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <glad/gl.h>

#include <OpenGL-shader-lib/OpenGL-shader-lib.hpp>

using namespace OpenGLShaderLib;
using namespace std::literals;

Shader::Shader(const std::filesystem::path& vertex_path, const std::filesystem::path& fragment_path) {
    const std::string vertex_code   = read_shader_from_file(vertex_path);
    const std::string fragment_code = read_shader_from_file(fragment_path);

    const char* c_vertex_code   = vertex_code.c_str();
    const char* c_fragment_code = fragment_code.c_str();

    const unsigned int vertex   = compile_shader(vertex_code, GL_VERTEX_SHADER);
    const unsigned int fragment = compile_shader(fragment_code, GL_FRAGMENT_SHADER);

    program_id = glCreateProgram();
    glAttachShader(program_id, vertex);
    glAttachShader(program_id, fragment);
    glLinkProgram(program_id);

    int success = 0;
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[GL_INFO_LOG_LEN] = {};
        glGetProgramInfoLog(program_id, GL_INFO_LOG_LEN, NULL, info_log);
        throw link_failure("ERROR::SHADER::PROGRAM::LINKING_FAILED: "s + info_log);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() const noexcept {
    glUseProgram(program_id);
}

void Shader::set_bool(const std::string& name, bool value) const noexcept {
    glUniform1i(glGetUniformLocation(program_id, name.c_str()), static_cast<int>(value));
}

void Shader::set_int(const std::string& name, const int value) const noexcept {
    glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
}

void Shader::set_float(const std::string& name, const float value) const noexcept {
    glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
}

std::string Shader::read_shader_from_file(const std::filesystem::path& path) const {
    std::ifstream shader_file;

    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        shader_file.open(path);

        const auto size = std::filesystem::file_size(path);

        std::string code(size, '\0');
        shader_file.read(code.data(), size);

        shader_file.close();

        return code;
    } catch (const std::ifstream::failure &e) {
        throw file_failure("ERROR::SHADER::FILE_READ_ERROR: "s + e.what());
    }
}

unsigned int Shader::compile_shader(const std::string& code, const int shader_type) const {
    unsigned int shader = 0;

    const char* c_code = code.c_str();
    shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &c_code, NULL);
    glCompileShader(shader);

    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[GL_INFO_LOG_LEN] = {};
        glGetShaderInfoLog(shader, GL_INFO_LOG_LEN, NULL, info_log);
        throw compile_failure("ERROR::SHADER::VERTEX::COMPILATION_FAILED"s + info_log);
    }

    return shader;
}

