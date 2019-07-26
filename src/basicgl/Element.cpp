#include "Element.hpp"
#include "Serie.hpp"
#include "Plot.hpp"
#include <cmath>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <GL/gl.h>
#include <iostream>
#include <unistd.h>

using namespace BasicGL;
using namespace std;

Element::Element(Elements element) : element(element)
{
    visible = true;
    wireframe = false;
    scales[0] = scales[1] = scales[2] = 1.0f;
    position[0] = position[1] = position[2] = 0;
    rotation[0] = rotation[1] = rotation[2] = 0;
    lineWidth = 1.0f;
    color[0] = color[1] = color[2] = color[3] = 1.0f;
    data = NULL;
    init();
}

Element::~Element()
{

}

void Element::init()
{
    switch(element)
    {
        case POINT:
        case LINE:
        case TRIANGLE:
        case RECTANGLE:
            reshape(1);
            break;
        case POLYGON:
        case SEQUENCE:
            reshape(0);
            break;
        case CIRCLE:
            reshape(36);
            break;
        default:
            reshape(0);
            break;
    }
    
    switch(element)
    {
        case POINT:
            point(0, 0, 0);
            glow();
            break;
        case LINE:
            line(-0.5f, 0, 0, 0.5f, 0, 0);
            glow();
            break;
        case TRIANGLE:
            triangle(0, -0.5f, 0, -0.5f, 0.5f, 0, 0.5f, 0.5f, 0);
            glow();
            break;
        case RECTANGLE:
            rectangle(-0.5f, -0.5f, 0, 0.5f, 0.5f, 0);
            glow();
            break;
        case CIRCLE:
            circle(0, 0, 0, 0.5f);
            glow();
            break;
        default:
            break;
    }
}

ElementPtr Element::reshape(int n, bool byElement)
{
    int m = 1;
    if(byElement)
    {
        switch(element)
        {
            case LINE:
                m = 2;
                break;
            case TRIANGLE:
                m = 3;
                break;
            case RECTANGLE:
                m = 4;
                break;
            default:
                break;
        }
    }
    points.resize(m*n);
    return this;
}

ElementPtr Element::create(Elements element)
{
    ElementPtr el = new Element(element);
    elements.push_back(el);
    return el;
}

ElementPtr Element::translate(float x, float y, float z)
{
    position[0] += x;
    position[1] += y;
    position[2] += z;
    return this;
}

ElementPtr Element::moveTo(float x, float y, float z)
{
    position[0] = x;
    position[1] = y;
    position[2] = z;
    return this;
}

ElementPtr Element::rotateX(float x)
{
    return rotate(x, 0, 0);
}

ElementPtr Element::rotateY(float y)
{
    return rotate(0, y, 0);
}

ElementPtr Element::rotateZ(float z)
{
    return rotate(0, 0, z);
}

ElementPtr Element::rotateToX(float x)
{
    return rotateTo(x, rotation[1], rotation[2]);
}

ElementPtr Element::rotateToY(float y)
{
    return rotateTo(rotation[0], y, rotation[2]);
}

ElementPtr Element::rotateToZ(float z)
{
    return rotateTo(rotation[0], rotation[1], z);
}

ElementPtr Element::rotate(float x, float y, float z)
{
    rotation[0] += x;
    rotation[1] += y;
    rotation[2] += z;
    return this;
}

ElementPtr Element::rotateTo(float x, float y, float z)
{
    rotation[0] = x;
    rotation[1] = y;
    rotation[2] = z;
    return this;
}

ElementPtr Element::scale(float x, float y, float z)
{
    if(y == FLT_MIN) y = x;
    if(z == FLT_MIN) z = y;
    scales[0] *= x;
    scales[1] *= y;
    scales[2] *= z;
    return this;
}

ElementPtr Element::scaleTo(float x, float y, float z)
{
    if(y == FLT_MIN) y = x;
    if(z == FLT_MIN) z = y;
    scales[0] = x;
    scales[1] = y;
    scales[2] = z;
    return this;
}

ElementPtr Element::rgb(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    color[0] = r/255.0f;
    color[1] = g/255.0f;
    color[2] = b/255.0f;
    color[3] = a/255.0f;
    for(int i = 0; i < points.size(); i++)
        points[i].rgb(r, g, b, a);
    for(int i = 0; i < elements.size(); i++)
        elements[i]->rgb(r, g, b, a);
    return this;
}

ElementPtr Element::rgb(float r, float g, float b, float a)
{
    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = a;
    for(int i = 0; i < points.size(); i++)
        points[i].rgb(r, g, b, a);
    for(int i = 0; i < elements.size(); i++)
        elements[i]->rgb(r, g, b, a);
    return this;
}

