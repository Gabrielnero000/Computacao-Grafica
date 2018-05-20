
﻿<h1 align="center">Introdução à Computação Gráfica</h1>

<p align="center">
  <img src="https://raw.githubusercontent.com/Gabrielnero000/Computacao-Grafica/master/assets/kratossmile.jpg">
  <sub>God of War - 2018
</p>

Este repostório é parte da disciplina de Introdução à Computação Gráfica lecionada pelo professor Christian Pagot na Universidade Federal da Paraíba. O conteúdo visa expôr técnicas básicas de computação gráfica de uma maneira simples e didáticas. A implementação conta com um pequeno framework fornecido pelo professor citado que simula acesso direto à memória de vídeo (framebuffer). Pronto para a dança?

 * [Parte 1 - Principios básicos e rasteirização (COMING SOON)](https://github.com/Gabrielnero000/Computacao-Grafica#parte-1-principios-b%C3%A1sicos-e-rasteiriza%C3%A7%C3%A3o-coming-soon)
 * [Parte 2 - Pipeline Gráfico: calma, ele não morde](https://github.com/Gabrielnero000/Computacao-Grafica#parte-2-pipeline-gr%C3%A1fico-calma-ele-n%C3%A3o-morde)
  
## Bibliotecas utilizadas:
* [freeglut](http://freeglut.sourceforge.net/) - gerenciador de janelas para o OpenGL usado no simulador.
* [OBJ-Loader](https://github.com/Bly7/OBJ-Loader) - pequeno utilitário para carregar modelos .obj.
* [glm](https://glm.g-truc.net/0.9.8/index.html) - biblioteca para auxiliar na manipulação de matrizes e vetores.

## Parte 1: Principios básicos e rasteirização (COMING SOON)

## Parte 2: Pipeline Gráfico: calma, ele não morde

<p align="center">
  <img src="https://raw.githubusercontent.com/Gabrielnero000/Computacao-Grafica/master/assets/susy.png">
  <sub>Susy após o pipeline aqui implementado
</p>

O **pipeline gráfico** nada mais é do que o processo em que dados em memória (**vertex data**) são manipulados até formarem a imagem final na tela. Consiste em uma série de transformações algébricas  que são divididas em algumas etapas, as quais explicaremos brevemente:

<p align="center">
  <img src="https://raw.githubusercontent.com/Gabrielnero000/Computacao-Grafica/master/assets/pipeline.jpeg">
  <sub>Diagrama do pipeline gráfico
</p>

### Espaço do objeto
Quando um objeto é modelado (usando  o **Blender**,  **Autodesk Maya**, etc) o fazemos dentro de um espaço. Este espaço é chamado **espaço do objeto**. Neste espaço, a posição do objeto só depende do próprio objeto. É neste espaço onde transformações relativas ao objeto (**escalonamento**, **rotação**, **translação** e etc) ocorrem.

<p align="center">
  <img src="https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/object_space.png?raw=true">
  <sub>No espaço do objeto, a posição só depende do objeto
</p>

### Espaço do universo
Ao adicionarmos objetos num cenário, entramos no **espaço do universo**, onde a posição dos objetos já não dependem deles mesmos, mas o contrário: definindo as bases deste espaço, a posição dos objetos dependem do ponto que ocupam no espaço.

<p align="center">
  <img src="https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/universe_space.png?raw=true">
  <sub>A posição depende de onde o objeto se localiza no espaço
</p>

### Espaço da câmera
Depois do cenário montado, é necessário estabelecer um "ponto de vista": uma **câmera**, que também possui seu próprio espaço. Aqui, a posição dos objetos dependem do ponto que se encontram em relação ao observador. 

<p align="center">
  <img src="https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/camera_space.png?raw=true">
  <sub>Tudo depende do "ponto de vista"
</p>

### Espaço de recorte
É aqui onde a brincadeira começa a ficar séria. Na passagem para este espaço são aplicadas as **distorções de perspectiva**, para causar (ou não) a ilusão de profundidade.

<p align="center">
  <img src="https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/clipping_space.png?raw=true">
  <sub>Uma distorção de perspectiva exagerada
</p>

### Espaço canônico
Neste espaço, gera-se um "cubo" que contém todos os objetos da cena (fará sentido mais tarde). É nesta etapa que objetos fora do alcance da visão da câmera são descartados.

<p align="center">
  <img src="https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/cannonic_space.png?raw=true">
  <sub>É gerado um "cubo" que engloba toda cena visível
</p>

### Espaço da tela
Por fim, os objetos passam para o **espaço da tela**, onde o objeto é finalmente projetado na tela.

<p align="center">
  <img src="https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/screen_space.png?raw=true">
  <sub>No fim, os dados são transformados em pixels
</p>

### Mas afinal, como funciona esse processo?
Não é difícil visualizar que estamos lidando com processos relacionados à **cálculos vetoriais** e **álgebra linear**. Todas as transformações de um espaço por outro ocorrem por meio da aplicação de um **operador linear**, que neste caso será sempre uma, **matriz** sobre os **vetores** que compõem um objeto, levando-os de um **espaço vetorial** para outro. Soa mais complicado do que realmente é, mas vamos devagar.

### Transformações básicas
Como dito anteriormente, no espaço do objeto diversas transformações podem ser aplicadas. Essas transformações podem ser de três categorias: linear, afim ou projetiva. Vamos preferir trabalhar apenas com transformações lineares, pois elas sempre podem ser compactadas em matrizes.

 Se $\vec{X} = \left[x, y, z, 1 \right]^T$ é o vetor que denota um vértice pertencente ao objeto, as principais transformações podem ser denotadas como:
* #### Escalonamento

<p align="center">
  <img src="https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/scale.png?raw=true">
</p>

A transformação de escalonamento afeta as dimensões do objedo, de forma que cada **coordenada** do vetor $\vec{X}$ é multiplicada por **escalares** $s_x$, $s_y$ e $s_z$, respectivamente. Para  $s_\gamma > 1$, o objeto aumenta de tamanho na direção  $\gamma$.  Para $1 > s_\gamma > 0$, o objeto diminui de tamanho na direção $\gamma$. Para $s_\gamma < 0$, ocorre um espelhamento junto com o escalonamento do objeto na direção $\gamma$. A escala pode ser **isotropica**, onde todos os escalares são iguais, ou **anisiotrópica**, onde algum escalar é diferente dos demais.

$$\begin{matrix} x'=x.s_x \\[0.3em] y'=y.s_y \\[0.3em] z'=z.s_z \\[0.3em] 1 = 1
\end{matrix} \implies
\begin{bmatrix} x'\\[0.3em] y' \\[0.3em] z' \\[0.3em] 1\end{bmatrix}=
\begin{bmatrix} s_x  & 0 & 0  & 0\\[0.3em]
							0 & s_y & 0  & 0\\[0.3em]
							0 & 0 & s_z  & 0\\[0.3em]
							0 & 0 & 0  & 1\end{bmatrix}
.\begin{bmatrix} x\\[0.3em] y\\[0.3em] z\\[0.3em] 1\end{bmatrix}$$

* #### Cisalhamento (Shear)
<p align="center">
  <img src="https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/shear.png?raw=true">
  </p>
  
## Parte 3: Implementação do Pipeline

Após a introdução teórica acerca do **Pipeline** e da **Rasterização de Primitivas**, chegou o momento de ser colocado em prática os conceitos aprendidos. Para isso, foi usado da linguagem C++ e das bibliotecas indicadas no inicio desse texto.

### Importação do Objeto

O modelo usado para demonstrar os resultados da implementação foi a Susy, um dos objetos padrões que podem ser gerados no Blender, através do formato .obj e sua importação foi feita utilizando a biblioteca OBJ-Loader.

### Matriz Model

Como explicado anteriormente, o objeto inicia com seu centro na origem dentro de um espaço conhecido como **Espaço do Objeto**. Para que ele seja posicionado em cena junto com os demais objetos é necessário executar mudanças em sua posição, tamanho e até em sua rotação, sendo todas essas mudanças condensadas na **Matriz Model**, uma matriz cuja qual qualquer vértice (com suas coordenadas representadas através de um vetor) precisa ser multiplicado para que possa fazer a transição para o **Espaço do Universo**. 
    
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

### Matriz View

Ao estar no Espaço do Universo, o próximo passo é definir uma câmera a qual irá capturar a cena aonde estão os objetos. Para isso é necessário escolher primeiramente os eixos da mesma através das informações que são necessárias para gerá-la, isto é, o ponto relativo à sua **posição** (position), o ponto para onde ela está **olhando** (look at), a partir desses dois pontos gerar um **vetor de direção** (direction) e, por fim, possuir o **vetor UP**, um vetor que representa o que seria "olhar para cima" no Espaço do Universo, que , nesse caso, é tomado como sendo igual ao vetor unitário que representa o eixo y do Universo.

$$\begin{matrix} Position: p = (p_x, p_y, p_z)\\[0.3em] LookAt: l = (l_x,l_y,l_z) \\[0.3em] VectorUP: u = (u_x,u_y,u_z)\\[0.3em] Direction: d=(l_x-p_x,l_y-p_y,l_z-p_z)
	\end{matrix}$$
	
O primeiro eixo do sistema de coordenadas da câmera a ser buscado é o **eixo z**, já que para gerá-lo basta encontrar o vetor que vai do ponto da posição da câmera até aquele que ela está olhando (resumidamente, o direction) e, em seguida, obter o unitário contrário a esse vetor (a câmera sempre aponta para o lado contrário ao seu eixo z fixada na origem de seu sistema de coordenadas). Em outras palavras:

$$\hat{Z_c} = - \frac{d}{|d|} = (z_{cx},z_{cy},z_{cz})$$
Em seguida, é possível encontrar o **eixo x** ao fazer o produto vetorial entre o $u$ e o $\hat{z_c}$ para obter o vetor perpendicular a esses dois e, após isso, dividir ele pelo módulo de si mesmo para obter seu unitário:
$$\hat{X_c} = \frac{u \times \hat{z_c} }{|u \times \hat{z_c} | } = (x_{cx},x_{cy},x_{cz})$$

Por fim, encontra-se o **eixo y** seguindo o mesmo procedimento usado acima, porém fazendo desta vez um produto vetorial entre $\hat{z_c}$ e $\hat{x_c}$:

$$\hat{Y_c} = \frac{\hat{z_c} \times \hat{x_c} }{ |\hat{z_c} \times \hat{x_c}| } = (y_{cx},y_{cy},y_{cz})$$

Após tal procedimento é preciso construir as matrizes que formarão a View. Sendo elas uma **Matriz B** que serve para representar os pontos do sistema de coordenadas do Universo no novo sistema relativo ao **Espaço da Câmera** e uma **Matriz T** usada para transladar todos os vértices em relação ao ponto position com objetivo de posicionar a câmera na origem.

$$B^T =
\begin{bmatrix}   x_{cx}  & x_{cy} & x_{cz}  & 0\\[0.3em]
							y_{cx}  & y_{cy} & y_{cz}  & 0\\[0.3em]
							z_{cx}  & z_{cy} & z_{cz}  & 0\\[0.3em]
							0 & 0 & 0  & 1\end{bmatrix}
\text{ e }
T =
\begin{bmatrix}   1 & 0 & 0 & -p_x\\[0.3em]
							0 & 1 & 0 & -p_y\\[0.3em]
							0 & 0 & 1 & -p_z\\[0.3em]
							0 & 0 & 0  & 1\end{bmatrix}$$

Para combinar tais matrizes e gerar a **Matriz View** é preciso apenas multiplicar uma pela outra.

$$M_{view} = B^T \times T$$

Programacionalmente isso pode ser resumido em algumas poucas linhas ao usar a biblioteca padrão vector de C++ e ao importar a glm para o uso de suas funções.


    // A função normalize transforma vetores em vetores unitários
    // A função cross retorna o produto vetorial entre dois vetores
    vec3 camera_z = -normalize(camera_target - camera_pos);
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
                  
    mat4 view_Matrix = B * T;
                  



