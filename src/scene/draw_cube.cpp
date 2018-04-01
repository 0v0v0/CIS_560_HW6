#include <scene/draw_cube.h>

cube::cube(GLWidget277 *context) : Mesh(context)
{
    initDataStruct();
}

void cube::create()
{
    this->Mesh::create();
}

void cube::initDataStruct()
{
    // Init vertices

    /*
    for(int i=-1;i<2;i+=2)
    {
        for(int j=-1;j<2;j+=2)
        {
            for(int k=-1;k<2;k+=2)
            {
                   tmp.id=count;
                   tmp.pos=glm::vec3(i,j,k);
                   count++;
                   vertices.push_back(tmp);
            }
        }
    }
    */

    Vertex *v0 = new Vertex(0, glm::vec3(-0.5, -0.5, -0.5));
    Vertex *v1 = new Vertex(1, glm::vec3(0.5, -0.5, -0.5));
    Vertex *v2 = new Vertex(2, glm::vec3(0.5, -0.5, 0.5));
    Vertex *v3 = new Vertex(3,glm::vec3(-0.5, -0.5, 0.5));

    Vertex *v4 = new Vertex(4, glm::vec3(-0.5, 0.5, 0.5));
    Vertex *v5 = new Vertex(5, glm::vec3(-0.5, 0.5, -0.5));
    Vertex *v6 = new Vertex(6, glm::vec3(0.5, 0.5, -0.5));
    Vertex *v7 = new Vertex(7, glm::vec3(0.5, 0.5, 0.5));

    vertices = {v0,v1,v2,v3,v4,v5,v6,v7};


    // Init half edges
    for(unsigned int i = 0; i < 24; i++) {
        HalfEdge* tmp;
        tmp=new HalfEdge(i);
        halfedges.push_back(tmp);
    }

    // Init faces
    for(unsigned int i = 0; i < 6; i++) {
        Face* tmp;
        tmp=new Face(i,glm::vec4(255));
        faces.push_back(tmp);
    }

    vertices[0]->edge = halfedges[2];
    vertices[1]->edge = halfedges[3];
    vertices[2]->edge = halfedges[0];
    vertices[3]->edge = halfedges[1];
    vertices[4]->edge = halfedges[22];
    vertices[5]->edge = halfedges[21];
    vertices[6]->edge = halfedges[20];
    vertices[7]->edge = halfedges[23];

    faces[0]->start = halfedges[0]; faces[0]->color = glm::vec4(glm::vec3(1,1,0),1);
    faces[1]->start = halfedges[4]; faces[1]->color = glm::vec4(glm::vec3(0,1,1),1);
    faces[2]->start = halfedges[8]; faces[2]->color = glm::vec4(glm::vec3(1,0,1),1);
    faces[3]->start = halfedges[12]; faces[3]->color = glm::vec4(glm::vec3(1,0,0),1);
    faces[4]->start = halfedges[16]; faces[4]->color = glm::vec4(glm::vec3(0,1,0),1);
    faces[5]->start = halfedges[20]; faces[5]->color = glm::vec4(glm::vec3(0,0,1),1);

    halfedges[0]->face = faces[0]; halfedges[0]->sym = halfedges[18]; halfedges[0]->next = halfedges[3]; halfedges[0]->vert = vertices[2];
    halfedges[1]->face = faces[0]; halfedges[1]->sym = halfedges[15]; halfedges[1]->next = halfedges[0]; halfedges[1]->vert = vertices[3];
    halfedges[2]->face = faces[0]; halfedges[2]->sym = halfedges[  4]; halfedges[2]->next = halfedges[1]; halfedges[2]->vert = vertices[0];
    halfedges[3]->face = faces[0]; halfedges[3]->sym = halfedges[  9]; halfedges[3]->next = halfedges[2]; halfedges[3]->vert = vertices[1];

    halfedges[4]->face = faces[1]; halfedges[4]->sym = halfedges[  2]; halfedges[4]->next = halfedges[7]; halfedges[4]->vert = vertices[1];
    halfedges[5]->face = faces[1]; halfedges[5]->sym = halfedges[14]; halfedges[5]->next = halfedges[4]; halfedges[5]->vert = vertices[0];
    halfedges[6]->face = faces[1]; halfedges[6]->sym = halfedges[20]; halfedges[6]->next = halfedges[5]; halfedges[6]->vert = vertices[5];
    halfedges[7]->face = faces[1]; halfedges[7]->sym = halfedges[10]; halfedges[7]->next = halfedges[6]; halfedges[7]->vert = vertices[6];

    halfedges[8]->face = faces[2]; halfedges[8]->sym = halfedges[19]; halfedges[8]->next = halfedges[11]; halfedges[8]->vert = vertices[7];
    halfedges[9]->face = faces[2]; halfedges[9]->sym = halfedges[3];    halfedges[9]->next = halfedges[8]; halfedges[9]->vert = vertices[2];
    halfedges[10]->face = faces[2]; halfedges[10]->sym = halfedges[7]; halfedges[10]->next = halfedges[9]; halfedges[10]->vert = vertices[1];
    halfedges[11]->face = faces[2]; halfedges[11]->sym = halfedges[23]; halfedges[11]->next = halfedges[10]; halfedges[11]->vert = vertices[6];

    halfedges[12]->face = faces[3]; halfedges[12]->sym = halfedges[17]; halfedges[12]->next = halfedges[15]; halfedges[12]->vert = vertices[3];
    halfedges[13]->face = faces[3]; halfedges[13]->sym = halfedges[21]; halfedges[13]->next = halfedges[12]; halfedges[13]->vert = vertices[4];
    halfedges[14]->face = faces[3]; halfedges[14]->sym = halfedges[5]; halfedges[14]->next = halfedges[13]; halfedges[14]->vert = vertices[5];
    halfedges[15]->face = faces[3]; halfedges[15]->sym = halfedges[1]; halfedges[15]->next = halfedges[14]; halfedges[15]->vert = vertices[0];

    halfedges[16]->face = faces[4]; halfedges[16]->sym = halfedges[22]; halfedges[16]->next = halfedges[19]; halfedges[16]->vert = vertices[7];
    halfedges[17]->face = faces[4]; halfedges[17]->sym = halfedges[12]; halfedges[17]->next = halfedges[16]; halfedges[17]->vert = vertices[4];
    halfedges[18]->face = faces[4]; halfedges[18]->sym = halfedges[0]; halfedges[18]->next = halfedges[17]; halfedges[18]->vert = vertices[3];
    halfedges[19]->face = faces[4]; halfedges[19]->sym = halfedges[8]; halfedges[19]->next = halfedges[18]; halfedges[19]->vert = vertices[2];

    halfedges[20]->face = faces[5]; halfedges[20]->sym = halfedges[6]; halfedges[20]->next = halfedges[23]; halfedges[20]->vert = vertices[6];
    halfedges[21]->face = faces[5]; halfedges[21]->sym = halfedges[13]; halfedges[21]->next = halfedges[20]; halfedges[21]->vert = vertices[5];
    halfedges[22]->face = faces[5]; halfedges[22]->sym = halfedges[16]; halfedges[22]->next = halfedges[21]; halfedges[22]->vert = vertices[4];
    halfedges[23]->face = faces[5]; halfedges[23]->sym = halfedges[11]; halfedges[23]->next = halfedges[22]; halfedges[23]->vert = vertices[7];

    for(unsigned int i = 0; i < 24; i++)
    {
        edgeList[i] = halfedges[i];
    }

    for(unsigned int i = 0; i < 24; i++)
    {
        faceList[i] = faces[i];
    }

    //
    std::cout<<"\ncube ready! \n";
}






