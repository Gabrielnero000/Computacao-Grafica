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
  <img src="https://raw.githubusercontent.com/Gabrielnero000/Computacao-Grafica/master/assets/pipeline.jpeg" width="1000" height="600">
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
  <img src="https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/screen_space.png?raw=true" width="1000" height="500">
  <sub>No fim, os dados são transformados em pixels
</p>

### Mas afinal, como funciona esse processo?
Não é difícil visualizar que estamos lidando com processos relacionados à **cálculos vetoriais** e **álgebra linear**. Todas as transformações de um espaço por outro ocorrem por meio da aplicação de um **operador linear**, que neste caso será sempre uma, **matriz** sobre os **vetores** que compõem um objeto, levando-os de um **espaço vetorial** para outro. Soa mais complicado do que realmente é, mas vamos devagar.

  
## Implementação do Pipeline

Após a introdução teórica acerca do **Pipeline** e da **Rasterização de Primitivas**, chegou o momento de ser colocado em prática os conceitos aprendidos. Para isso, foi usado da linguagem C++ e das bibliotecas indicadas no inicio desse texto.

### Importação do Objeto

O modelo usado para demonstrar os resultados da implementação foi a Susy, um dos objetos padrões que podem ser gerados no Blender, através do formato .obj e sua importação foi feita utilizando a biblioteca OBJ-Loader.

### Matriz Model
Como dito anteriormente, no espaço do objeto diversas transformações podem ser aplicadas. Essas transformações podem ser de três categorias: linear, afim ou projetiva. Vamos preferir trabalhar apenas com transformações lineares, pois elas sempre podem ser compactadas em matrizes.

 Se <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/62637c2b72fa8c4dca0837cdf4b3b12d.svg?invert_in_darkmode" align=middle width=112.041105pt height=32.25585pt/> é o vetor que denota um vértice pertencente ao objeto, as principais transformações podem ser denotadas como:
 
* #### Escalonamento
<p align="center">
  <img src="https://raw.githubusercontent.com/Gabrielnero000/Computacao-Grafica/master/assets/scale.jpeg" width="1000" height="500">
</p>

A transformação de escalonamento afeta as dimensões do objedo, de forma que cada **coordenada** do vetor <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/1e6682a7e06941e96dc9ec2c99d32d8e.svg?invert_in_darkmode" align=middle width=14.90874pt height=31.79913pt/> é multiplicada por **escalares** <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/99036d87bf2572fcf6e37a0f99818e2e.svg?invert_in_darkmode" align=middle width=15.15987pt height=14.15535pt/>, <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/e3bcffc05e1413cfdca192e030566bba.svg?invert_in_darkmode" align=middle width=14.785155pt height=14.15535pt/> e <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/18dc843d3217f94cff62acc1f661e343.svg?invert_in_darkmode" align=middle width=14.45796pt height=14.15535pt/>, respectivamente. Para  <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/8da4ccd50620c93910995bcde0c222ad.svg?invert_in_darkmode" align=middle width=46.277055pt height=21.18732pt/>, o objeto aumenta de tamanho na direção  <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/11c596de17c342edeed29f489aa4b274.svg?invert_in_darkmode" align=middle width=9.423975pt height=14.15535pt/>.  Para <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/a405f1e053899e4de72b7283664392df.svg?invert_in_darkmode" align=middle width=76.41381pt height=21.18732pt/>, o objeto diminui de tamanho na direção <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/11c596de17c342edeed29f489aa4b274.svg?invert_in_darkmode" align=middle width=9.423975pt height=14.15535pt/>. Para <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/869402577740cb08a5364b26905cd7cc.svg?invert_in_darkmode" align=middle width=46.277055pt height=21.18732pt/>, ocorre um espelhamento junto com o escalonamento do objeto na direção <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/11c596de17c342edeed29f489aa4b274.svg?invert_in_darkmode" align=middle width=9.423975pt height=14.15535pt/>. A escala pode ser **isotropica**, onde todos os escalares são iguais, ou **anisiotrópica**, onde algum escalar é diferente dos demais.

