#include <iostream>
#include <stdlib.h>
#include <GLFW/glfw3.h>

static void keyboard(GLFWwindow* window, int key, int scancode, int press, int mods){
    if (key == GLFW_KEY_ESCAPE && press == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
    
    if (key == GLFW_KEY_R && press == GLFW_PRESS) glClearColor(1, 0, 0, 0); //red
    if (key == GLFW_KEY_G && press == GLFW_PRESS) glClearColor(0, 1, 0, 0); //green
    if (key == GLFW_KEY_P && press == GLFW_PRESS) glClearColor(0, 0, 1, 0); //purple
    if (key == GLFW_KEY_Y && press == GLFW_PRESS) glClearColor(1, 1, 0, 0); //yellow
    if (key == GLFW_KEY_B && press == GLFW_PRESS) glClearColor(0, 1, 1, 0); //blue
    if (key == GLFW_KEY_M && press == GLFW_PRESS) glClearColor(1, 0, 1, 0); //magenta
    if (key == GLFW_KEY_W && press == GLFW_PRESS) glClearColor(1, 1, 1, 0); //white
    if (key == GLFW_KEY_D && press == GLFW_PRESS) glClearColor(0, 0, 0, 0); //dark
}

void cursor(GLFWwindow* window, int entered){
    if(entered){
        glClearColor(1, 1, 1, 0); //white
    }else{
        glClearColor(0, 0, 0, 0); //black
    }
}

void mouse(GLFWwindow* window, int button, int press, int mods){
    if(button == GLFW_MOUSE_BUTTON_RIGHT && press == GLFW_PRESS) glClearColor(0.2, 0.2, 0.2, 0); //bright
    if(button ==  GLFW_MOUSE_BUTTON_LEFT && press == GLFW_PRESS) glClearColor(0.8, 0.8, 0.8, 0); //dark
}

int main() {
    if(!glfwInit()) exit(EXIT_FAILURE);
    
    GLFWwindow* window = glfwCreateWindow(1000, 1000, "Quadratik", nullptr, nullptr);
    if (window == nullptr){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    glfwSetKeyCallback(window, keyboard);
    glfwSetCursorEnterCallback(window, cursor);
    glfwSetMouseButtonCallback(window, mouse);
    
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(0.517, 0.521, 0.588); //grey
        glBegin(GL_POLYGON);
        glVertex3f(0.5,   0.5, 0.0);
        glVertex3f(-0.5,  0.5, 0.0);
        glVertex3f(-0.5, -0.5, 0.0);
        glVertex3f(0.5,  -0.5, 0.0);
        glEnd();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
