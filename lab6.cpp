#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <GLUT/GLUT.h>
#include <string>
#include <unordered_map>
#include "SOIL.h"

GLint width=800, height=600, dep=1000;
GLint wbox=600, hbox=300, dbox=500;

GLfloat xMove = 0.0f;
GLfloat yMove = 0.0f;
GLfloat zMove = 0.0f;

GLfloat xRot = 0.0f;
GLfloat yRot = 0.0f;
GLfloat zRot = 0.0f;

GLfloat size = 1.0f;

GLboolean IS_FILL = true;

GLfloat cs45 = 0.70710678118;
GLfloat s3526 = 0.57728771208;
GLfloat c3526 = 0.81654081188;

GLfloat GDepth = 10;
GLfloat GLength = 200;
GLfloat GLrx = 50;
GLfloat GLry = 100;
GLfloat GLCenterX=0;
GLfloat GLCenterY=40;
GLfloat GLN=10;

GLfloat speedX = 1.0f;
GLfloat speedY = 1.0f;
GLfloat speedZ = 1.0f;

GLboolean global_ambient = false;
GLboolean two_side = true;
GLboolean local_viewer = true;

GLboolean IS_TEXTURE = true;
GLuint textureID;

GLfloat timer = glfwGetTime(), dt;

void makeLight() {
    float light0_diffuse[] = {0.8f, 0.8f, 0.8f};
    float light0_position[] = {-390, 290, 300, 1.0};
    float light0_ambient[] = {0.5, 0.5, 0.5, 1.0};
    //float specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
    float light0_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float ambient[] = {0.5f, 0.2f, 0.2f, 1.0f};
    float default_amb[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    if (global_ambient)
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    else
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, default_amb);
    if (two_side)
        glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    else
        glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    if (local_viewer)
        glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    else
        glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
}

void animation(int value){
    if (xMove > wbox - GLength || xMove < -wbox) speedX = -speedX;  // достижение края экрана по X
    if (yMove > hbox || yMove < - hbox + GLength) speedY = -speedY;  // Y
    xMove += speedX;   // сдвигаем координаты на заданный шаг
    yMove += speedY;
    if (xMove > (wbox-GLength+speedX)) xMove = wbox-GLength-1;
    else if (xMove < -(width + speedX)) xMove = -wbox - 1;
    if (yMove > (hbox+speedY)) yMove = hbox-1;
    else if(yMove < -(hbox-GLength+speedY)) yMove = -hbox+GLength-1;
    
    glutPostRedisplay();
    glutTimerFunc(10, animation, 1);
}

void save(){
    std::ofstream t;
    t.open("./textfile.txt");
    if (t.is_open()){
        t << "xmove = (" << xMove << ") ymove = ("<< yMove << ") zmove = ("<< zMove << ")";
    }
}

void load(){
    std::ifstream file("./textfile.txt");
    std::string s, t="";
    getline(file, s);
    file.close();
    int i=0;
    
    for (; s[i]!= '('; i++);
    i++;
    for (; s[i]!= ')'; t+=s[i++]);
    xMove = std::stod(t);
    t="";
    
    for (; s[i]!= '('; i++);
    i++;
    for (; s[i]!= ')'; t+=s[i++]);
    yMove = std::stod(t);
    t="";
    
    for (; s[i]!= '('; i++);
    i++;
    for (; s[i]!= ')'; t+=s[i++]);
    zMove = std::stod(t);
    t="";
}