<p align="center"><img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/c2e1bd9ec4a93d3280b473821cfcf3ab.svg?invert_in_darkmode" align=middle width=336.138pt height=89.783265pt/></p>

* #### Rotação

<p align="center">
  <img src="https://raw.githubusercontent.com/Gabrielnero000/Computacao-Grafica/master/assets/rotation.jpeg" width="1000" height="500">
</p>

A rotação consiste em girar o objeto em torno de algum dos eixos. Se valendo de principios trigonométricos, podemos escrever todas as coordenadas como função do ângulo <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/27e556cf3caa0673ac49a8f0de3c73ca.svg?invert_in_darkmode" align=middle width=8.1430305pt height=22.74591pt/> que desejamos rotacionar. As rotações em torno dos eixos <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/332cc365a4987aacce0ead01b8bdcc0b.svg?invert_in_darkmode" align=middle width=9.359955pt height=14.10255pt/>, <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/deceeaf6940a8c7a5a02373728002b0f.svg?invert_in_darkmode" align=middle width=8.61696pt height=14.10255pt/>, e  <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/f93ce33e511096ed626b4719d50f17d2.svg?invert_in_darkmode" align=middle width=8.33646pt height=14.10255pt/> podem ser representadas, respectivamente, pelas matrizes:
<p align="center"><img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/529f687dc559bf5cb00b28c8581481eb.svg?invert_in_darkmode" align=middle width=514.72575pt height=89.651265pt/></p>

<p align="center"><img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/87c349d375b8ea5f94d8f21f96c946c7.svg?invert_in_darkmode" align=middle width=515.1894pt height=89.651265pt/></p>

<p align="center"><img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/6a3fea70fd38e1acd1dd4ebc5748f327.svg?invert_in_darkmode" align=middle width=502.96785pt height=89.651265pt/></p>
* #### Cisalhamento (Shear)

<p align="center">
  <img src="https://raw.githubusercontent.com/Gabrielnero000/Computacao-Grafica/master/assets/shear.jpeg" width="1000" height="500">
</p>

O cisalhamento (conhecido por **shear**) consiste em gerar uma distorção nos vértices de maneira uniforme por meio de uma incrementação, de forma que a área (ou volume, se em 3D) do objeto não é alterada. É uma transformação incomumente utilizada explicitamente, porém mais a frente veremos que ela é extremamente útil.

<p align="center"><img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/c53ea42a6a89b301b6785a209ebaefb1.svg?invert_in_darkmode" align=middle width=363.38775pt height=89.651265pt/></p>

### Mas e essa quarta coordenada?
Note que, apesar de estarmos trabalhando no <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/bb70675111fc3226508b95d261fa59cc.svg?invert_in_darkmode" align=middle width=21.358425pt height=27.72561pt/> (três dimensões), os vetores e matrizes estão no <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/a8739591b1c8f032e085f4b2f7837eb6.svg?invert_in_darkmode" align=middle width=22.06347pt height=32.0925pt/>. Esta última coordenada é chamada **coordenada homogênea**, e possui diversas aplicações, como por exemplo na transformação de **translação**, que é uma **transformação afim**, que pode ser simulado por uma transformação **shear**. Isso ocorre por que ao aplicarmos um shear no <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/2b6bb70bf6a1b3c106852c8b5d0bf20f.svg?invert_in_darkmode" align=middle width=41.64765pt height=27.72561pt/>, a visualização no <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/5e3a0192e90ff45fe43714a12c4c725c.svg?invert_in_darkmode" align=middle width=22.06347pt height=26.19969pt/> é que o objeto se deslocou. Outra aplicação, como veremos mais a frente, se encontra na aplicação da distorção perspectiva.

