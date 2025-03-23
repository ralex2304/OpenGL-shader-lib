#pragma once

#include <filesystem>
#include <glad/gl.h>
#include <stdexcept>
#include <string>

class Shader {
    public:

        Shader(const std::filesystem::path vertex_path, const std::filesystem::path fragment_path);

        void use() const;

        void set_bool(const std::string& name, const bool value) const;
        void set_int(const std::string& name, const int value) const;
        void set_float(const std::string& name, const float value) const;

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
        unsigned int program_id;

        std::string read_shader_from_file(const std::filesystem::path path) const;
        unsigned int compile_shader(const std::string& code, const int shader_type) const;
};

