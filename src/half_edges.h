#pragma once
#ifndef HALF_EDGES_H
#define HALF_EDGES_H

#include <la.h>
#include <map>
#include <drawable.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

//Pre-announce
class Vertex;
class HalfEdge;
class Face;
class Mesh;
//

class Vertex
{
public:
    int id;//ID of this vertex
    glm::vec3 pos;//position of this vertex;
    HalfEdge* edge;//half edge points to this vertex;

    Vertex(int i,glm::vec3 pos):id(i),pos(pos){}
};

class HalfEdge
{
public:
    int id;//ID of this half edge
    Face* face;//face on its left
    Vertex* vert;//the vertex this half edge points to
    HalfEdge* next;//the next half edge
    HalfEdge* sym;//the half edge shares the same line with it

    HalfEdge(int i):id(i){}
};

class Face
{
public:
    int id;//ID of this face
    HalfEdge* start;//the half edge this face starts at;
    glm::vec4 color;//color of this face;

    Face(int i,glm::vec4 color):id(i),color(color){}
};

//Use to store all the vertex, halfedges and faces
class Mesh:
        public Drawable
{
public:
    int id;//ID of this mesh

    void Create();
    Mesh(GLWidget277 *context);
    virtual void create();

    std::map<unsigned int, Vertex*> idList;
    std::map<unsigned int, glm::vec4> norList ;
    std::map<unsigned int, glm::vec4> color_List;
    std::map<unsigned int, HalfEdge*> edgeList;
    std::map<unsigned int, Face*> faceList;

    std::vector<Face*> faces;
    std::vector<Vertex*> vertices;
    std::vector<HalfEdge* > halfedges;

    void createMeshVertexPositions(std::vector<glm::vec4> &mesh_vert_pos);
    void createMeshVertexNormals(std::vector<glm::vec4> &mesh_vert_nor);
    void createMeshVertexColors(std::vector<glm::vec4> &mesh_vert_col);
    void createMeshIndices(std::vector<GLuint> &mesh_idx);
    void recalculateNormals();

    //HW6
    Vertex *addVertex(int ID);     //add a vertex in the middle of an edge, input edge ID
    void triangulate(int ID);   //triangulate a face, input face ID
    void subdivision();         //sub divide a mesh
    void quadranglate(Face *face, Vertex *cendroid, std::vector<int> midpoints);//Quadangulate face
};

//Construct Half Edge Structure
class ConnectMesh : public Mesh
{
public:
    ConnectMesh(GLWidget277 *context);

    void update_mesh();         //Update mesh after edits

    void connect(std::vector<glm::vec3> obj_vertexs,
                 std::vector<glm::vec3> obj_norms,
                 std::vector<glm::vec3> obj_uvs,
                 std::vector<int> obj_face_index,
                 std::map<int, int> obj_index_vertexs);

    virtual void create();
    std::map<std::pair<unsigned int, unsigned int>, HalfEdge*> triangle;

};



#endif // HALF_EDGES_H