A combinação (leia-se multiplicação) dessas matrizes gera a **Matriz Model**, cuja qual qualquer vértice (com suas coordenadas representadas através de um vetor) precisa ser multiplicado para que possa fazer a transição para o **Espaço do Universo**. Em código, temos:
 ```c++   
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
```

### Matriz View

Ao estar no Espaço do Universo, o próximo passo é definir uma câmera a qual irá capturar a cena aonde estão os objetos. Para isso é necessário escolher primeiramente os eixos da mesma através das informações que são necessárias para gerá-la, isto é, o ponto relativo à sua **posição** (position), o ponto para onde ela está **olhando** (look at), a partir desses dois pontos gerar um **vetor de direção** (direction) e, por fim, possuir o **vetor UP**, um vetor que representa o que seria "olhar para cima" no Espaço do Universo, que , nesse caso, é tomado como sendo igual ao vetor unitário que representa o eixo y do Universo.

<p align="center"><img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/e97d39a044578ed63867ac96082cec79.svg?invert_in_darkmode" align=middle width=289.94295pt height=90.95031pt/></p>
	
O primeiro eixo do sistema de coordenadas da câmera a ser buscado é o **eixo z**, já que para gerá-lo basta encontrar o vetor que vai do ponto da posição da câmera até aquele que ela está olhando (resumidamente, o direction) e, em seguida, obter o unitário contrário a esse vetor (a câmera sempre aponta para o lado contrário ao seu eixo z fixada na origem de seu sistema de coordenadas). Em outras palavras:

<p align="center"><img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/04c1c5fec173e8bd5310c77309dfcc7a.svg?invert_in_darkmode" align=middle width=187.5687pt height=37.863705pt/></p>
Em seguida, é possível encontrar o **eixo x** ao fazer o produto vetorial entre o <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/6dbb78540bd76da3f1625782d42d6d16.svg?invert_in_darkmode" align=middle width=9.375135pt height=14.10255pt/> e o <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/ae91a0256427659f4d318d44a00d5b71.svg?invert_in_darkmode" align=middle width=13.468785pt height=22.74591pt/> para obter o vetor perpendicular a esses dois e, após isso, dividir ele pelo módulo de si mesmo para obter seu unitário:
<p align="center"><img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/1ac17de3ade939b00b3083f40ab32dce.svg?invert_in_darkmode" align=middle width=217.74555pt height=37.863705pt/></p>

Por fim, encontra-se o **eixo y** seguindo o mesmo procedimento usado acima, porém fazendo desta vez um produto vetorial entre <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/ae91a0256427659f4d318d44a00d5b71.svg?invert_in_darkmode" align=middle width=13.468785pt height=22.74591pt/> e <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/0c7171938924e0be780ce5e79585aab5.svg?invert_in_darkmode" align=middle width=15.21267pt height=22.74591pt/>:

<p align="center"><img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/ae1992708fc475663c7d94d543b4bb0a.svg?invert_in_darkmode" align=middle width=216.3612pt height=37.863705pt/></p>

Após tal procedimento é preciso construir as matrizes que formarão a View. Sendo elas uma **Matriz B** que serve para representar os pontos do sistema de coordenadas do Universo no novo sistema relativo ao **Espaço da Câmera** e uma **Matriz T** usada para transladar todos os vértices em relação ao ponto position com objetivo de posicionar a câmera na origem.

<p align="center"><img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/0c59dba9f73eac77c1d77f3120630a42.svg?invert_in_darkmode" align=middle width=379.87455pt height=88.65747pt/></p>

Para combinar tais matrizes e gerar a **Matriz View** é preciso apenas multiplicar uma pela outra.

<p align="center"><img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/86fc3b28dc6ab015df152cf0a6eb0fdd.svg?invert_in_darkmode" align=middle width=121.76571pt height=17.086575pt/></p>

Programacionalmente isso pode ser resumido em algumas poucas linhas ao usar a biblioteca padrão vector de C++ e ao importar a glm para o uso de suas funções.

 ```c++   
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
```

