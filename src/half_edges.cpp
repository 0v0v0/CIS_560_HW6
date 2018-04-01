#include <half_edges.h>

Mesh::Mesh(GLWidget277 *context) : Drawable(context) {}

void Mesh::createMeshVertexPositions(std::vector<glm::vec4> &mesh_vert_pos)
{
    for(int i = 0; i < idList.size(); i++)
    {
        mesh_vert_pos.push_back(glm::vec4(idList[i]->pos,1));
    }
}

void Mesh::createMeshVertexNormals(std::vector<glm::vec4> &mesh_vert_nor)
{
    int count = 0;
    for(Face* f : faces)
    {
        HalfEdge *half = f->start;
        int temp = 0;

        while(1)
        {
            glm::vec3 v0 = half->vert->pos;
            glm::vec3 v1 = half->next->vert->pos;
            glm::vec3 v2 = half->next->next->vert->pos;
            glm::vec4 n = glm::vec4(glm::vec3(-1) * glm::normalize(glm::cross(v1-v0, v1-v2)), 1);
            mesh_vert_nor.push_back(n);
            temp++;
            half = half->next;
            if(half == f->start)
            {
                count += temp; break;
            }
        }
    }
}

void Mesh::createMeshVertexColors(std::vector<glm::vec4> &mesh_vert_col)
{
    for(int i = 0; i < color_List.size(); i++)
    {
        mesh_vert_col.push_back(color_List[i]);
    }
}

void Mesh::createMeshIndices(std::vector<GLuint> &mesh_idx)
{
    int count = 0;
    for(int i=0;i<faces.size();i++)
    {
        Face* f=faces.at(i);

        std::vector<int> tris;
        HalfEdge *half = f->start;
        int tmp = 0;

        while(1)
        {
            idList[tmp + count] = half->vert;
            color_List[tmp + count] = f->color;
            tris.push_back(tmp++);
            if(tris.size() == 3)
            {
                mesh_idx.push_back(tris[0] + count);
                mesh_idx.push_back(tris[1] + count);
                mesh_idx.push_back(tris[2] + count);
                tris.erase(tris.begin() + 1, tris.begin() + 2);
            }
            half = half->next;
            if(half == f->start)
            {
                count += tmp; break;
            }
        }
    }

}

void Mesh::create()
{
    std::vector<GLuint> mesh_idx;
    std::vector<glm::vec4> mesh_vert_pos;
    std::vector<glm::vec4> mesh_vert_nor;
    std::vector<glm::vec4> mesh_vert_col;

    createMeshIndices(mesh_idx);
    createMeshVertexPositions(mesh_vert_pos);
    createMeshVertexNormals(mesh_vert_nor);
    createMeshVertexColors(mesh_vert_col);

    count = mesh_idx.size();

    generateIdx();
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_idx.size() * sizeof(GLuint), mesh_idx.data(), GL_STATIC_DRAW);
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, mesh_vert_pos.size() * sizeof(glm::vec4), mesh_vert_pos.data(), GL_STATIC_DRAW);
    generateNor();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    context->glBufferData(GL_ARRAY_BUFFER, mesh_vert_nor.size() * sizeof(glm::vec4), mesh_vert_nor.data(), GL_STATIC_DRAW);
    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, mesh_vert_col.size() * sizeof(glm::vec4), mesh_vert_col.data(), GL_STATIC_DRAW);

    std::cout<<"mesh ready!";
}

//////
//Connect HalfEdges
///
ConnectMesh::ConnectMesh(GLWidget277 *context) : Mesh(context) {}

void ConnectMesh::create()
{
    this->Mesh::create();
}

