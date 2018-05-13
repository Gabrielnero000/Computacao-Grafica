<h1 align="center">Introdução à Computação Gráfica</h1>

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

<p align="center">
  <img src="https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/scale_matriz.png?raw=true">
</p>

* #### Cisalhamento (Shear)
<p align="center">
  <img src="https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/shear.png?raw=true">
  </p>