ElementPtr Element::setWireframe(bool wireframe)
{
    this->wireframe = wireframe;
    for(int i = 0; i < elements.size(); i++)
        elements[i]->setWireframe(wireframe);
    return this;
}

ElementPtr Element::point(float x, float y, int index)
{
    if(points.size() > index)
    {
        points[index].xyz[0] = x;
        points[index].xyz[1] = y;
    }
    return this;
}

ElementPtr Element::point(float x, float y, float z, int index)
{
    if(points.size() > index)
    {
        points[index].xyz[0] = x;
        points[index].xyz[1] = y;
        points[index].xyz[2] = z;
    }
    return this;
}

ElementPtr Element::line(float x1, float y1, float x2, float y2, int index)
{
    index *= 2;
    if(points.size() > index + 1)
    {
        points[index].xyz[0] = x1;
        points[index].xyz[1] = y1;
        points[index+1].xyz[0] = x2;
        points[index+1].xyz[1] = y2;
    }
    return this;
}

ElementPtr Element::line(float x1, float y1, float z1, float x2, float y2, float z2, int index)
{
    index *= 2;
    if(points.size() > index + 1)
    {
        points[index].xyz[0] = x1;
        points[index].xyz[1] = y1;
        points[index].xyz[2] = z1;
        points[index+1].xyz[0] = x2;
        points[index+1].xyz[1] = y2;
        points[index+1].xyz[2] = z2;
    }
    return this;
}

ElementPtr Element::triangle(float x1, float y1, float x2, float y2, float x3, float y3, int index)
{
    index *= 3;
    if(points.size() > index + 2)
    {
        points[index].xyz[0] = x1;
        points[index].xyz[1] = y1;
        points[index+1].xyz[0] = x2;
        points[index+1].xyz[1] = y2;
        points[index+2].xyz[0] = x3;
        points[index+2].xyz[1] = y3;
    }
    return this;
}

ElementPtr Element::triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int index)
{
    index *= 3;
    if(points.size() > index + 2)
    {
        points[index].xyz[0] = x1;
        points[index].xyz[1] = y1;
        points[index].xyz[2] = z1;
        points[index+1].xyz[0] = x2;
        points[index+1].xyz[1] = y2;
        points[index+1].xyz[2] = z2;
        points[index+2].xyz[0] = x3;
        points[index+2].xyz[1] = y3;
        points[index+2].xyz[2] = z3;
    }
    return this;
}

ElementPtr Element::rectangle(float x1, float y1, float x2, float y2, int index)
{
    index *= 4;
    if(points.size() > index + 3)
    {
        points[index].xyz[0] = x1;
        points[index].xyz[1] = y1;
        points[index+1].xyz[0] = x1;
        points[index+1].xyz[1] = y2;
        points[index+2].xyz[0] = x2;
        points[index+2].xyz[1] = y2;
        points[index+3].xyz[0] = x2;
        points[index+3].xyz[1] = y1;
    }
    return this;
}

ElementPtr Element::rectangle(float x1, float y1, float z1, float x2, float y2, float z2, int index)
{
    index *= 4;
    if(points.size() > index + 3)
    {
        points[index].xyz[0] = x1;
        points[index].xyz[1] = y1;
        points[index].xyz[2] = z1;
        points[index+1].xyz[0] = x1;
        points[index+1].xyz[1] = y2;
        points[index+1].xyz[2] = (z1+z2)/2.0f;
        points[index+2].xyz[0] = x2;
        points[index+2].xyz[1] = y2;
        points[index+2].xyz[2] = z2;
        points[index+3].xyz[0] = x2;
        points[index+3].xyz[1] = y1;
        points[index+3].xyz[2] = (z1+z2)/2.0f;
    }
    return this;
}

ElementPtr Element::circle(float x, float y, float r)
{
    float step = 2 * M_PI / (points.size() - 1);
    for(int i = 0; i < points.size(); i++)
    {
        points[i].xyz[0] = r * cos(step * i);
        points[i].xyz[1] = r * sin(step * i);
    }
    return this;
}

ElementPtr Element::circle(float x, float y, float z, float r)
{
    float step = 2 * M_PI / (points.size() - 1);
    for(int i = 0; i < points.size(); i++)
    {
        points[i].xyz[0] = r * cos(step * i);
        points[i].xyz[1] = r * sin(step * i);
        points[i].xyz[2] = z;
    }
    return this;
}

ElementPtr Element::glow()
{
    float step = 360.0f / points.size();
    for(int i = 0; i < points.size(); i++)
    {
        hsvTorgb(step*i, 1.0f, 1.0f, points[i].color);
    }
    return this;
}

