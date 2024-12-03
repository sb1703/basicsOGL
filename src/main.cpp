#include "config.h"
#include "triangle_mesh_2.h"
#include "material.h"
#include "linear_algebra.h"

unsigned int make_program(const std::string& vertex_filepath, const std::string& fragment_filepath);
unsigned int make_module(const std::string& filepath, unsigned int module_type);

int main(){

    // std::ifstream file;
    // std::string line;

    // file.open("../src/shaders/vertex.txt");
    // while(std::getline(file, line)) {
    //     std::cout << line << std::endl;
    // }

    GLFWwindow* window;

    if(!glfwInit()){
        std::cout << "GLFW couldn't be start" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(640, 480, "Basics", NULL, NULL);
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Couldn't load opengl" << std::endl;
        glfwTerminate();
        return -1;
    }

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);

    TriangleMesh2* triangle = new TriangleMesh2();
    Material* material = new Material("../img/mr_robot.jpg");
    Material* mask = new Material("../img/vignette.jpg");

    unsigned int shaderProgram = make_program("../src/shaders/vertex.txt", "../src/shaders/fragment.txt");

    // set texture units
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "material"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "mask"), 1);

    glm::vec3 quad_position = {-0.2f, 0.4f, 0.0f};
    glm::vec3 camera_pos = {-0.4f, 0.0f, 0.2f};
    glm::vec3 camera_target = {0.0f, 0.0f, 0.0f};
    glm::vec3 up = {0.0f, 0.0f, 1.0f};
    unsigned int model_location = glGetUniformLocation(shaderProgram, "model"); 
    unsigned int view_location = glGetUniformLocation(shaderProgram, "view"); 
    unsigned int proj_location = glGetUniformLocation(shaderProgram, "projection");

    glm::mat4 view = glm::lookAt(camera_pos, camera_target, up);
    glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 projection = glm::perspective(45.0f, 640.0f / 480.0f, 0.1f, 10.0f);
    glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(projection));

    // enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, quad_position);
        model = glm::rotate(model, (float)glfwGetTime(), {0.0f, 0.0f, 1.0f});
        glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
        
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        material->use(0);
        mask->use(1);
        triangle->draw2();
        glfwSwapBuffers(window);
    }

    glDeleteProgram(shaderProgram);
    delete triangle;
    delete material;
    delete mask;
    glfwTerminate();
    return 0;
}

unsigned int make_program(const std::string& vertex_filepath, const std::string& fragment_filepath) {

    std::vector<unsigned int> modules;
    modules.push_back(make_module(vertex_filepath, GL_VERTEX_SHADER));
    modules.push_back(make_module(fragment_filepath, GL_FRAGMENT_SHADER));

    unsigned int shaderProgram = glCreateProgram();
    for(unsigned int shaderModule : modules) {
        glAttachShader(shaderProgram, shaderModule);
    }
    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        char errorLog[1024];
        glGetProgramInfoLog(shaderProgram, 1024, NULL, errorLog);
        std::cout << "Shader Program Linking Error:\n" << errorLog << std::endl;
    }

    for(unsigned int shaderModule : modules) {
        glDeleteShader(shaderModule);
    }

    return shaderProgram;
}

unsigned int make_module(const std::string& filepath, unsigned int module_type) {

    std::ifstream file;
    std::stringstream bufferedLines;
    std::string line;

    file.open(filepath);
    while(std::getline(file, line)) {
        bufferedLines << line << "\n";
    }
    std::string shaderSource = bufferedLines.str();
    const char* shaderSrc = shaderSource.c_str();
    bufferedLines.str("");
    file.close();

    unsigned int shaderModule = glCreateShader(module_type);
    glShaderSource(shaderModule, 1, &shaderSrc, NULL);
    glCompileShader(shaderModule);

    int success;
    glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
    if(!success) {
        char errorLog[1024];
        glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
        std::cout << "Shader Module Compilation Error:\n" << errorLog << std::endl;
    }

    return shaderModule;
}