void ConnectMesh::connect(std::vector<glm::vec3> obj_vertexs,
                          std::vector<glm::vec3> obj_norms,
                          std::vector<glm::vec3> obj_uvs,
                          std::vector<int> obj_face_index,
                          std::map<int, int> obj_index_vertexs)
{
    // Init
    for(unsigned int i = 0; i < obj_vertexs.size(); i++)
    {
        vertices.push_back(new Vertex(i, obj_vertexs[i]));
    }

    for(unsigned int i = 0; i < obj_face_index.size(); i++)
    {
        faces.push_back(new Face(i, glm::vec4(1)));
    }

    for(unsigned int i = 0; i < obj_index_vertexs.size(); i++)
    {
        halfedges.push_back(new HalfEdge(i));
    }

    //Random Color
    srand(time(NULL));

    int count=0;

    for(unsigned int i = 0; i < faces.size(); i++)
    {
        float r =(rand() % 255 + 1.0) / 255.0;
        float g =(rand() % 255 + 1.0) / 255.0;
        float b =(rand() % 255 + 1.0) / 255.0;

        count +=obj_face_index[i];
        faces[i]->start = halfedges[count - 1];
        faces[i]->color = glm::vec4(r,g,b,1);
    }

    //Connect
    int s = 0, e = 0;

    for(unsigned int i = 0; i < faces.size(); i++)
    {
        s = e;
        e += obj_face_index[i];
        int temp_s = s;
        while(temp_s < e)
        {
            halfedges[temp_s]->face = faces[i];
            if(temp_s == e - 1)
            {
                halfedges[temp_s]->next = halfedges[s];
                unsigned int a = obj_index_vertexs[temp_s];
                unsigned int b = obj_index_vertexs[s];
                triangle[std::make_pair(a, b)] = halfedges[s];
            }
            else
            {
                halfedges[temp_s]->next = halfedges[temp_s+1];
                unsigned int a = obj_index_vertexs[temp_s];
                unsigned int b = obj_index_vertexs[temp_s+1];
                triangle[std::make_pair(a, b)] = halfedges[temp_s + 1];
            }
            vertices[obj_index_vertexs[temp_s]]->edge = halfedges[temp_s];
            halfedges[temp_s]->vert = vertices[obj_index_vertexs[temp_s]];
            temp_s++;
        }
    }

    //Get Symmetry Edges
    for(auto & edgeEntry : triangle)
    {
        HalfEdge * fe = edgeEntry.second;
        unsigned int a = edgeEntry.first.first;
        unsigned int b = edgeEntry.first.second;
        if(triangle.find(std::make_pair(b,a)) != triangle.end())
        {
            fe->sym = triangle[std::make_pair(b,a)];
        }
    }

    triangle.clear();

    //Edge
    for(unsigned int i = 0; i < halfedges.size(); i++)
    {
        edgeList[i] = halfedges[i];
    }

    //Face
    for(unsigned int i = 0; i < faces.size(); i++)
    {
        faceList[i] = faces[i];
    }

    //Normals
    for(unsigned int i = 0; i <obj_index_vertexs.size(); i++)
    {
        norList[i] = glm::vec4(obj_norms[i],1);
    }

    std::cout<<"\nConnection Ready!\n";

}


//HW6

Vertex* Mesh::addVertex(int ID)
{

    HalfEdge *heo = halfedges.at(ID);    //original half edge
    HalfEdge *heo_sym = halfedges.at(ID)->sym;   //Symmtry half edge
    Vertex *vertex_after = heo->vert; //original vertex
    Vertex *vertex_before = heo_sym->vert; //vertex before origin

    //Vertex in Middle
    Vertex *mid_vertex = new Vertex(vertices.size(),(vertex_before->pos+vertex_after->pos)*0.5f);
    vertices.push_back(mid_vertex);

    //New HalfEdges
    HalfEdge *he_new = new HalfEdge(halfedges.size());
    halfedges.push_back(he_new);
    //size++
    HalfEdge *he_new_sym = new HalfEdge(halfedges.size());
    halfedges.push_back(he_new_sym);

    //Connect
    //sequence:
    //                    heo     ->  he_new        ->  next
    //                        mid_point
    //  next    <-  heo_new_sym   <-  heo_sym

    he_new->face = heo->face;
    he_new->vert = heo->vert;
    he_new->next = heo->next;
    he_new->sym = heo_sym;

    heo->next = he_new;
    heo->vert = mid_vertex;
    heo->sym = he_new_sym;

    he_new_sym->sym = heo;
    he_new_sym->face = heo_sym->face;
    he_new_sym->vert = heo_sym->vert;
    he_new_sym->next = heo_sym->next;

    heo_sym->next = he_new_sym;
    heo_sym->vert = mid_vertex;
    heo_sym->sym = he_new;

    return mid_vertex;

}

