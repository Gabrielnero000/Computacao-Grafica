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

 Se <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/62637c2b72fa8c4dca0837cdf4b3b12d.svg?invert_in_darkmode" align=middle width=112.041105pt height=32.25585pt/> é o vetor que denota um vértice pertencente ao objeto, as principais transformações podem ser denotadas como:
* #### Escalonamento

<p align="center">
  <img src="https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/scale.png?raw=true">
</p>

A transformação de escalonamento afeta as dimensões do objedo, de forma que cada **coordenada** do vetor <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/1e6682a7e06941e96dc9ec2c99d32d8e.svg?invert_in_darkmode" align=middle width=14.90874pt height=31.79913pt/> é multiplicada por **escalares** <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/99036d87bf2572fcf6e37a0f99818e2e.svg?invert_in_darkmode" align=middle width=15.15987pt height=14.15535pt/>, <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/e3bcffc05e1413cfdca192e030566bba.svg?invert_in_darkmode" align=middle width=14.785155pt height=14.15535pt/> e <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/18dc843d3217f94cff62acc1f661e343.svg?invert_in_darkmode" align=middle width=14.45796pt height=14.15535pt/>, respectivamente. Para  <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/8da4ccd50620c93910995bcde0c222ad.svg?invert_in_darkmode" align=middle width=46.277055pt height=21.18732pt/>, o objeto aumenta de tamanho na direção  <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/11c596de17c342edeed29f489aa4b274.svg?invert_in_darkmode" align=middle width=9.423975pt height=14.15535pt/>.  Para <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/a405f1e053899e4de72b7283664392df.svg?invert_in_darkmode" align=middle width=76.41381pt height=21.18732pt/>, o objeto diminui de tamanho na direção <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/11c596de17c342edeed29f489aa4b274.svg?invert_in_darkmode" align=middle width=9.423975pt height=14.15535pt/>. Para <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/869402577740cb08a5364b26905cd7cc.svg?invert_in_darkmode" align=middle width=46.277055pt height=21.18732pt/>, ocorre um espelhamento junto com o escalonamento do objeto na direção <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/11c596de17c342edeed29f489aa4b274.svg?invert_in_darkmode" align=middle width=9.423975pt height=14.15535pt/>. A escala pode ser **isotropica**, onde todos os escalares são iguais, ou **anisiotrópica**, onde algum escalar é diferente dos demais.

<p align="center"><img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/c2e1bd9ec4a93d3280b473821cfcf3ab.svg?invert_in_darkmode" align=middle width=336.138pt height=89.783265pt/></p>

* #### Cisalhamento (Shear)
<p align="center">
  <img src="https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/shear.png?raw=true">
  </p>
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTU2NDg5MzQ2MV19
-->