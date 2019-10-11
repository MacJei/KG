#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

#include "Shader.h"
#include "multyplyes.h"

#define NUMBER_OF_LINES 4
#define PIRAMID_HEIGHT 0.7f

bool keys[1024];

bool ret = false;
GLfloat x_rotation = 0.0f, y_rotation = 0.0f, z_rotation = 0.0f;
GLfloat scale = 1.0f;

GLFWwindow* InitOpenGl(GLuint width, GLuint height);
void DestroyOpenGl(GLuint VAO, GLuint VBO, GLuint EBO);
void InitBuffers(GLuint& VAO, GLuint& VBO, GLuint& EBO, GLuint num_of_angles);
void GameLoop(GLFWwindow* window, Shader& shader, GLuint VAO, GLuint VBO, GLuint EBO);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
void new_func_size_callback(GLFWwindow* window, int width, int height);


GLfloat* vertex_vector(GLuint num_of_angles);
// void vertex_indexses(GLuint* indexis);




GLFWwindow* InitOpenGl(GLuint width, GLuint height){
    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Bronnikov_12", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, new_func_size_callback);

    glewExperimental = GL_TRUE;

    glewInit();

    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    return window;
}

void DestroyOpenGl(GLuint VAO, GLuint VBO, GLuint EBO){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    if(EBO){
        glDeleteBuffers(1, &EBO);
    }
    glfwTerminate();
}

void print_vec(GLfloat* vertex){
    int j = 0;
    for(int i = 0; i < 36*NUMBER_OF_LINES; ++i){
        std::cout << vertex[i] << '\t';
        if(++j > 5){
            j = 0;
            std::cout << std::endl;
        }
    }
}

void InitBuffers(GLuint& VAO, GLuint& VBO, GLuint& EBO, GLuint num_of_angles){
    GLfloat *vertices = vertex_vector(num_of_angles);
    // print_vec(vertices);
    // GLuint indices[2*3*NUMBER_OF_LINES];

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 36 * num_of_angles, vertices, GL_DYNAMIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    EBO = 0;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Unbind VAO
    free(vertices);
}

// If EBO enable:

/*
void vertex_vector(GLfloat* vertex_ans){
    int j = 0;
    for(float i = 0.0f; i < 2 * M_PI; i+= 2 * M_PI / NUMBER_OF_LINES){
        vertex_ans[j++] = -0.5*cos(i);;
        vertex_ans[j++] = 0.0f;
        vertex_ans[j++] = 0.5*sin(i);
    }
    vertex_ans[j++] = 0.0f;
    vertex_ans[j++] = 0.0f;
    vertex_ans[j++] = 0.0f;
    vertex_ans[j++] = 0.0f;
    vertex_ans[j++] = PIRAMID_HEIGHT;
    vertex_ans[j++] = 0.0f;
}

void vertex_indexses(GLuint* indexis){
    int j = 0;
    for(int i = 0; i < NUMBER_OF_LINES; ++i){
        indexis[j++] = i;
        indexis[j++] = NUMBER_OF_LINES;
        indexis[j++] = i + 1;
    }
    indexis[j-1] = 0;
    for(int i = 0; i < NUMBER_OF_LINES; ++i){
        indexis[j++] = i;
        indexis[j++] = NUMBER_OF_LINES + 1;
        indexis[j++] = i + 1;
    }
    indexis[j-1] = 0;
} 
*/

// If EBO disabled

