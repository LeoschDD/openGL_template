#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

static const char* GlErrorToString(GLenum e) {
    switch (e) {
        case GL_NO_ERROR: return "GL_NO_ERROR";
        case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
        default: return "Unknown GL error";
    }
}

void PrintOpenGLErrors(char const* const Function, char const* const File, int const Line)
{
    for (GLenum err; (err = glGetError()) != GL_NO_ERROR; ) {
        std::cerr << "OpenGL Error in " << File << " at line " << Line
                  << " calling " << Function << ": " << GlErrorToString(err)
                  << " (0x" << std::hex << err << std::dec << ")\n";
    }
}

#ifdef _DEBUG
#define CheckedGLCall(x) do { PrintOpenGLErrors(">>BEFORE<< " #x, __FILE__, __LINE__); (x); PrintOpenGLErrors(#x, __FILE__, __LINE__); } while (0)
#define CheckedGLResult(x) (x); PrintOpenGLErrors(#x, __FILE__, __LINE__)
#else
#define CheckedGLCall(x) (x)
#define CheckedGLResult(x) (x)
#endif

void PrintShaderInfoLog(GLint Shader)
{
    GLint len = 0;
    glGetShaderiv(Shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 1) {
        std::string log(size_t(len), '\0');
        GLsizei written = 0;
        glGetShaderInfoLog(Shader, len, &written, log.data());
        std::cout << "Shader Info Log:\n" << log << "\n";
    }
}

int main()
{
    if (!glfwInit()) return -1;

    // Optional, aber empfehlenswert für moderne Shader (#version 150~330):
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);

    // Wichtig mit Core Profile
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "GLEW init error: " << glewGetErrorString(err) << "\n";
        glfwTerminate();
        return -1;
    }

    const char* VertexShaderSource = R"GLSL(
        #version 150
        in vec2 position;
        void main() { gl_Position = vec4(position, 0.0, 1.0); }
    )GLSL";

    const char* FragmentShaderSource = R"GLSL(
        #version 150
        out vec4 outColor;
        void main() { outColor = vec4(1.0, 1.0, 1.0, 1.0); }
    )GLSL";

    GLfloat const Vertices[] = { 0.0f, 0.5f,  0.5f, -0.5f,  -0.5f, -0.5f };
    GLuint  const Elements[] = { 0, 1, 2 };

    GLuint VAO; CheckedGLCall(glGenVertexArrays(1, &VAO));
    CheckedGLCall(glBindVertexArray(VAO));

    GLuint VBO; CheckedGLCall(glGenBuffers(1, &VBO));
    CheckedGLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    CheckedGLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW));
    CheckedGLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

    GLuint EBO; CheckedGLCall(glGenBuffers(1, &EBO));
    CheckedGLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    CheckedGLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Elements), Elements, GL_STATIC_DRAW));

    GLint compiled = 0;
    GLuint vs = CheckedGLResult(glCreateShader(GL_VERTEX_SHADER));
    CheckedGLCall(glShaderSource(vs, 1, &VertexShaderSource, nullptr));
    CheckedGLCall(glCompileShader(vs));
    CheckedGLCall(glGetShaderiv(vs, GL_COMPILE_STATUS, &compiled));
    if (!compiled) { std::cerr << "Vertex shader failed\n"; PrintShaderInfoLog(vs); }

    GLuint fs = CheckedGLResult(glCreateShader(GL_FRAGMENT_SHADER));
    CheckedGLCall(glShaderSource(fs, 1, &FragmentShaderSource, nullptr));
    CheckedGLCall(glCompileShader(fs));
    CheckedGLCall(glGetShaderiv(fs, GL_COMPILE_STATUS, &compiled));
    if (!compiled) { std::cerr << "Fragment shader failed\n"; PrintShaderInfoLog(fs); }

    GLuint prog = CheckedGLResult(glCreateProgram());
    CheckedGLCall(glAttachShader(prog, vs));
    CheckedGLCall(glAttachShader(prog, fs));
    CheckedGLCall(glBindFragDataLocation(prog, 0, "outColor"));
    CheckedGLCall(glLinkProgram(prog));
    CheckedGLCall(glUseProgram(prog));

    GLint posLoc = CheckedGLResult(glGetAttribLocation(prog, "position"));
    CheckedGLCall(glEnableVertexAttribArray(posLoc));
    CheckedGLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    CheckedGLCall(glVertexAttribPointer(posLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0));
    CheckedGLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

    while (!glfwWindowShouldClose(window)) {
        CheckedGLCall(glClear(GL_COLOR_BUFFER_BIT));
        CheckedGLCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    CheckedGLCall(glDeleteProgram(prog));
    CheckedGLCall(glDeleteShader(fs));
    CheckedGLCall(glDeleteShader(vs));
    CheckedGLCall(glDeleteBuffers(1, &EBO));
    CheckedGLCall(glDeleteBuffers(1, &VBO));
    CheckedGLCall(glDeleteVertexArrays(1, &VAO));

    glfwTerminate();
    return 0;
}
