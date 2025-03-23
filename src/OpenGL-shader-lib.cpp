#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include <OpenGL-shader-lib/OpenGL-shader-lib.hpp>

std::string Shader::read_shader_from_file(const std::filesystem::path path) const {
    std::string code;
    std::ifstream shader_file;

    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        shader_file.open(path);

        std::stringstream shader_stream;

        shader_stream << shader_file.rdbuf();

        shader_file.close();

        code = shader_stream.str();
    } catch (const std::ifstream::failure &e) {
        throw file_failure(std::string("ERROR::SHADER::FILE_READ_ERROR: ") + e.what());
    }

    return code;
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
        char info_log[512] = {};
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        throw compile_failure(std::string("ERROR::SHADER::VERTEX::COMPILATION_FAILED") + info_log);
    }

    return shader;
}

Shader::Shader(const std::filesystem::path vertex_path, const std::filesystem::path fragment_path) {
    std::string vertex_code   = read_shader_from_file(vertex_path);
    std::string fragment_code = read_shader_from_file(fragment_path);

    const char* c_vertex_code   = vertex_code.c_str();
    const char* c_fragment_code = fragment_code.c_str();

    unsigned int vertex   = compile_shader(vertex_code, GL_VERTEX_SHADER);
    unsigned int fragment = compile_shader(fragment_code, GL_FRAGMENT_SHADER);

    program_id = glCreateProgram();
    glAttachShader(program_id, vertex);
    glAttachShader(program_id, fragment);
    glLinkProgram(program_id);

    int success = 0;
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512] = {};
        glGetProgramInfoLog(program_id, 512, NULL, info_log);
        throw link_failure(std::string("ERROR::SHADER::PROGRAM::LINKING_FAILED: ") + info_log);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() const {
    glUseProgram(program_id);
}

void Shader::set_bool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(program_id, name.c_str()), (int)value);
}

void Shader::set_int(const std::string& name, const int value) const {
    glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
}

void Shader::set_float(const std::string& name, const float value) const {
    glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
}

