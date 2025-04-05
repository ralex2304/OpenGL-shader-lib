#pragma once

#include <filesystem>
#include <glad/gl.h>
#include <stdexcept>
#include <string>

namespace OpenGLShaderLib {
    class Shader {
    public:

        Shader(const std::filesystem::path& vertex_path, const std::filesystem::path& fragment_path);

        void use() const noexcept;

        void set_bool(const std::string& name, bool value) const noexcept;
        void set_int(const std::string& name, int value) const noexcept;
        void set_float(const std::string& name, float value) const noexcept;

        class file_failure: public std::runtime_error {
            using std::runtime_error::runtime_error;
        };
        class compile_failure: public std::runtime_error {
            using std::runtime_error::runtime_error;
        };
        class link_failure: public std::runtime_error {
            using std::runtime_error::runtime_error;
        };

    private:
        std::string read_shader_from_file(const std::filesystem::path& path) const;
        unsigned int compile_shader(const std::string& code, int shader_type) const;

    private:
        static constexpr size_t GL_INFO_LOG_LEN = 512;

        unsigned int program_id = {};
    };
};