void Mesh::triangulate(int ID)
{
    Face* f1 = faces[id];
    HalfEdge* heo = f1->start;

    if(heo->next->next->next != heo)  //If face is not triangulated
    {
        HalfEdge* he_a = new HalfEdge(halfedges.size());
        halfedges.push_back(he_a);

        HalfEdge* he_b = new HalfEdge(halfedges.size());
        halfedges.push_back(he_b);

        he_a->vert = heo->vert;
        he_b->vert = heo->next->next->vert;
        he_a->sym = he_b;
        he_b->sym = he_a;

        Face* f2 = new Face(faces.size(), f1->color);
        faces.push_back(f2);
        he_a->face=f2;
        heo->face=f2;
        heo->next->next->face=f2;

        he_b->face=f1;

        f2->start=he_a;

        he_b->next = heo->next->next->next;
        heo->next->next->next = he_a;
        he_a->next = heo->next;
        heo->next = he_b;
    }
}


void Mesh::subdivision()
{
    //Compute Centroids
    std::map<int, std::vector<int>> cendroid_map;   //(vertex,near faces)
    std::map<int, int> centroids;                   //(face,centroid)

    for(int i=0;i<faces.size();i++)
    {
        Face* this_face=faces.at(i);    //face pointer
        int num_vert = 0;               //record vertices number in face
        glm::vec3 pos;                  //position
        HalfEdge * HE = this_face->start;   //half edge pointer

        //if not start
        while(HE->next != this_face->start)
        {
            cendroid_map[HE->vert->id].push_back(this_face->id);
            num_vert++;
            pos += HE->vert->pos;
            HE = HE->next;
        }
        //average pos
        pos = pos/(float)num_vert;
        //add cendroid index into face
        centroids[this_face->id] = vertices.size();
        //push cendroid
        vertices.push_back(new Vertex(vertices.size(), pos));
    }


    //Copy Origin HalfEdges
    std::vector<HalfEdge*> halfedges_origin(halfedges);

    //Find Edge Vertices
    std::map<int, std::vector<int>> midpoint_map; //(vertex,near midpoints)

    std::vector<int> midpoints;

    std::map<std::pair<int,int>,int> facePairMap; //(face pair),Midpoint)

    //Edge Midpoints
    for(int i=0;i<halfedges_origin.size();i++)
    {
        HalfEdge* he=halfedges_origin.at(i);
        Face* f0 = he->face;        //the face of this half edge
        Face* f1 = he->sym->face;   //symmtry face of this half edge
        Vertex *v0 = he->vert;      //the vertex it connects to
        Vertex *v1 = he->sym->vert; //the vertex its sym connects to

        if(facePairMap.find(std::make_pair(f1->id, f0->id)) == facePairMap.end() &&
                facePairMap.find(std::make_pair(f0->id, f1->id)) == facePairMap.end() )
        {
            //add midpoint,return the point we added
            Vertex* midpoint = addVertex(he->id);
            //neighbour pos
            glm::vec3 e0v = v0->pos;
            glm::vec3 e1v = v1->pos;
            //neighbour cendroid pos
            glm::vec3 f0v = vertices[centroids[f0->id]]->pos;
            glm::vec3 f1v = vertices[centroids[f1->id]]->pos;
            //add new point to face pair map
            facePairMap[std::make_pair(f0->id, f1->id)] = midpoint->id;
            facePairMap[std::make_pair(f1->id, f0->id)] = midpoint->id;
            //add new point to mid point map
            midpoint_map[v0->id].push_back(midpoint->id);
            midpoint_map[v1->id].push_back(midpoint->id);
            //add new point to midpoints list
            midpoints.push_back(midpoint->id);

            glm::vec3 new_pos;
            //average pos according to 2 neighbours and 2 centroids
            new_pos = (e0v + e1v + f0v + f1v)/4.f;
            // update midpoints position
            midpoint->pos = new_pos;
        }
    }

    //Smooth Original Vertices
    for(int i=0;i<midpoint_map.size();i++)
    {
        glm::vec3 mid_pos;
        glm::vec3 cendroid_pos;
        float n = midpoint_map[i].size();
        for(int j = 0; j < midpoint_map[i].size(); j++)
        {
            int mid_id = midpoint_map[i][j];
            mid_pos += vertices[mid_id]->pos;
            int cendroid_id = centroids[cendroid_map[i][j]];
            cendroid_pos += vertices[cendroid_id]->pos;
        }

        Vertex* origin_vertex = vertices.at(i);

        glm::vec3 tmp=origin_vertex->pos;
        /*
        v' = (n-2)v/n +
        sum(e)/n2 +
        sum(f)/n2
        */
        tmp = origin_vertex->pos*((n-2)/n)+
                mid_pos /(n*n)+
                cendroid_pos/(n*n);
        origin_vertex->pos = tmp;
    }

    //Copy Original Faces
    std::vector<Face*> face_origin(faces);

    //Quadranglate
    for(int i=0;i<face_origin.size();i++)
    {
        //the face
        Face* face=face_origin.at(i);
        //mid points
        std::vector<int> midpoint_list;
        HalfEdge *he = face->start;

        //Loop all edges
        while(he->next != face->start)
        {
            Vertex* vert = he->vert;

            if(std::find(midpoints.begin(), midpoints.end(), vert->id) != midpoints.end())
            {
                midpoint_list.push_back(vert->id);
            }
            he = he->next;
        }
        quadranglate(face, vertices.at(centroids[face->id]), midpoint_list);
    }

    //Update mesh
    idList.clear();
    create();

    //clear all maps and vectors
    halfedges_origin.clear();
    face_origin.clear();
    cendroid_map.clear();
    centroids.clear();
    midpoint_map.clear();
    midpoints.clear();
    facePairMap.clear();
}