void Element::hsvTorgb(float h, float s, float v, float *rgb)
{
    float c = v*s;
    float h_ = h/60.0f;
    float x = c*(1 - abs( fmod(h_, 2) - 1));
    float r1, g1, b1;
    if(h_ >= 0 && h_ <= 1)
        r1 = c, g1 = x, b1 = 0;
    else if(h_ > 1 && h_ <= 2)
        r1 = x, g1 = c, b1 = 0;
    else if(h_ > 2 && h_ <= 3)
        r1 = 0, g1 = c, b1 = x;
    else if(h_ > 3 && h_ <= 4)
        r1 = 0, g1 = x, b1 = c;
    else if(h_ > 4 && h_ <= 5)
        r1 = x, g1 = 0, b1 = c;
    else if(h_ > 5 && h_ <= 6)
        r1 = c, g1 = 0, b1 = x;
    else
        r1 = 0, g1 = 0, b1 = 0;
    float m = v - c;
    rgb[0] = r1 + m;
    rgb[1] = g1 + m;
    rgb[2] = b1 + m;
    rgb[3] = 1.0f;
}

void Element::draw()
{
    if(!visible)
        return;
    if(assoc != NULL)
    {
        glPushMatrix();
        assoc->draw();
        glPopMatrix();
    }
    int n = 0;
    bool withBegin = true;
    GLenum t = GL_POINTS;
    switch(element)
    {
        case POINT:
            n = points.size();
            t = GL_POINTS;
            glPointSize(scales[0]);
            break;
        case LINE:
            n = points.size();
            t = GL_LINES;
            break;
        case TRIANGLE:
            n = points.size();
            if(wireframe)
                t = GL_LINE_LOOP;
            else
                t = GL_TRIANGLES;
            break;
        case RECTANGLE:
            n = 4;
            if(wireframe)
                t = GL_LINE_LOOP;
            else
                t = GL_QUADS;
            break;
        case POLYGON:
            n = points.size();
            if(wireframe)
                t = GL_LINE_LOOP;
            else
                t = GL_POLYGON;
            break;
        case SEQUENCE:
            n = points.size();
            t = GL_LINE_STRIP;
            break;
        case CIRCLE:
            n = points.size();
            if(wireframe)
                t = GL_LINE_LOOP;
            else
                t = GL_POLYGON;
            break;
        case SPHERE:
        case CONE:
        case CUBE:
        case CYLINDER:
        case TEAPOT:
            withBegin = false;
            break;
        case CONTAINER:
            withBegin = false;
            break;
        default:
            break;
    }



    glLineWidth(lineWidth);
    glTranslatef(position[0], position[1], position[2]);
    glScalef(scales[0], scales[1], scales[2]);
    glRotatef(57.2958f * rotation[0], 1.0f, 0.0f, 0.0f);
    glRotatef(57.2958f * rotation[1], 0.0f, 1.0f, 0.0f);
    glRotatef(57.2958f * rotation[2], 0.0f, 0.0f, 1.0f);
    if(withBegin)
    {
        glBegin(t);
        for(int i = 0; i < n; i++)
        {
            glColor4fv(points[i].color);
            glVertex3f(points[i].xyz[0], points[i].xyz[1], points[i].xyz[2]);
        }
        glEnd();
    }
    else
    {
        glColor4fv(color);
        switch(element)
        {
            case SPHERE:
                if(wireframe)
                    glutWireSphere(1.0f, 20, 20);
                else
                    glutSolidSphere(1.0f, 20, 20);
                break;
            case CONE:
                if(wireframe)
                    glutWireCone(0.5f, 1.0f, 20, 20);
                else
                    glutSolidCone(0.5f, 1.0f, 20, 20);
                break;
            case CUBE:
                if(wireframe)
                    glutWireCube(1.0f);
                else
                    glutSolidCube(1.0f);
                break;
            case CYLINDER:
                if(wireframe)
                    glutWireCylinder(0.5f, 1.0f, 20, 20);
                else
                    glutSolidCylinder(0.5f, 1.0f, 20, 20);
                break;
            case TEAPOT:
                if(wireframe)
                    glutWireTeapot(1.0f);
                else
                    glutSolidTeapot(1.0f);
                break;
            default:
                break;
        }   
    }
    glColor4fv(color);
    for(int i = 0; i < elements.size(); i++)
    {
        if(dynamic_cast<Plot*>(this) != NULL && dynamic_cast<Serie*>(elements[i]) != NULL)
        {
            Plot *plt = (Plot*) this;

            Serie* serie = (Serie*)elements[i];
            if(!serie->aligned)
            {
                if(plt->fixedAxis)
                {
                    serie->minX = plt->axisMinX;
                    serie->maxX = plt->axisMaxX;
                    serie->minY = plt->axisMinY;
                    serie->maxY = plt->axisMaxY;
                }
                serie->align(plt->minX, plt->maxX, plt->minY, plt->maxY, serie->color);
                serie->aligned = true;
            }
        }
        glPushMatrix();
        elements[i]->draw();
        glPopMatrix();
    }
}