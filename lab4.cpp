#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265

using namespace std;

GLint px, py=0;
GLint width=1080, height=1080;
GLint zoom = 2;
static unsigned int* byts_arr;
GLint color= 0 | 255<<16 | 255<<24 | 255<<8;

GLboolean drc = true;
GLboolean IS_FILTER = true;
GLboolean IS_FILL = false;

struct Activ{
    int yMax;
    double dx;
    int xStart;
    double x;
};

struct Y_N{
    int n;
    int y;
    int x;
}*vert;

struct Vertices{
    double *vertex;
    int count=0;
} points, intersept;

int compare(const void * a, const void * b){
    return ( (*(Y_N*)a).y - (*(Y_N*)b).y);
}

void new_vertex(Vertices *t){
    double *newV= new double[t->count+2];
    
    for(int i= 0; i< t->count; i++){
        newV[i]=t->vertex[i];
    }
    
    delete [] t->vertex;
    t->vertex = newV;
    t->count+=2;
    t->vertex[t->count-2]=px;
    t->vertex[t->count-1]=py;
    
    return;
}
void filling(){
    delete [] vert;
    vert = new Y_N[points.count/2];
    vector<Activ> v;
    for (int i=0; i<points.count/2; i++){
        vert[i].n= i;
        vert[i].y= points.vertex[i*2+1];
        vert[i].x= points.vertex[i*2];
    }
    
    qsort(vert, points.count/2,  sizeof(Y_N), compare);
    int yMin=vert[0].y;
    int i=0, y=yMin;
    while (true){
        while (vert[i].y==y){
            Activ t;
            int n= vert[i].n, n0= (n==0? 2*(points.count/2-1)+1 : 2*((n-1)%(points.count/2))+1), n1=2*((n+1)%(points.count/2))+1;
            t.yMax= points.vertex[n*2+1] > points.vertex[n0] ? points.vertex[n*2+1] :  points.vertex[n0];
            t.xStart= vert[i].x;
            t.dx= double((points.vertex[n0-1]-vert[i].x))/abs((vert[i].y-points.vertex[n0]));
            t.x=vert[i].x;
            if (t.yMax>y) v.push_back(t);
            
            t.x=vert[i].x;
            t.yMax= points.vertex[n*2+1] > points.vertex[n1] ? points.vertex[n*2+1] :  points.vertex[n1];
            t.xStart= vert[i].x;
            t.dx= double((points.vertex[n1-1]-vert[i].x))/(abs(vert[i].y-points.vertex[n1]));
            if (t.yMax>y) v.push_back(t);
            i++;
        }
        if (v.size()==0) return;
        
        int yNext=10000;
        
        for (int l=0; l<points.count/2; l++){
            if (vert[i].y>y && vert[i].y<yNext) yNext=vert[i].y;
        }
        
        for (;y<yNext;y++){
            sort( v.begin( ), v.end( ), [ ]( const Activ& a, const Activ& b ){return a.x < b.x;});
            
            for (int k=0; k<v.size(); k+=2){
                for (int x= int(v[k].x+1); x<=v[k+1].x; x++) byts_arr[(width-y-50)*width+x]=255;
            }
            for (int k=0; k<v.size(); k++) v[k].x+=v[k].dx;
        }
        for (int p=0; p< v.size(); p++){
            if (v[p].yMax==y){
                v.erase(v.begin()+p);
                p--;
            }
        }
    }
    delete [] vert;
    return;
}

