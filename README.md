<h1 align="center">Introdução à Computação Gráfica</h1>

![God of War 4 - Modo de fotografia](https://raw.githubusercontent.com/Gabrielnero000/Computacao-Grafica/master/assets/kratossmile.jpg)
<div align="center">
  <sub> God of War 4 - Modo de fotografia
</div>

Este repostório é parte da disciplina de Introdução à Computação Gráfica lecionada pelo professor Christian Pagot na Universidade Federal da Paraíba. O conteúdo visa expôr técnicas básicas de computação gráfica de uma maneira simples e didáticas. A implementação conta com um pequeno framework fornecido pelo professor citado que simula acesso direto à memória de vídeo (framebuffer). Pronto para a dança?

 * [Parte 1 - Principios básicos e rasteirização (COMING SOON)]
 * [Parte 2 - Pipeline Gráfico: calma, ele não morde]
  
## Bibliotecas utilizadas:
* [freeglut](http://freeglut.sourceforge.net/) - gerenciador de janelas para o OpenGL usado no simulador.
* [OBJ-Loader](https://github.com/Bly7/OBJ-Loader) - pequeno utilitário para carregar modelos .obj.
* [glm](https://glm.g-truc.net/0.9.8/index.html) - biblioteca para auxiliar na manipulação de matrizes e vetores.

## Parte 1: Principios básicos e rasteirização (COMING SOON)

## Parte 2: Pipeline Gráfico: calma, ele não morde
![Susy com pipeline](https://raw.githubusercontent.com/Gabrielnero000/Computacao-Grafica/master/assets/susy.png)
<div align="center">
  <sub> Susy após o pipeline aqui implementado
</div>

O **pipeline gráfico** nada mais é do que o processo em que dados em memória (**vertex data**) são manipulados até formarem a imagem final na tela. Consiste em uma série de transformações algébricas  que são divididas em algumas etapas, as quais explicaremos brevemente:

![Pipeline](https://raw.githubusercontent.com/Gabrielnero000/Computacao-Grafica/master/assets/pipeline.jpeg)

<div align="center">
  <sub> Diagrama do pipeline gráfico
</div>

### Espaço do objeto
Quando um objeto é modelado (usando  o **Blender**,  **Autodesk Maya**, etc) o fazemos dentro de um espaço. Este espaço é chamado **espaço do objeto**. Neste espaço, a posição do objeto só depende do próprio objeto. É neste espaço onde transformações relativas ao objeto (**escalonamento**, **rotação**, **translação** e etc) ocorrem.

![Espaço do objeto](https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/object_space.png?raw=true)

<div align="center">
  <sub> No espaço do objeto, a posição só depende do objeto
</div>

### Espaço do universo
Ao adicionarmos objetos num cenário, entramos no **espaço do universo**, onde a posição dos objetos já não dependem deles mesmos, mas o contrário: definindo as bases deste espaço, a posição dos objetos dependem do ponto que ocupam no espaço.

![Espaço do universo](https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/universe_space.png?raw=true)

<div align="center">
  <sub>  A posição depende de onde o objeto se localiza no espaço
</div>

### Espaço da câmera
Depois do cenário montado, é necessário estabelecer um "ponto de vista": uma **câmera**, que também possui seu próprio espaço. Aqui, a posição dos objetos dependem do ponto que se encontram em relação ao observador. 

![Espaço da câmera](https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/camera_space.png?raw=true)

<div align="center">
  <sub>  Tudo depende do "ponto de vista"
</div>

### Espaço de recorte
É aqui onde a brincadeira começa a ficar séria. Na passagem para este espaço são aplicadas as **distorções de perspectiva**, para causar (ou não) a ilusão de profundidade.

![Espaço de recorte](https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/clipping_space.png?raw=true)

<div align="center">
  <sub>  Uma distorção de perspectiva exagerada
</div>

### Espaço canônico
Neste espaço, gera-se um "cubo" que contém todos os objetos da cena (fará sentido mais tarde). É nesta etapa que objetos fora do alcance da visão da câmera são descartados.

![Espaço canônico](https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/cannonic_space.png?raw=true)

<div align="center">
  <sub>  É gerado um "cubo" que engloba toda cena visível
</div>

### Espaço da tela
Por fim, os objetos passam para o **espaço da tela**, onde o objeto é finalmente projetado na tela.

![Espaço da tela](https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/screen_space.png?raw=true)

<div align="center">
  <sub>  No fim, os dados são transformados em pixels
</div>

### Mas afinal, como funciona esse processo?
Não é difícil visualizar que estamos lidando com processos relacionados à **cálculos vetoriais** e **álgebra linear**. Todas as transformações de um espaço por outro ocorrem por meio da aplicação de um **operador linear**, que neste caso será sempre uma, **matriz** sobre os **vetores** que compõem um objeto, levando-os de um **espaço vetorial** para outro. Soa mais complicado do que realmente é, mas vamos devagar.
