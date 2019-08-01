#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

GLfloat xMove = 0.0f;
GLfloat yMove = 0.0f;
GLfloat zMove = 0.0f;

GLfloat xRot = 0.0f;
GLfloat yRot = 0.0f;
GLfloat zRot = 0.0f;

GLfloat size = 1.0f;

GLboolean IS_FILL = false;

GLfloat cs45 = 0.70710678118;
GLfloat s3526 = 0.57728771208;
GLfloat c3526 = 0.81654081188;

const GLfloat kVerts[] = {
    cs45,   cs45 * s3526,       cs45 * c3526,       0,
    0,      c3526,              -s3526,             0,
    cs45,   -cs45 * s3526,      -cs45 * c3526,      0,
    0,      0,                  0,                  1
};


void keyboard(GLFWwindow * window, int key, int scancode, int action, int mods){
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
                
                //Сдвиг
            case GLFW_KEY_UP:
                yMove += 5.0f;
                break;
            case GLFW_KEY_DOWN:
                yMove -= 5.0f;
                break;
            case GLFW_KEY_LEFT:
                xMove -= 5.0f;
                break;
            case GLFW_KEY_RIGHT:
                xMove += 5.0f;
                break;
            case GLFW_KEY_I:
                zMove += 5.0f;
                break;
            case GLFW_KEY_O:
                zMove -= 5.0f;
                break;
                
                //Масштабирование
            case GLFW_KEY_B: //Big
                size += 0.05f;
                break;
            case GLFW_KEY_L: //Little
                size -= 0.05f;
                break;
                
                //Поворот
            case GLFW_KEY_W:
                xRot += 5.0f;
                break;
            case GLFW_KEY_S:
                xRot -= 5.0f;
                break;
            case GLFW_KEY_A:
                yRot += 5.0f;
                break;
            case GLFW_KEY_D:
                yRot -= 5.0f;
                break;
            case GLFW_KEY_R:
                zRot += 5.0f;
                break;
            case GLFW_KEY_E:
                zRot -= 5.0f;
                break;
                
                //Закраска
            case GLFW_KEY_F:
                IS_FILL = !IS_FILL;
                break;
        }
    }
}

void DrawCube(int k){
    GLint pVerts[] = {
        50 * k,    50 * k,    50 * k,
        50 * k,    -50 * k,   50 * k,
        -50 * k,   -50 * k,   50 * k,
        -50 * k,   50 * k,    50 * k,
        50 * k,    50 * k,    -50 * k,
        50 * k,    -50 * k,   -50 * k,
        -50 * k,   -50 * k,   -50 * k,
        -50 * k,   50 * k,    -50 * k
    };
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_INT, 0, pVerts);
    
    glPolygonMode(GL_FRONT_AND_BACK, IS_FILL ? GL_FILL : GL_LINE);
    
    glBegin(GL_QUADS);
    
    //Передняя - розовый
    glColor3ub(255, 0, 255);
    glArrayElement(0);
    glArrayElement(4);
    glArrayElement(7);
    glArrayElement(3);
    
    //Верхняя - красный
    glColor3ub(255, 0, 0);
    glArrayElement(0);
    glArrayElement(1);
    glArrayElement(2);
    glArrayElement(3);
    
    //Правая - зеленый
    glColor3ub(0, 225, 0);
    glArrayElement(0);
    glArrayElement(1);
    glArrayElement(5);
    glArrayElement(4);
    
    //Нижняя - синий
    glColor3ub(0, 0, 255);
    glArrayElement(4);
    glArrayElement(5);
    glArrayElement(6);
    glArrayElement(7);
    
    //Левая - желтый
    glColor3ub(255, 255, 0);
    glArrayElement(3);
    glArrayElement(2);
    glArrayElement(6);
    glArrayElement(7);
    
    //Дальняя - голубой
    glColor3ub(0, 255, 255);
    glArrayElement(1);
    glArrayElement(5);
    glArrayElement(6);
    glArrayElement(2);
    
    glEnd();
}

int main(){
    if (!glfwInit()) exit(1);
    
    GLFWwindow * window = glfwCreateWindow(800, 600, "2А", NULL, NULL);
    if (!window){
        glfwTerminate();
        exit(1);
    }
    
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyboard);
    
    int width, height;
    
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-400, 400, -300, 300, -500, 500);
    glMultMatrixf(kVerts);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        //маленький куб
        glPushMatrix();
        glTranslatef(-200, -200, -250);
        glRotatef(15, 1, 0, 0);
        glRotatef(-15, 0, 1, 0);
        DrawCube(1);
        glPopMatrix();
        
        //Большой куб
        glPushMatrix();
        glScalef(size, size, size);
        glTranslatef(xMove, yMove, zMove);
        glRotatef(xRot, 1, 0, 0);
        glRotatef(yRot, 0, 1, 0);
        glRotatef(zRot, 0, 0, 1);
        DrawCube(2);
        glPopMatrix();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
