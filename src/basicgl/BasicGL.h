#ifndef _SIMPLE_GL_H_
#define _SIMPLE_GL_H_

#include <string>
#include <vector>
#include <cfloat>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <time.h>
#include <list>

#define MODE_2D 0
#define MODE_3D 1

#define  KEY_F1                        0x0001
#define  KEY_F2                        0x0002
#define  KEY_F3                        0x0003
#define  KEY_F4                        0x0004
#define  KEY_F5                        0x0005
#define  KEY_F6                        0x0006
#define  KEY_F7                        0x0007
#define  KEY_F8                        0x0008
#define  KEY_F9                        0x0009
#define  KEY_F10                       0x000A
#define  KEY_F11                       0x000B
#define  KEY_F12                       0x000C
#define  KEY_LEFT                      0x0064
#define  KEY_UP                        0x0065
#define  KEY_RIGHT                     0x0066
#define  KEY_DOWN                      0x0067
#define  KEY_PAGE_UP                   0x0068
#define  KEY_PAGE_DOWN                 0x0069
#define  KEY_HOME                      0x006A
#define  KEY_END                       0x006B
#define  KEY_INSERT                    0x006C

#define ELEMENT_CONTAINER 0
#define ELEMENT_POINT 1
#define ELEMENT_LINE 2
#define ELEMENT_POLYGON 3
#define ELEMENT_TRIANGLE 4
#define ELEMENT_QUAD 5
#define ELEMENT_SPHERE 6
#define ELEMENT_CONE 7
#define ELEMENT_CUBE 8
#define ELEMENT_CYLINDER 9
#define ELEMENT_TEAPOT 10

static float ABS(float x) 
{
    if(x >= 0)
        return x;
    return -x;
}

typedef struct BasicGLPoint_st
{
    float color[4];
    float xyz[3];

    BasicGLPoint_st()
    {
        color[0] = color[1] = color[2] = color[3] = 1.0f;
        xyz[0] = xyz[1] = xyz[2] = 0.0f;
    }

    struct BasicGLPoint_st* translate(float x, float y, float z)
    {
        xyz[0] += x;
        xyz[1] += y;
        xyz[2] += z;
        return this;
    }

    struct BasicGLPoint_st* moveTo(float x, float y, float z)
    {
        xyz[0] = x;
        xyz[1] = y;
        xyz[2] = z;
        return this;
    }

    struct BasicGLPoint_st* rgb(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255)
    {
        color[0] = r/255.0f;
        color[1] = g/255.0f;
        color[2] = b/255.0f;
        color[3] = a/255.0f;
        return this;
    }

    struct BasicGLPoint_st* rgb(float r = 0, float g = 0, float b = 0, float a = 1.0f)
    {
        color[0] = r;
        color[1] = g;
        color[2] = b;
        color[3] = a;
        return this;
    }

}BasicGLPoint;

typedef struct BasicGLElement_st
{
    int element;
    int size;
    bool wirefrane;
    float scales[3];
    float position[3];
    float rotation[3];
    float color[4];
    BasicGLPoint *points;
    void *data;
    std::list<struct BasicGLElement_st*> elements;


    BasicGLElement_st()
    {
        element = ELEMENT_POINT;
        size = 0;
        wirefrane = false;
        scales[0] = scales[1] = scales[2] = 1.0f;
        position[0] = position[1] = position[2] = 0;
        rotation[0] = rotation[1] = rotation[2] = 0;
        points = NULL;
        color[0] = color[1] = color[2] = color[3] = 1.0f;
        data = NULL;
    }

    struct BasicGLElement_st* reshape(int n)
    {
        if(points != NULL)
            delete[] points;
        if(n == 0)
            points = NULL;
        else
            points = new BasicGLPoint[n];
        size = n;
        return this;
    }

    struct BasicGLElement_st* CreateElement(int element);

    struct BasicGLElement_st* translate(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
        position[0] += x;
        position[1] += y;
        position[2] += z;
        return this;
    }

    struct BasicGLElement_st* moveTo(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
        position[0] = x;
        position[1] = y;
        position[2] = z;
        return this;
    }

    struct BasicGLElement_st* rotate(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
        rotation[0] += x;
        rotation[1] += y;
        rotation[2] += z;
        return this;
    }

    struct BasicGLElement_st* rotateTo(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
        rotation[0] = x;
        rotation[1] = y;
        rotation[2] = z;
        return this;
    }

    struct BasicGLElement_st* scale(float x = 0.0f, float y = FLT_MIN, float z = FLT_MIN)
    {
        if(y == FLT_MIN) y = x;
        if(z == FLT_MIN) z = y;
        scales[0] += x;
        scales[1] += y;
        scales[2] += z;
        return this;
    }

    struct BasicGLElement_st* scaleTo(float x = 1.0f, float y = FLT_MIN, float z = FLT_MIN)
    {
        if(y == FLT_MIN) y = x;
        if(z == FLT_MIN) z = y;
        scales[0] = x;
        scales[1] = y;
        scales[2] = z;
        return this;
    }

    struct BasicGLElement_st* rgb(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255)
    {
        color[0] = r/255.0f;
        color[1] = g/255.0f;
        color[2] = b/255.0f;
        color[3] = a/255.0f;
        for(int i = 0; i < size; i++)
            points[i].rgb(r, g, b, a);
        return this;
    }

    struct BasicGLElement_st* rgb(float r = 0, float g = 0, float b = 0, float a = 1.0f)
    {
        color[0] = r;
        color[1] = g;
        color[2] = b;
        color[3] = a;
        for(int i = 0; i < size; i++)
            points[i].rgb(r, g, b, a);
        return this;
    }

    void draw();

}BasicGLElement;

