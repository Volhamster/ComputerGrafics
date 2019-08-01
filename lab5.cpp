#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

GLfloat L = 0.3f; //длина ребра отсекателя

bool ISCLIPP = false;

class Point2D {
public:
    GLfloat x;
    GLfloat y;
    
    Point2D(GLfloat x, GLfloat y) : x(x), y(y) {}
};

class Line {
public:
    Point2D p0;
    Point2D p1;
    Point2D n;
    Point2D outP0;
    Point2D outP1;
    
    bool DISPLAYING = true;
    
    double t;
    
    Line(const Point2D &p0, const Point2D &p1) : p0(p0), p1(p1), outP0(p0), outP1(p1), n(Point2D(0, 0)) {}
    
    void display(bool flag) {
        if (DISPLAYING) {
            glColor3d(1, 1, 1);
            glVertex2d(p0.x, p0.y);
            glVertex2d(p1.x, p1.y);
            
            if (flag) {
                glVertex2d(p0.x / 2 + p1.x / 2, p0.y / 2 + p1.y / 2);
                glVertex2d(n.x, n.y);
            }
        }
    }
    
    void display(int n){
        glColor3d(0.0, 0.8f, 0);
        glVertex2d(p0.x, p0.y);
        glVertex2d(outP0.x, outP0.y);
        glVertex2d(p1.x, p1.y);
        glVertex2d(outP1.x, outP1.y);
    }
};

double Skal(Point2D p1, Point2D p2) {
    return p1.x * p2.x + p1.y * p2.y;
}

vector<Line> lines;

void input(){
    cout << "Line:" << endl;
    
    pair<float, float> point1;
    pair<float, float> point2;
    
    scanf("%f", &point1.first);
    scanf("%f", &point1.second);
    
    scanf("%f", &point2.first);
    scanf("%f", &point2.second);
    
    lines.push_back(Line(Point2D(point1.first, point1.second), Point2D(point2.first, point2.second)));
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT){
        switch (key){
            case GLFW_KEY_F:
                ISCLIPP = !ISCLIPP;
                break;
            case GLFW_KEY_I:
                input();
                break;
        }
    }
}

int Chopping(double &Tin, double &To, double n, double dn) {
    double Th;
    if (dn < 0) {
        Th = n / dn;
        if (Th > To) return 0;
        else if (Th > Tin) Tin = Th;
        
    } else if (dn > 0) {
        Th = n / dn;
        if (Th < To) To = Th;
    } else {
        if (n <= 0) return 1;
        return 0;
    }
    return 0;
}


int main() {
    GLFWwindow *window;
    
    if (!glfwInit()) {
        return -1;
    }
    
    window = glfwCreateWindow(800, 600, "5", NULL, NULL);
    glfwSetKeyCallback(window, keyCallback);
    
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glfwMakeContextCurrent(window);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    vector<Line> CutWin;
    vector<Point2D> normals;
    
    CutWin.push_back(Line(Point2D(-L, -L), Point2D(L, -L)));
    CutWin.push_back(Line(Point2D(L, -L), Point2D(L, L)));
    CutWin.push_back(Line(Point2D(L, L),Point2D(-L, L)));
    CutWin.push_back(Line(Point2D(-L, L),Point2D(-L, -L)));
    
    for (int i = 0; i < CutWin.size(); i++) {
        CutWin[i].n = Point2D(CutWin[i].p1.y - CutWin[i].p0.y, -(CutWin[i].p1.x - CutWin[i].p0.x));
    }
    
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushMatrix();
        
        glBegin(GL_LINES);
        
        glColor3d(0.8, 0.0, 0.0);
        for (int i = 0; i < CutWin.size(); i++) CutWin[i].display(false);
        
        glColor3d(0.0f, 1, 1);
        if(!ISCLIPP){
            for (int i = 0; i < lines.size(); i++) {
                lines[i].DISPLAYING = true;
                lines[i].display(false);
            }
        }
        
        if (ISCLIPP) {
            for (int i = 0; i < lines.size(); i++) {
                double w, dn;
                double Tin = 0.0, Tout = 1.0;
                Point2D c = Point2D(lines[i].p1.x - lines[i].p0.x, lines[i].p1.y - lines[i].p0.y);
                
                for (int j = 0; j < CutWin.size(); j++) {
                    Point2D tmp = Point2D(CutWin[j].p0.x - lines[i].p0.x, CutWin[j].p0.y - lines[i].p0.y);
                    w = Skal(CutWin[j].n, tmp);
                    dn = Skal(CutWin[j].n, c);
                    if (!Chopping(Tin, Tout, w, dn)) lines[i].DISPLAYING = false;
                }
                if (Tout < 1.0 && Tout > 0.0) {
                    lines[i].outP1.x = lines[i].p0.x + c.x * Tout;
                    lines[i].outP1.y = lines[i].p0.y + c.y * Tout;
                }
                
                if (Tin > 0.0 && Tin < 1.0) {
                    lines[i].outP0.x = lines[i].p0.x + c.x * Tin;
                    lines[i].outP0.y = lines[i].p0.y + c.y * Tin;
                }
                
                for (int i = 0; i < lines.size(); i++) lines[i].display(1);
            }
        }
        glEnd();
        glPopMatrix();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}