### Matriz Projection
A próxima matriz é aquela que faz a transição do **Espaço de Câmera** para o **Espaço de Recorte** e, além disso, prepara os vértices para que quando forem divididos por um determinado valor gerem uma distorção nos objetos para conferir uma sensação de perspectiva. Ela é obtida através de uma relação geomêtrica entre a distância de um ponto c (posição da câmera ou centro focal) até um plano conhecido como near plane e as coordenadas x, y e z dos vértices que estarão além desse plano.

<p align="center">
  <img src="https://raw.githubusercontent.com/Gabrielnero000/Computacao-Grafica/master/assets/perspective.jpeg" width="1000" height="500">
</p>

Duas matrizes são extraídas dessa relação: a **Matriz T** que faz uma translação em z para levar o centro focal para a origem e a **Matriz P** que efetivamente aplica a projeção.

<p align="center"><img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/a7c108d70d2bde68ad2e71ec295f07c2.svg?invert_in_darkmode" align=middle width=318.7437pt height=91.021425pt/></p>
								
A junção dessas duas matrizes através de uma multiplicação é o que forma a **Matriz Projection**.
<p align="center"><img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/d0dedd8841d05118bfd9e4d4969b05fc.svg?invert_in_darkmode" align=middle width=287.0736pt height=91.021425pt/></p>

 ```c++   
// Cria um float d que representa a distância do centro focal
// até o near plane
float d = 0.5f;

// Cria a matriz projection baseada no d para gerar uma distorção
// projetiva
mat4 projection_Matrix = mat4(vec4(1, 0, 0,   0),
                              vec4(0, 1, 0,   0),
                              vec4(0, 0, 1,-1/d),
                              vec4(0, 0, d,   0));
```

### Transição para o Espaço Canônico
Dentre todas as transições entre espaços, a do **Espaço de Recorte** para o **Espaço Canônico** é a única que não pode ser expressa através de uma matriz. Para que ela ocorra é necessário dividir o vetor dos vértices pela coordenada homogênea: 
<p align="center"><img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/8880b2d1244df36e7abbc311bf33f1d0.svg?invert_in_darkmode" align=middle width=72.687285pt height=32.950665pt/></p>

E, assim, obter todas as coordenadas divididas por <img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/31fae8b8b78ebe01cbfbe2fe53832624.svg?invert_in_darkmode" align=middle width=12.165285pt height=14.10255pt/> e a coordenada homogênea retornando a ter o valor 1.
 ```c++   
// Aplica a divisão que leva do espaço de recorte para o canônico
for(unsigned int i = 0; i < model.size(); i++)
{
    model[i] = model[i] / model[i].w;
}
```

### Matriz View Port
No final do Pipeline é preciso fazer a transição para o **Espaço da Tela** com objetivo de, assim, a cena ser efetivamente exibida. Para isso, é preciso apenas aplicar algumas transformações básicas: primeiramente uma escala para inverter a imagem no eixo y (pois o ponto (0,0) da tela encontra-se no canto superior esquerdo, diferente do inferior esquerdo que estávamos tomando até agora) e, em seguida, é executada uma translação em conjunto com uma outra escala para mudar todos vértices do intervalo de [ (-1,-1) , (1, 1) ] para [ (0, 0) , (w-1, h-1) ] sendo w a largura da tela e h sua altura.  
<p align="center"><img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/cf6b7cac3d7232c610e1dc4df10483f1.svg?invert_in_darkmode" align=middle width=526.7691pt height=91.464945pt/></p>

Aonde a **Matriz View Port** é a junção dessas três através de uma multiplicação entre elas, em outras palavras:
<p align="center"><img src="https://rawgit.com/Gabrielnero000/Computacao-Grafica/master/svgs/29bff96bcf40ec539e72e1f8b70c1afa.svg?invert_in_darkmode" align=middle width=184.3248pt height=15.885705pt/></p>