void drawLine(int x1, int y1, int x2, int y2){
    int dx = (x2- x1);
    int dy = (y2- y1);
    int sx = x1< x2? 1: -1;
    int sy = y1 < y2? 1: -1;
    if (dx==0) return;
    double tg = (double) dy/dx;
    bool intense = tg>0;
    bool swaped = -1>tg || tg>1;
    if (swaped) {
        swap(x1,y1);
        swap(x2,y2);
        swap(dx,dy);
        swap(sx,sy);
    }
    dx = abs(dx);
    dy = abs(dy);
    int error = dx - dy;
    
    int I = 255;
    int e = I*dx;
    int m = 2*dy*I;
    int w = 2*e;
    
    if (swaped) byts_arr[(width-x2-50)*width+y2]=(unsigned char)(intense ? m/(4*dx) : 255 - m/(4*dx));
    byts_arr[(width-y2-50)*width+x2]=255;
    while (x1!= x2 || y1!= y2) {
        int error2 = error * 2;
        
        if (IS_FILTER){
            if (swaped) byts_arr[(width-x1-50)* width+y1]=255;
            else byts_arr[(width-y1-50)* width+ x1]=255;
        } else {
            if (swaped) byts_arr[(width- x1- 50)* width+ y1]= (unsigned char)(drc^intense ? e/(2*dx) : 255 - e/(2*dx));
            else byts_arr[(width-y1-50)* width+ x1]=(unsigned char)(drc^(!intense) ? e/(2*dx) : 255 - e/(2*dx));
        }
        
        if (error2>-dy){
            error-=dy;
            x1+= sx;
            e+=m;
        }
        if (error2< dx){
            error+= dx;
            y1+= sy;
            e+=-w;
        }
    }
    return;
}

void drawing(){
    for (int i=0; i< width*height; byts_arr[i]=color, i++);
    if (IS_FILL) filling();

    for (int i=0; i< points.count/2; i++){
        drawLine(points.vertex[i*2], points.vertex[i*2+1], points.vertex[(i+1)*2%points.count], points.vertex[((i+1)*2+1)%points.count]);
    }
    
    glRasterPos2f(-1, -1);
    glPixelZoom(zoom, zoom);
    glDrawPixels(int(width), int(height), GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, byts_arr);
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods){
    if( action == 1) return;
    switch (key){
        case GLFW_KEY_F:
            IS_FILTER=!IS_FILTER;
            break;
        case GLFW_KEY_P:
            IS_FILL=!IS_FILL;
            break;
        case GLFW_KEY_C:
            points.count=0;
            IS_FILL=false;;
            break;
        case GLFW_KEY_B:
            zoom++;
            break;
        case GLFW_KEY_L:
            zoom--;
            if (zoom == 0) zoom = 1;
            break;
        case GLFW_KEY_ESCAPE:
            if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED) glfwSetWindowShouldClose(window, true);
            break;
        default:
            break;
    }
    return;
}

bool oriantation(Vertices * a){
    double S=0;
    for (int i=0; i<a->count/2; i++){
        int n1 = i*2, n2=(i+1)%(a->count/2)*2;
        S+=(a->vertex[n2] - a->vertex[n1]) * (a->vertex[n2+1] + a->vertex[n1+1]);
    }
    return S>0;
}

void mouse(GLFWwindow* window, int button, int action, int mods){
    for (int i=0; i< width*height; byts_arr[i]=color , i++);
    if (action==0 && mods==0) {
        new_vertex(&points);
        drc = oriantation(&points);
    }
    return;
}

void mouse_pos(GLFWwindow* window, double xpos, double ypos){
    px = xpos;
    py = ypos;
    return;
}

int main(int argc, const char *argv[]){
    if (!glfwInit()) exit(1);
    
    GLFWwindow* window = glfwCreateWindow(width, height, "lab4", NULL, NULL);
    
    glfwMakeContextCurrent(window);
    glClearColor(1, 1, 1, 0.0f);
    
    glfwSetCursorPosCallback(window, mouse_pos);
    glfwSetKeyCallback(window, keyboard);
    glfwSetMouseButtonCallback(window, mouse);
    
    byts_arr = new unsigned int[width*height];
    for (int i=0; i < width*height; byts_arr[i]=color, i++);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    
    while (!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        drawing();
        glfwSwapBuffers(window);
        glfwWaitEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    delete [] byts_arr;
    
    return(0);
}