void Mesh::quadranglate(Face *f, Vertex *cen, std::vector<int> midpoints)
{
    //New face pointers
    Face* new_face[midpoints.size()];
    //first is the old face
    new_face[0] = f;
    //new faces, start from 1 not 0!
    for(int i = 1; i < midpoints.size(); i++)
    {
        new_face[i] = new Face(faces.size(),glm::vec4(1));
        faces.push_back(new_face[i]);
    }

    //can find sym he by std::find
    std::map<std::pair<unsigned int, unsigned int>, HalfEdge*> he_map;

    //int count = 0;
    HalfEdge *last;

    Vertex *v = f->start->vert;
    HalfEdge *h = f->start->next;
    //loop edges via mid_points
    for(int i = 0; i < midpoints.size(); i++)
    {

        if(std::find(midpoints.begin(), midpoints.end(), v->id) != midpoints.end())
        {
            Face* nf = new_face[i];
            HalfEdge *he0 = new HalfEdge(halfedges.size());
            HalfEdge *he1 = new HalfEdge(halfedges.size() + 1);

            last = he0;
            nf->start = h;
            h->face = nf;

            he0->next = h;
            he0->vert = v;
            he0->face = nf;
            //add to he_map
            he_map[std::make_pair(cen->id, v->id)] = he0;

            HalfEdge *he2 = h->next->next;

            v = h->next->vert;
            h = h->next;
            h->face = nf;
            h->next = he1;
            he1->vert = cen;
            he1->next = he0;
            he1->face = nf;
            //add sym to he_map
            he_map[std::make_pair(v->id, cen->id)] = he1;

            halfedges.push_back(he0);
            halfedges.push_back(he1);

            h = he2;
        }
        else
        {
            //foward
            h = h->next;
            v = h->vert;
        }
    }

    cen->edge = last;

    //find sym edges by changing pairs
    for(auto & edge : he_map)
    {
        HalfEdge * fe = edge.second;
        int a = edge.first.first;
        int b = edge.first.second;

        if(he_map.find(std::make_pair(b,a)) != he_map.end())
        {
            fe->sym = he_map[std::make_pair(b,a)];
        }
    }

    he_map.clear();
}








