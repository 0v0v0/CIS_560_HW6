#pragma once
#ifndef DRAW_CUBE_H
#define DRAW_CUBE_H

#include <glm/vec3.hpp>
#include <half_edges.h>
#include <iostream>


class cube: public Mesh
{
public:
    cube(GLWidget277 *context);

    void initDataStruct();
    virtual void create();
    void genClrList();
};


#endif // DRAW_CUBE_H
