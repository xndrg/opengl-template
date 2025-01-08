#include <iostream>
#include <stdio.h>

#include <typedefs.h>
#include <shader.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800
#define FPS           144

// static float vertex_buffer_data[] = {
//      // Verticies              // Texture coordinates
//     -0.3f, -0.3f, 0.0f,        0.0f, 0.0f,
//     -0.3f,  0.3f, 0.0f,        0.0f, 1.0f,
//      0.3f,  0.3f, 0.0f,        1.0f, 1.0f,
//      0.3f, -0.3f, 0.0f,        1.0f, 0.0f,
// };

static float vertex_buffer_data[] = {
    -0.3f, -0.3f,  0.0f,       0.0f, 0.0f, // 0
    -0.3f,  0.3f,  0.0f,       0.0f, 1.0f, // 1
     0.3f,  0.3f,  0.0f,       1.0f, 1.0f, // 2
     0.3f, -0.3f,  0.0f,       1.0f, 0.0f, // 3
    -0.3f,  0.3f, -0.6f,       1.0f, 1.0f, // 4
     0.3f,  0.3f, -0.6f,       1.0f, 1.0f, // 5
    -0.3f, -0.3f, -0.6f,       0.0f, 0.0f, // 6
     0.3f, -0.3f, -0.6f,       1.0f, 0.0f, // 7
};

// static uint index_buffer_data[] = {
//     0, 1, 2,
//     2, 3, 0,
// };

static uint index_buffer_data[] = {
    0, 1, 2,
    2, 3, 0,
    1, 4, 5,
    5, 2, 1,
    0, 3, 6,
    3, 6, 7,
    6, 4, 5,
    5, 7, 6,
    0, 1, 4,
    4, 6, 0,
    3, 2, 5,
    5, 7, 3,
};

void framebuffer_resize_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "ERROR: Could not initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Math", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "ERROR: Could not create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
    
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        std::cerr << "ERROR: Could not initialize OpenGL context\n";
        glfwTerminate();
        return -1;
    }
    fprintf(stdout, "INFO: Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    uint shader_program = load_shaders("../assets/shaders/vertex.glsl", "../assets/shaders/fragment.glsl");
    glUseProgram(shader_program);

    int width, height, ch_num;
    stbi_set_flip_vertically_on_load(true);  
    u8 *data = stbi_load("../assets/textures/container.jpg", &width, &height, &ch_num, 0);
    if (data == NULL)
    {
        std::cerr << "ERROR: Could not load the texture\n";
        glfwTerminate();
        return -1;
    }
    fprintf(stdout, "INFO: Texture loaded successfully: [width: %d, height: %d, ch_num: %d]\n", width, height, ch_num);
    uint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    uint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
 
    uint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

    uint index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer_data), index_buffer_data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glm::mat4 trans = glm::mat4(1.0f);
    uint trans_uniform = glGetUniformLocation(shader_program, "trans");
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, sizeof(index_buffer_data) / sizeof(uint), GL_UNSIGNED_INT, 0);

        trans = glm::rotate(trans, glm::radians(0.01f), glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(trans_uniform, 1, GL_FALSE, glm::value_ptr(trans));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
