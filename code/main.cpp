#include "framework.h"
#include "meshLoader.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

using namespace glm;
float rotateY = 0.0;

//-----------------------------------------------------------------------------
void MyGlDraw(void)
{
    std::vector<vec4> model = loadMesh("susy.obj");

    mat4 identity_Matrix = mat4(vec4(1, 0, 0, 0),
                                vec4(0, 1, 0, 0),
                                vec4(0, 0, 1, 0),
                                vec4(0, 0, 0, 1));

    mat4 rotate_Matrix = mat4(vec4(cos(rotateY), 0, -sin(rotateY), 0),
                              vec4(0, 1, 0, 0),
                              vec4(sin(rotateY), 0, cos(rotateY), 0),
                              vec4(0, 0, 0, 1));

    rotateY += 0.01;

    mat4 model_Matrix = identity_Matrix * rotate_Matrix;

    vec3 camera_pos    =   vec3(0, 0, 5);  //Camera's position
    vec3 camera_target =   vec3(0, 0, 0);  //Camera's look-at point
    vec3 camera_up     =   vec3(0, 1, 0);  //Camera's up-vector

    vec3 camera_z = -normalize(camera_pos - camera_target);
    vec3 camera_x = normalize(cross(camera_up, camera_z));
    vec3 camera_y = normalize(cross(camera_z, camera_x));

    mat4 B = mat4(vec4(camera_x, 0),
                  vec4(camera_y, 0),
                  vec4(camera_z, 0),
                  vec4(0, 0, 0, 1));

    mat4 T = mat4(vec4(1, 0, 0, -camera_pos.x),
                  vec4(0, 1, 0, -camera_pos.y),
                  vec4(0, 0, 1, -camera_pos.z),
                  vec4(0, 0, 0, 1));

    mat4 view_Matrix = transpose(B) * T;

    float d = 0.5f;

    mat4 projection_Matrix = mat4(vec4(1, 0, 0, 0),
                                  vec4(0, 1, 0, 0),
                                  vec4(0, 0, 1, -1/d),
                                  vec4(0, 0, d, 0));


    mat4 MVPmatrix = model_Matrix * view_Matrix * projection_Matrix;

    for(unsigned int i = 0; i < model.size(); i++)
    {
        model[i] = model[i] * MVPmatrix;
    }

    for(unsigned int i = 0; i < model.size(); i++)
    {
        model[i] = model[i] / model[i].w;
    }

    int w = 512;
    int h = 512;

    mat4 S1 = mat4(vec4(1, 0, 0, 0),
                   vec4(0, -1, 0, 0),
                   vec4(0, 0, 1, 0),
                   vec4(0, 0, 0, 1));

    mat4 T1 = mat4(vec4(w/2, 0, 0, 0),
                   vec4(0, h/2, 0, 0),
                   vec4(0, 0, 1, 0),
                   vec4(1, 1, 0, 1));

    mat4 S2 = mat4(vec4(1, 0, 0, 0),
                   vec4(0, 1, 0, 0),
                   vec4(0, 0, 1, 0),
                   vec4((w-1)/2, (h-1)/2, 0, 1));

    mat4 viewport_Matrix = S2 * T1 * S1;

    for(unsigned int i = 0; i < model.size(); i++)
    {
        model[i] = round(viewport_Matrix * model[i]);
    }


    Pixel p1, p2, p3;

    p1.R = 255;
    p1.G = 255;
    p1.B = 255;
    p1.A = 0;

    p2 = p1;
    p3 = p1;

    memset(FBptr, 0, w*h*4);

    for(unsigned int i = 0; i < model.size(); i+=3)
    {
        p1.x = model[i][0];
        p1.y = model[i][1];

        p2.x = model[i+1][0];
        p2.y = model[i+1][1];

        p3.x = model[i+2][0];
        p3.y = model[i+2][1];

        DrawTriangle(p1, p2, p3);
    }
}

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
    // Inicializações.
    InitOpenGL(&argc, argv);
    InitCallBacks();
    InitDataStructures();

    // Ajusta a função que chama as funções do mygl.h
    DrawFunc = MyGlDraw;

    // Framebuffer scan loop.
    glutMainLoop();

    return 0;
}

