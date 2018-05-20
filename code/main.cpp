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
    // Instancia um vetor que carrega o vértice de um dos triângulos
    std::vector<vec4> model = loadMesh("susy.obj");

    // Cria uma matriz identidade 4x4 
    mat4 identity_Matrix = mat4(vec4(1, 0, 0, 0),
                                vec4(0, 1, 0, 0),
                                vec4(0, 0, 1, 0),
                                vec4(0, 0, 0, 1));

    // Cria uma matriz de rotação que rotaciona o objeto em torno do eixo Y, isso
    // é feito para demonstrar a figura mudando dinâmicamente a cada frame numa rotação
    // exibindo, assim, ela em diversos ângulos
    mat4 rotate_Matrix = mat4(vec4(cos(rotateY), 0, -sin(rotateY), 0),
                              vec4(0, 1, 0, 0),
                              vec4(sin(rotateY), 0, cos(rotateY), 0),
                              vec4(0, 0, 0, 1));

    // Define a próxima rotação como 0.01º à mais do que a anterior
    rotateY += 0.01;

    // Faz com que a matriz model seja igual a matriz de rotação
    mat4 model_Matrix = identity_Matrix * rotate_Matrix;

    // Cria três vetores: uma para a posição da camera, outro para onde ela está
    // mirando e outro que representa o UP da cena
    vec3 camera_pos    =   vec3(0, 0, 5);  //Camera's position
    vec3 camera_target =   vec3(0, 0, 0);  //Camera's look-at point
    vec3 camera_up     =   vec3(0, 1, 0);  //Camera's up-vector

    // Aqui é construido os três eixos aonde a camera está posicionada:
    //  O eixo Z é obtido através do vetor unitário contrário ao que vai da 
    //  posição da camera até aonde ela está mirando, a função normalize
    //  transforma vetores em vetores unitários
    vec3 camera_z = -normalize(camera_pos - camera_target);
    //  O eixo x da camera é representado pelo vetor perpendicular ao eixo z e ao UP
    //  (que está atrelado ao eixo Y do universo nesse caso), o produto vetorial
    //  obtém justamente esse vetor perpendicular e utiliza-se o normalize para deixá-lo
    //  unitário 
    vec3 camera_x = normalize(cross(camera_up, camera_z));
    //  Para encontrar o eixo Y basta fazer o produto vetorial de Z e X, encontrando o
    //  vetor perpendicular a esses dois eixos e, em seguida, transformá-lo em unitário
    vec3 camera_y = normalize(cross(camera_z, camera_x));

    // Cria uma matriz que servirã para passar os pontos para o
    // espaço de camera
    mat4 B = mat4(vec4(camera_x, 0),
                  vec4(camera_y, 0),
                  vec4(camera_z, 0),
                  vec4(0, 0, 0, 1));

    // A Matriz T translada a camera para a origem da cena
    mat4 T = mat4(vec4(1, 0, 0, -camera_pos.x),
                  vec4(0, 1, 0, -camera_pos.y),
                  vec4(0, 0, 1, -camera_pos.z),
                  vec4(0, 0, 0, 1));

    // Faz a transposta da matriz B e combina ela com a matriz T
    mat4 view_Matrix = transpose(B) * T;

    // Cria um float d que representa a distância do centro focal
    // até o near plane
    float d = 0.5f;

    // Cria a matriz projection baseada no d para gerar uma distorção
    // projetiva
    mat4 projection_Matrix = mat4(vec4(1, 0, 0, 0),
                                  vec4(0, 1, 0, 0),
                                  vec4(0, 0, 1, -1/d),
                                  vec4(0, 0, d, 0));


    // Combina as três matrizes de conversão (model, view e projection)
    // em uma só chamada MVP
    mat4 MVPmatrix = model_Matrix * view_Matrix * projection_Matrix;

    // Aplica a transformação nos pontos do espaço de objeto para o de recorte
    for(unsigned int i = 0; i < model.size(); i++)
    {
        model[i] = model[i] * MVPmatrix;
    }

    // Aplica a divisão que leva do espaço de recorte para o canônico
    for(unsigned int i = 0; i < model.size(); i++)
    {
        model[i] = model[i] / model[i].w;
    }

    // Largura da tela:
    int w = 512;
    // Altura da tela:
    int h = 512;

    // Cria uma matriz que é uma escala de inversão no eixo Y, pois o ponto
    // 0,0 da tela fica no canto superior esquerdo (ao em vez do inferior esquerdo)
    mat4 S1 = mat4(vec4(1, 0, 0, 0),
                   vec4(0,-1, 0, 0),
                   vec4(0, 0, 1, 0),
                   vec4(0, 0, 0, 1));
    // Cria uma matriz para reescalar os valores para um range de 0 ao tamanho da tela
    mat4 T1 = mat4(vec4(w/2, 0, 0, 0),
                   vec4(0, h/2, 0, 0),
                   vec4(0, 0, 1, 0),
                   vec4(1, 1, 0, 1));
    // Cria uma matriz para transladar os vetores de 0 até o tamanho da tela
    mat4 S2 = mat4(vec4(1, 0, 0, 0),
                   vec4(0, 1, 0, 0),
                   vec4(0, 0, 1, 0),
                   vec4((w-1)/2, (h-1)/2, 0, 1));
    // Combina as três matrizes criadas acima para gerar a View Port
    mat4 viewport_Matrix = S2 * T1 * S1;

    // Aplica a transformação do espaço canônico para o de tela
    for(unsigned int i = 0; i < model.size(); i++)
    {
        model[i] = round(viewport_Matrix * model[i]);
    }

    // A partir daqui ocorre a rasterização:
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

