#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include <glad/gl.h>

#include <typedefs.h>
#include <shader.h>

uint load_shaders(const char *vertex_path, const char *fragment_path)
{
    uint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    uint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertex_src;
    std::ifstream vertex_stream(vertex_path, std::ios::in);
    if (!vertex_stream.is_open())
    {
        fprintf(stderr, "ERROR: Could not open `%s`\n", vertex_path);
        return 0;
    }
    std::stringstream vstr_stream;
    vstr_stream << vertex_stream.rdbuf();
    vertex_src = vstr_stream.str();
    
    std::string fragment_src;
    std::ifstream fragment_stream(fragment_path, std::ios::in);
    if (!fragment_stream.is_open())
    {
        fprintf(stderr, "ERROR: Could not open `%s`\n", vertex_path);
        return 0;        
    }
    std::stringstream fstr_stream;
    fstr_stream << fragment_stream.rdbuf();
    fragment_src = fstr_stream.str();

    char const *vptr = vertex_src.c_str();
    glShaderSource(vertex_shader, 1, &vptr, NULL);
    glCompileShader(vertex_shader);
    int vertex_compile_status = GL_FALSE;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_compile_status);
    if (vertex_compile_status == GL_FALSE)
    {
        int length;
        glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &length);
        char *log_msg = (char*)malloc(sizeof(char)*length + sizeof(char));
        memset(log_msg, 0, sizeof(char)*length + sizeof(char));
        glGetShaderInfoLog(vertex_shader, length, NULL, log_msg);
        fprintf(stderr, "ERROR: Could not compile the vertex shader:\n%s\n", log_msg);
        free(log_msg);
    }
    fprintf(stdout, "INFO: Vertex shader compiled successfully\n");

    char const *fptr = fragment_src.c_str();
    glShaderSource(fragment_shader, 1, &fptr, NULL);
    glCompileShader(fragment_shader);
    int fragment_compile_status = GL_FALSE;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_compile_status);
    if (fragment_compile_status == GL_FALSE)
    {
        int length;
        glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &length);
        char *log_msg = (char*)malloc(sizeof(char)*length + sizeof(char));
        memset(log_msg, 0, sizeof(char)*length + sizeof(char));
        glGetShaderInfoLog(fragment_shader, length, NULL, log_msg);
        fprintf(stderr, "ERROR: Could not compile the fragment shader:\n%s\n", log_msg);
        free(log_msg);
    }
    fprintf(stdout, "INFO: Fragment shader compiled successfully\n");

    uint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    int program_link_status = GL_FALSE;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &program_link_status);
    if (program_link_status == GL_FALSE)
    {
        int length;
        glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &length);
        char *log_msg = (char*)malloc(sizeof(char)*length + sizeof(char));
        memset(log_msg, 0, sizeof(char)*length + sizeof(char));
        glGetProgramInfoLog(shader_program, length, NULL, log_msg);
        fprintf(stderr, "ERROR: Could not link the shader program:\n%s\n", log_msg);
        free(log_msg);
    }
    fprintf(stdout, "INFO: Shader program linked successfully\n");

    glDetachShader(shader_program, vertex_shader);
    glDetachShader(shader_program, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}