void makeTexture(float xCenter, float yCenter, float length, float rx, float ry, float pointCount, float n) {
    xCenter/=2;
    yCenter/=2;
    
    if (pointCount<3) pointCount=3;
    const float step = float(2 * M_PI) / pointCount;
    
    float dx1=0, dy1=0, dx2=0, dy2=0;
    
    float len = length/2;
    
    if (n<2) n=3;
    float s=2*len/n;
    
    float delx = 2*xCenter/n;
    float dely = 2*yCenter/n;
    
    glPolygonMode(GL_FRONT_AND_BACK, IS_FILL ? GL_FILL : GL_LINE);
    glFrontFace(GL_CW);
    glColor3d(0.85,0.1,0.5);
    
    for (float angle = 0; angle <= float(2 * M_PI); angle += step) {
        glBegin(GL_TRIANGLES);
        
        glVertex3f(xCenter, yCenter, len);
        dx1 = rx * cosf(angle);
        dy1 = ry * sinf(angle);
        glVertex3f(dx1 + xCenter, dy1 + yCenter, len);
        dx2 = rx * cosf(angle+step);
        dy2 = ry * sinf(angle+step);
        glVertex3f(dx2 + xCenter, dy2 + yCenter, len);
        glEnd();
        
        glColor3d(0.85,0.1,0.5);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(dx1 + xCenter, dy1 + yCenter, len);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(dx2 + xCenter, dy2 + yCenter, len);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(dx2 - xCenter, dy2 - yCenter, -len);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(dx1 - xCenter, dy1 - yCenter, -len);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        
        glBegin(GL_TRIANGLES);
        glVertex3f(-xCenter, -yCenter, -len);
        glVertex3f(dx1 - xCenter, dy1 - yCenter, -len);
        glVertex3f(dx2 - xCenter, dy2 - yCenter, -len);
        glEnd();
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
                
                //Изменение каркаса
            case GLFW_KEY_5:
                GLN-=1;
                break;
            case GLFW_KEY_6:
                GLN+=1;
                break;
                
            case GLFW_KEY_T:
                IS_TEXTURE = !IS_TEXTURE;
                break;
                
            case GLFW_KEY_N:
                save();
                break;
                
            case GLFW_KEY_M:
                load();
                break;
        }
    }
}

void DrawFigure(float xCenter, float yCenter, float length, float rx, float ry, float pointCount, float n) {
    xCenter/=2;
    yCenter/=2;
    
    if (pointCount<3) pointCount=3;
    const float step = float(2 * M_PI) / pointCount;
    
    float dx1=0, dy1=0, dx2=0, dy2=0;
    
    float len = length/2;
    
    if (n<2) n=3;
    float s=2*len/n;
    
    float delx = 2*xCenter/n;
    float dely = 2*yCenter/n;
    
    glPolygonMode(GL_FRONT_AND_BACK, IS_FILL ? GL_FILL : GL_LINE);
    
    for (float angle = 0; angle <= float(2 * M_PI); angle += step) {
        glColor3d(0, 0, 1);
        glBegin(GL_TRIANGLES);
        glVertex3f(xCenter, yCenter, len);
        dx1 = rx * cosf(angle);
        dy1 = ry * sinf(angle);
        glVertex3f(dx1 + xCenter, dy1 + yCenter, len);
        dx2 = rx * cosf(angle+step);
        dy2 = ry * sinf(angle+step);
        glVertex3f(dx2 + xCenter, dy2 + yCenter, len);
        glEnd();
        
        glColor3d(1, 0, 0);
        glBegin(GL_QUADS);
        glVertex3f(dx1 + xCenter, dy1 + yCenter, len);
        glVertex3f(dx2 + xCenter, dy2 + yCenter, len);
        glVertex3f(dx2 - xCenter, dy2 - yCenter, -len);
        glVertex3f(dx1 - xCenter, dy1 - yCenter, -len);
        glEnd();
        
        glColor3d(1, 0, 1);
        glBegin(GL_TRIANGLES);
        glVertex3f(-xCenter, -yCenter, -len);
        glVertex3f(dx1 - xCenter, dy1 - yCenter, -len);
        glVertex3f(dx2 - xCenter, dy2 - yCenter, -len);
        glEnd();
        
        for (float stage=s, i=xCenter - delx, j=yCenter - dely; stage<2*len-1; stage+=s, i-=delx, j-=dely){
            glColor3d(1, 0, 0);
            glBegin(GL_TRIANGLES);
            glVertex3f(i, j, len-stage);
            glVertex3f(dx1 + i, dy1 + j, len-stage);
            glVertex3f(dx2 + i, dy2 + j, len-stage);
            glEnd();
        }
    }
}

int main(){
    if (!glfwInit()) exit(1);
    
    GLFWwindow * window = glfwCreateWindow(width, height, "6", NULL, NULL);
    if (!window){
        glfwTerminate();
        exit(1);
    }
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
    
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyboard);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        int w=512, h=512;
        unsigned char* image = SOIL_load_image("./metaljv2.bmp", &w, &h, 0, SOIL_LOAD_RGB);
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-width/2, width/2, -height/2, height/2, -500, 500);
        //glMultMatrixf(kVerts);
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
        
        if (IS_TEXTURE) {
            glEnable(GL_TEXTURE_2D);
            makeTexture(GLCenterX, GLCenterY, GLength, GLrx, GLry, GDepth, GLN);
        }
        else {
            glDisable(GL_TEXTURE_2D);
            DrawFigure(GLCenterX, GLCenterY, GLength, GLrx, GLry, GDepth, GLN);
        }
        glPopMatrix();
        
        makeLight();
        glutTimerFunc(10, animation, 1);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