typedef struct BasicGLKeyboard_st
{
    int key;
    int x, y;
    bool shift, ctrl, alt;
}&BasicGLKeyboard;

typedef struct BasicGLMouse_st
{
    bool left, middle, right;
    bool pressed, released, move, entered, leave;
    int x, y;
}&BasicGLMouse;

struct BasicGLWindow_st;

typedef const std::vector<BasicGLElement*>& BasicGLElements;
typedef void (*AnimationFunction)(BasicGLElements elements, struct BasicGLWindow_st *window, float ellapsedTime);
typedef void (*KeyboardFunction)(BasicGLKeyboard keyboard, struct BasicGLWindow_st *window);
typedef void (*MouseFunction)(BasicGLMouse mouse, struct BasicGLWindow_st *window);

typedef struct BasicGLCamera_st
{
    float rotation[3];
    float position[3];
    float translation[3];

    BasicGLCamera_st()
    {
        rotation[0] = rotation[1] = rotation[2] = 0;
        translation[0] = 0;
        translation[1] = 0;
        translation[2] = -1.0f;
        position[0] = 0;
        position[1] = 0;
        position[2] = 5.0f;
    }

    struct BasicGLCamera_st* translate(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
        position[0] += x;
        position[1] += y;
        position[2] += z;
        return this;
    }

    struct BasicGLCamera_st* moveTo(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
        position[0] = x;
        position[1] = y;
        position[2] = z;
        return this;
    }

    struct BasicGLCamera_st* rotate(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
        rotation[0] += x;
        rotation[1] += y;
        rotation[2] += z;
        translation[0] = cos(rotation[1]) * -sin(rotation[0]);
        translation[1] = sin(rotation[1]);
        translation[2] = cos(rotation[1]) * -cos(rotation[0]);
        return this;
    }

    struct BasicGLCamera_st* rotateTo(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
        rotation[0] = x;
        rotation[1] = y;
        rotation[2] = z;
        translation[0] = cos(rotation[1]) * sin(rotation[0]);
        translation[1] = sin(rotation[1]);
        translation[2] = cos(rotation[1]) * cos(rotation[0]);
        return this;
    }

}BasicGLCamera;

typedef struct BasicGLWindow_st
{
    int id;
    int index;
    int mode;
    BasicGLCamera camera;
    float bg[4];
    float timeSinceBegin;
    struct timespec lastTime;

    std::vector<BasicGLElement*> elements;
    AnimationFunction animationFunction;
    KeyboardFunction keyboardFunction;
    MouseFunction mouseFunction;

    BasicGLWindow_st()
    {
        bg[0] = bg[1] = bg[2] = 0.0f;
        bg[3] = 1.0f;
        animationFunction = NULL;
        timeSinceBegin = 0;
        memset(&lastTime, '\0', sizeof(lastTime));
    }

}BasicGLWindow;

class BasicGL
{
    public:
        static void Init(int argc, char *argv[]);
        static void Destroy();

        static int CreateWindow(const char *name, int mode = MODE_2D, int width = 0, int height = 0, int x = -1, int y = -1);
        static int CreateWindow(const std::string name, int mode = MODE_2D, int width = 0, int height = 0, int x = -1, int y = -1);

        static void SelectWindow(int index);
        static void SetBackground(float r, float g, float b, float a = 1.0f);
        static void SetBackground(int r, int g, int b, int a = 255);
        static void SetAnimationFunction(AnimationFunction animationFunction);
        static void SetKeyboardFunction(KeyboardFunction keyboardFunction);
        static void SetMouseFunction(MouseFunction mouseFunction);

        static BasicGLElement* CreateElement(int element);

        static void Run();
    
    private:
        static std::vector<BasicGLWindow> windows;
        static int currentWindow;
        static int WindowIndex();

        static void Render();
        static void KeyboardNormal(unsigned char key, int x, int y);
        static void KeyboardSpecial(int key, int x, int y);
        static void MouseButtons(int button, int state, int x, int y);
        static void MouseMotion(int x, int y);
        static void MouseEntry(int state);
        static void Resize(int w, int h);
        static void Timer(int index);
};

#endif
