#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <map>

//Global buffers
//
std::vector<glm::vec3> obj_vertexs;
std::vector<glm::vec3> obj_norms;
std::vector<glm::vec3> obj_uvs;
std::map<int, int> obj_index_vertexs;
std::vector<int> obj_face_index;

//
//function declarations
//
void load_obj();
//


MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent),
      geom_cylinder(this), geom_sphere(this),
      prog_lambert(this), prog_flat(this),
      gl_camera(),
      geom_cube(this),//add cube here
      loaded_obj(this)//add loaded obj here
{
    // Connect the timer to a function so that when the timer ticks the function is executed
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    // Tell the timer to redraw 60 times per second
    timer.start(16);
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    geom_cylinder.destroy();
    geom_sphere.destroy();
    geom_cube.destroy();
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the instances of Cylinder and Sphere.
    //geom_cylinder.create();

    //geom_sphere.create();

    //geom_cube.initDataStruct();
    geom_cube.create();

    // Create and set up the diffuse shader
    prog_lambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    prog_flat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    // Set a color with which to draw geometry since you won't have one
    // defined until you implement the Node classes.
    // This makes your geometry render green.
    prog_lambert.setGeometryColor(glm::vec4(0,1,0,1));
    prog_flat.setGeometryColor(glm::vec4(0,1,0,1));
    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
//    vao.bind();
    glBindVertexArray(vao);


    //Load Obj
    load_obj();
    //Connect the loaded mesh
    loaded_obj.connect(obj_vertexs,
                       obj_norms,
                       obj_uvs,
                       obj_face_index,
                       obj_index_vertexs);

    loaded_obj.create();

}

void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    gl_camera = Camera(w, h);
    glm::mat4 viewproj = gl_camera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    prog_lambert.setViewProjMatrix(viewproj);
    prog_flat.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function updateGL is called, paintGL is called implicitly.
//DO NOT CONSTRUCT YOUR SCENE GRAPH IN THIS FUNCTION!
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    prog_flat.setViewProjMatrix(gl_camera.getViewProj());
    prog_lambert.setViewProjMatrix(gl_camera.getViewProj());

#define NOPE
#ifdef NOPE
    //Create a model matrix. This one scales the sphere uniformly by 3, then translates it by <-2,0,0>.
    //Note that we have to transpose the model matrix before passing it to the shader
    //This is because OpenGL expects column-major matrices, but you've
    //implemented row-major matrices.
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2,0,0)) * glm::scale(glm::mat4(1.0f), glm::vec3(3,3,3));
    //Send the geometry's transformation matrix to the shader
    prog_lambert.setModelMatrix(model);
    prog_flat.setModelMatrix(model);
    //Draw the example sphere using our lambert shader


    //Enable this to draw the cube!
    //prog_flat.draw(geom_cube);

    //draw the loaded mesh
    prog_flat.draw(loaded_obj);

    //Now do the same to render the cylinder
    //We've rotated it -45 degrees on the Z axis, then translated it to the point <2,2,0>
    //model = glm::translate(glm::mat4(1.0f), glm::vec3(2,2,0)) * glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0,0,1));
    //prog_lambert.setModelMatrix(model);
    //prog_lambert.draw(geom_cylinder);
#endif
}


void MyGL::keyPressEvent(QKeyEvent *e)
{

    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    // This could all be much more efficient if a switch
    // statement were used, but I really dislike their
    // syntax so I chose to be lazy and use a long
    // chain of if statements instead
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        gl_camera.RotateAboutUp(-amount);
    } else if (e->key() == Qt::Key_Left) {
        gl_camera.RotateAboutUp(amount);
    } else if (e->key() == Qt::Key_Up) {
        gl_camera.RotateAboutRight(-amount);
    } else if (e->key() == Qt::Key_Down) {
        gl_camera.RotateAboutRight(amount);
    } else if (e->key() == Qt::Key_1) {
        gl_camera.fovy += amount;
    } else if (e->key() == Qt::Key_2) {
        gl_camera.fovy -= amount;
    } else if (e->key() == Qt::Key_W) {
        gl_camera.TranslateAlongLook(amount);
    } else if (e->key() == Qt::Key_S) {
        gl_camera.TranslateAlongLook(-amount);
    } else if (e->key() == Qt::Key_D) {
        gl_camera.TranslateAlongRight(amount);
    } else if (e->key() == Qt::Key_A) {
        gl_camera.TranslateAlongRight(-amount);
    } else if (e->key() == Qt::Key_Q) {
        gl_camera.TranslateAlongUp(-amount);
    } else if (e->key() == Qt::Key_E) {
        gl_camera.TranslateAlongUp(amount);
    } else if (e->key() == Qt::Key_R) {
        gl_camera = Camera(this->width(), this->height());
    }
    gl_camera.RecomputeAttributes();
    update();  // Calls paintGL, among other things
}

void MyGL::timerUpdate()
{
    // This function is called roughly 60 times per second.
    // Use it to perform any repeated actions you want to do,
    // such as

    //std::cout << "AAA" << std::endl;
}


//I edited here!!!
void load_obj()
{
    FILE* fp1,fp2;
    //open a text file
    //ALL obj must be put inside the builded folder!!!!!

    if(fopen_s(&fp1,"cube.obj","r") != 0)
    {
        printf("\nN ERROR! FUCK!\n");
    }

    char aaa[100];//string buffer

    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;

    while( fscanf(fp1, "%s", aaa)!=EOF )//If not end...
    {
        if ( strcmp( aaa, "v" ) == 0 )
        {
            glm::vec3 vertex;
            fscanf(fp1, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertices.push_back(vertex);

        }
        else if ( strcmp( aaa, "vt" ) == 0 )
        {
            glm::vec2 uv;
            fscanf(fp1, "%f %f\n", &uv.x, &uv.y );
            temp_uvs.push_back(uv);

        }
        else if ( strcmp( aaa, "vn" ) == 0 )
        {
            glm::vec3 normal;
            fscanf(fp1, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            temp_normals.push_back(normal);

        }
        else if ( strcmp( aaa, "f" ) == 0 )
        {
            int nv = 0;
            while(1)
            {
                std::vector<unsigned int> temp_indices;
                unsigned int vertexIndex, uvIndex, normalIndex;
                int matches = fscanf(fp1, "%d/%d/%d\n", &vertexIndex, &uvIndex, &normalIndex);

                if (matches != 3)
                {
                    obj_face_index.push_back(nv);
                    break;
                }

                vertexIndices.push_back(vertexIndex);
                uvIndices    .push_back(uvIndex);
                normalIndices.push_back(normalIndex);
                nv++;
            }
        }
    }

    for(int i=0; i<vertexIndices.size(); i++ )
    {
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
        obj_index_vertexs[i] = vertexIndex - 1;
    }


    obj_norms = std::vector<glm::vec3>(temp_normals);
    obj_vertexs = std::vector<glm::vec3>(temp_vertices);

    //Close & Destroy
    vertexIndices.clear();
    uvIndices.clear();
    normalIndices.clear();
    temp_normals.clear();
    temp_uvs.clear();
    temp_vertices.clear();

    fclose(fp1);
}