GLfloat* vertex_vector(GLuint num_of_angles){
    GLfloat* vertex_ans = (GLfloat*)malloc(36 * sizeof(GLfloat) * num_of_angles);
    int j = 0;
    for(float i = 0.0f; i < 2 * M_PI; i+= 2 * M_PI / num_of_angles){
        vertex_ans[j++] = -0.5*cos(i);
        vertex_ans[j++] = 0.0f;
        vertex_ans[j++] = 0.5*sin(i);
        vertex_ans[j++] = i / (2*M_PI);
        vertex_ans[j++] = i / (2*M_PI) * i / (2*M_PI);
        vertex_ans[j++] = 1.0f - i / (2*M_PI);
        vertex_ans[j++] = 0.0f;
        vertex_ans[j++] = PIRAMID_HEIGHT;
        vertex_ans[j++] = 0.0f;
        vertex_ans[j++] = i / (2*M_PI);
        vertex_ans[j++] = i / (2*M_PI) * i / (2*M_PI);
        vertex_ans[j++] = 1.0f - i / (2*M_PI);
        vertex_ans[j++] = -0.5*cos(i + 2 * M_PI / num_of_angles);
        vertex_ans[j++] = 0.0f;
        vertex_ans[j++] = 0.5*sin(i + 2 * M_PI / num_of_angles);    
        vertex_ans[j++] = i / (2*M_PI);
        vertex_ans[j++] = i / (2*M_PI) * i / (2*M_PI);
        vertex_ans[j++] = 1.0f - i / (2*M_PI);
    }
    for(float i = 0.0f; i < 2 * M_PI; i+= 2 * M_PI / num_of_angles){
        vertex_ans[j++] = -0.5*cos(i);
        vertex_ans[j++] = 0.0f;
        vertex_ans[j++] = 0.5*sin(i);
        vertex_ans[j++] = 0.0f;
        vertex_ans[j++] = 0.0f;
        vertex_ans[j++] = 0.0f;
        vertex_ans[j++] = 0.0f;
        vertex_ans[j++] = 0.0f;
        vertex_ans[j++] = 0.0f;
        vertex_ans[j++] = 0.0f;
        vertex_ans[j++] = 0.0f;
        vertex_ans[j++] = 0.0f;
        vertex_ans[j++] = -0.5*cos(i + 2 * M_PI / num_of_angles);
        vertex_ans[j++] = 0.0f;
        vertex_ans[j++] = 0.5*sin(i + 2 * M_PI / num_of_angles);    
        vertex_ans[j++] = 0.0f;
        vertex_ans[j++] = 0.0f;
        vertex_ans[j++] = 0.0f;
    }
    return vertex_ans;
}

const GLuint WIDTH = 800, HEIGHT = 600;

void new_func_size_callback(GLFWwindow* window, int width, int height){
    if(!height){
        height = 0;
    }
    glViewport(0, 0, width, height);
}

void GameLoop(GLFWwindow* window, Shader& shader, GLuint VAO, GLuint VBO, GLuint EBO){
    glm::mat4 model = glm::mat4(1.0f);
    const glm::mat4 first_conf = glm::mat4(1.0f);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        do_movement();

        int width, height;
        glfwGetWindowSize(window, &width, &height);


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.Use();

        glm::mat4 view;
        glm::mat4 projection;
        glm::mat4 rotation;
        glm::vec4 quaternion;
        xyz_rotation_to_quaternion(quaternion, x_rotation, y_rotation, z_rotation);
        quaternion_to_matrix(rotation, quaternion);
        glm::mat4 scalem = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
        if(ret){
            model = first_conf;
            ret = false;
        }else{
            model = rotation * scalem * model;
        }
        view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
        projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
        glm::mat4 MVP = projection * view * model;

        GLint modelLoc = glGetUniformLocation(shader.Program, "MVP");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(MVP));

        glBindVertexArray(VAO);

        if(EBO){
            glDrawElements(GL_TRIANGLES, 2*3*NUMBER_OF_LINES, GL_UNSIGNED_INT, 0);
        }else{
            glDrawArrays(GL_TRIANGLES, 0, 2*3*NUMBER_OF_LINES);
        }
        glBindVertexArray(0);

        glfwSwapBuffers(window);
       // getchar();
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
        return;
    }
    if(action == GLFW_PRESS) {
        keys[key] = true;
    }else if(action == GLFW_RELEASE){
        keys[key] = false;
    }
    return;
}

void do_movement(){
    if(keys[GLFW_KEY_UP]){
        scale = 1.04f;
    }else if(keys[GLFW_KEY_DOWN]){
        scale = 0.96f;
    }else{
        scale = 1.0f;
    }
    if(keys[GLFW_KEY_W]){
        x_rotation = glm::radians(5.0f);
    }else if(keys[GLFW_KEY_Q]){
        x_rotation = -glm::radians(5.0f);
    }else{
        x_rotation = 0.0f;
    }
    if(keys[GLFW_KEY_A]){
        y_rotation = -glm::radians(5.0f);
    }else if(keys[GLFW_KEY_S]){
        y_rotation = glm::radians(5.0f);
    }else{
        y_rotation = 0.0f;
    }
    if(keys[GLFW_KEY_Z]){
        z_rotation = -glm::radians(5.0f);
    }else if(keys[GLFW_KEY_X]){
        z_rotation = glm::radians(5.0f);
    }else{
        z_rotation = 0.0f;
    }
    if(keys[GLFW_KEY_R]){
        ret = true;
    }
}


int main()
{
    GLFWwindow* window = InitOpenGl(WIDTH, HEIGHT);

    Shader ourShader("shaders/shader.vs", "shaders/shader.frag");

    GLuint VBO, VAO, EBO;
    InitBuffers(VAO, VBO, EBO, NUMBER_OF_LINES);

    GameLoop(window, ourShader, VAO, VBO, EBO);

    DestroyOpenGl(VAO, VBO, EBO);

    return 0;
}