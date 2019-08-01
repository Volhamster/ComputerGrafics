#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>

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

GLfloat GDepth = 10;
GLfloat GLength = 200;
GLfloat GLrx = 50;
GLfloat GLry = 100;
GLfloat GLCenterX=0;
GLfloat GLCenterY=40;

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
            case GLFW_KEY_LEFT:
                xMove -= 5.0f;
                break;
            case GLFW_KEY_RIGHT:
                xMove += 5.0f;
                break;
            case GLFW_KEY_UP:
                yMove += 5.0f;
                break;
            case GLFW_KEY_DOWN:
                yMove -= 5.0f;
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
            case GLFW_KEY_Q:
                zRot += 5.0f;
                break;
            case GLFW_KEY_E:
                zRot -= 5.0f;
                break;
                
                //Закраска
            case GLFW_KEY_F:
                IS_FILL = !IS_FILL;
                break;
                
                //Разбиение
            case GLFW_KEY_Z:
                GDepth+=1;
                break;
            case GLFW_KEY_X:
                GDepth-=1;
                break;
                
                //Высота образующей
            case GLFW_KEY_C:
                GLength+=10;
                break;
            case GLFW_KEY_V:
                GLength-=10;
                break;
                
                //Растягивания диаметров
            case GLFW_KEY_1:
                GLrx+=10;
                break;
            case GLFW_KEY_2:
                GLrx-=10;
                break;
            case GLFW_KEY_3:
                GLry+=10;
                break;
            case GLFW_KEY_4:
                GLry-=10;
                break;
        }
    }
}

void DrawFigure(float xCenter, float yCenter, float length, float rx, float ry, float pointCount) {
    xCenter/=2;
    yCenter/=2;
    
    if (pointCount<3) pointCount=3;
    const float step = float(2 * M_PI) / pointCount;

    float dx1=0, dy1=0, dx2=0, dy2=0;
    
    float len = length/2;
    
    glPolygonMode(GL_FRONT_AND_BACK, IS_FILL ? GL_FILL : GL_LINE);
    
    for (float angle = 0; angle <= float(2 * M_PI); angle += step) {
        glColor3d(0,1,0);
        glBegin(GL_TRIANGLES);
        glVertex3f(xCenter, yCenter, len);
        dx1 = rx * cosf(angle);
        dy1 = ry * sinf(angle);
        glVertex3f(dx1 + xCenter, dy1 + yCenter, len);
        dx2 = rx * cosf(angle+step);
        dy2 = ry * sinf(angle+step);
        glVertex3f(dx2 + xCenter, dy2 + yCenter, len);
        glEnd();
        
        glColor3d(1, 0, 1);
        glBegin(GL_QUADS);
        glVertex3f(dx1 + xCenter, dy1 + yCenter, len);
        glVertex3f(dx2 + xCenter, dy2 + yCenter, len);
        glVertex3f(dx2 - xCenter, dy2 - yCenter, -len);
        glVertex3f(dx1 - xCenter, dy1 - yCenter, -len);
        glEnd();
        
        glColor3d(1, 1, 0);
        glBegin(GL_TRIANGLES);
        glVertex3f(-xCenter, -yCenter, -len);
        glVertex3f(dx1 - xCenter, dy1 - yCenter, -len);
        glVertex3f(dx2 - xCenter, dy2 - yCenter, -len);
        glEnd();
    }
}

int main(){
    if (!glfwInit()) exit(1);
    
    int width=800, height=600;
    
    GLFWwindow * window = glfwCreateWindow(width, height, "3", NULL, NULL);
    if (!window){
        glfwTerminate();
        exit(1);
    }
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
    
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyboard);
    
    glEnable(GL_DEPTH_TEST);
    
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-width/2, width/2, -height/2, height/2, -500, 500);
        glMultMatrixf(kVerts);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        //маленький куб
        glPushMatrix();
        glTranslatef(-200, -200, -250);
        glRotatef(15, 1, 0, 0);
        glRotatef(-15, 0, 1, 0);
        DrawCube(1);
        glPopMatrix();
        
        //наклонный эллиптический цилиндр
        glPushMatrix();
        glScalef(size, size, size);
        glTranslatef(xMove, yMove, zMove);
        glRotatef(xRot, 1, 0, 0);
        glRotatef(yRot, 0, 1, 0);
        glRotatef(zRot, 0, 0, 1);
        DrawFigure(GLCenterX, GLCenterY, GLength, GLrx, GLry, GDepth);
        glPopMatrix();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
