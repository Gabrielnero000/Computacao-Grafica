<!DOCTYPE html>
<html>

<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>README</title>
  <link rel="stylesheet" href="https://stackedit.io/style.css" />
</head>

<body class="stackedit">
  <div class="stackedit__left">
    <div class="stackedit__toc">
      
<ul>
<li>
<ul>
<li><a href="#bibliotecas-utilizadas">Bibliotecas utilizadas:</a></li>
<li><a href="#parte-1-principios-básicos-e-rasteirização-coming-soon">Parte 1: Principios básicos e rasteirização (COMING SOON)</a></li>
<li><a href="#parte-2-pipeline-gráfico-calma-ele-não-morde">Parte 2: Pipeline Gráfico: calma, ele não morde</a></li>
</ul>
</li>
</ul>

    </div>
  </div>
  <div class="stackedit__right">
    <div class="stackedit__html">
      <h1 align="center">Introdução à Computação Gráfica</h1>
<p align="center">
  <img src="https://raw.githubusercontent.com/Gabrielnero000/Computacao-Grafica/master/assets/kratossmile.jpg">
  <sub>God of War - 2018
</sub></p>
<p>Este repostório é parte da disciplina de Introdução à Computação Gráfica lecionada pelo professor Christian Pagot na Universidade Federal da Paraíba. O conteúdo visa expôr técnicas básicas de computação gráfica de uma maneira simples e didáticas. A implementação conta com um pequeno framework fornecido pelo professor citado que simula acesso direto à memória de vídeo (framebuffer). Pronto para a dança?</p>
<ul>
<li><a href="https://github.com/Gabrielnero000/Computacao-Grafica#parte-1-principios-b%C3%A1sicos-e-rasteiriza%C3%A7%C3%A3o-coming-soon">Parte 1 - Principios básicos e rasteirização (COMING SOON)</a></li>
<li><a href="https://github.com/Gabrielnero000/Computacao-Grafica#parte-2-pipeline-gr%C3%A1fico-calma-ele-n%C3%A3o-morde">Parte 2 - Pipeline Gráfico: calma, ele não morde</a></li>
</ul>
<h2 id="bibliotecas-utilizadas">Bibliotecas utilizadas:</h2>
<ul>
<li><a href="http://freeglut.sourceforge.net/">freeglut</a> - gerenciador de janelas para o OpenGL usado no simulador.</li>
<li><a href="https://github.com/Bly7/OBJ-Loader">OBJ-Loader</a> - pequeno utilitário para carregar modelos .obj.</li>
<li><a href="https://glm.g-truc.net/0.9.8/index.html">glm</a> - biblioteca para auxiliar na manipulação de matrizes e vetores.</li>
</ul>
<h2 id="parte-1-principios-básicos-e-rasteirização-coming-soon">Parte 1: Principios básicos e rasteirização (COMING SOON)</h2>
<h2 id="parte-2-pipeline-gráfico-calma-ele-não-morde">Parte 2: Pipeline Gráfico: calma, ele não morde</h2>
<p align="center">
  <img src="https://raw.githubusercontent.com/Gabrielnero000/Computacao-Grafica/master/assets/susy.png">
  <sub>Susy após o pipeline aqui implementado
</sub></p>
<p>O <strong>pipeline gráfico</strong> nada mais é do que o processo em que dados em memória (<strong>vertex data</strong>) são manipulados até formarem a imagem final na tela. Consiste em uma série de transformações algébricas  que são divididas em algumas etapas, as quais explicaremos brevemente:</p>
<p align="center">
  <img src="https://raw.githubusercontent.com/Gabrielnero000/Computacao-Grafica/master/assets/pipeline.jpeg">
  <sub>Diagrama do pipeline gráfico
</sub></p>
<h3 id="espaço-do-objeto">Espaço do objeto</h3>
<p>Quando um objeto é modelado (usando  o <strong>Blender</strong>,  <strong>Autodesk Maya</strong>, etc) o fazemos dentro de um espaço. Este espaço é chamado <strong>espaço do objeto</strong>. Neste espaço, a posição do objeto só depende do próprio objeto. É neste espaço onde transformações relativas ao objeto (<strong>escalonamento</strong>, <strong>rotação</strong>, <strong>translação</strong> e etc) ocorrem.</p>
<p align="center">
  <img src="https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/object_space.png?raw=true">
  <sub>No espaço do objeto, a posição só depende do objeto
</sub></p>
<h3 id="espaço-do-universo">Espaço do universo</h3>
<p>Ao adicionarmos objetos num cenário, entramos no <strong>espaço do universo</strong>, onde a posição dos objetos já não dependem deles mesmos, mas o contrário: definindo as bases deste espaço, a posição dos objetos dependem do ponto que ocupam no espaço.</p>
<p align="center">
  <img src="https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/universe_space.png?raw=true">
  <sub>A posição depende de onde o objeto se localiza no espaço
</sub></p>
<h3 id="espaço-da-câmera">Espaço da câmera</h3>
<p>Depois do cenário montado, é necessário estabelecer um “ponto de vista”: uma <strong>câmera</strong>, que também possui seu próprio espaço. Aqui, a posição dos objetos dependem do ponto que se encontram em relação ao observador.</p>
<p align="center">
  <img src="https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/camera_space.png?raw=true">
  <sub>Tudo depende do "ponto de vista"
</sub></p>
<h3 id="espaço-de-recorte">Espaço de recorte</h3>
<p>É aqui onde a brincadeira começa a ficar séria. Na passagem para este espaço são aplicadas as <strong>distorções de perspectiva</strong>, para causar (ou não) a ilusão de profundidade.</p>
<p align="center">
  <img src="https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/clipping_space.png?raw=true">
  <sub>Uma distorção de perspectiva exagerada
</sub></p>
<h3 id="espaço-canônico">Espaço canônico</h3>
<p>Neste espaço, gera-se um “cubo” que contém todos os objetos da cena (fará sentido mais tarde). É nesta etapa que objetos fora do alcance da visão da câmera são descartados.</p>
<p align="center">
  <img src="https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/cannonic_space.png?raw=true">
  <sub>É gerado um "cubo" que engloba toda cena visível
</sub></p>
<h3 id="espaço-da-tela">Espaço da tela</h3>
<p>Por fim, os objetos passam para o <strong>espaço da tela</strong>, onde o objeto é finalmente projetado na tela.</p>
<p align="center">
  <img src="https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/screen_space.png?raw=true">
  <sub>No fim, os dados são transformados em pixels
</sub></p>
<h3 id="mas-afinal-como-funciona-esse-processo">Mas afinal, como funciona esse processo?</h3>
<p>Não é difícil visualizar que estamos lidando com processos relacionados à <strong>cálculos vetoriais</strong> e <strong>álgebra linear</strong>. Todas as transformações de um espaço por outro ocorrem por meio da aplicação de um <strong>operador linear</strong>, que neste caso será sempre uma, <strong>matriz</strong> sobre os <strong>vetores</strong> que compõem um objeto, levando-os de um <strong>espaço vetorial</strong> para outro. Soa mais complicado do que realmente é, mas vamos devagar.</p>
<h3 id="transformações-básicas">Transformações básicas</h3>
<p>Como dito anteriormente, no espaço do objeto diversas transformações podem ser aplicadas. Essas transformações podem ser de três categorias: linear, afim ou projetiva. Vamos preferir trabalhar apenas com transformações lineares, pois elas sempre podem ser compactadas em matrizes.</p>
<p>Se <span class="katex--inline"><span class="katex"><span class="katex-mathml"><math><semantics><mrow><mover accent="true"><mrow><mi>X</mi></mrow><mo>⃗</mo></mover><mo>=</mo><msup><mrow><mo fence="true">[</mo><mi>x</mi><mo separator="true">,</mo><mi>y</mi><mo separator="true">,</mo><mi>z</mi><mo separator="true">,</mo><mn>1</mn><mo fence="true">]</mo></mrow><mi>T</mi></msup></mrow><annotation encoding="application/x-tex">\vec{X} = \left[x, y, z, 1 \right]^T</annotation></semantics></math></span><span class="katex-html" aria-hidden="true"><span class="strut" style="height: 0.981231em;"></span><span class="strut bottom" style="height: 1.23123em; vertical-align: -0.25em;"></span><span class="base"><span class="mord accent"><span class="vlist-t"><span class="vlist-r"><span class="vlist" style="height: 0.96677em;"><span class="" style="top: -3em;"><span class="pstrut" style="height: 3em;"></span><span class="mord"><span class="mord mathit" style="margin-right: 0.07847em;">X</span></span></span><span class="" style="top: -3.25233em;"><span class="pstrut" style="height: 3em;"></span><span class="accent-body accent-vec" style="margin-left: 0.16668em;"><span class="">⃗</span></span></span></span></span></span></span><span class="mrel">=</span><span class="minner"><span class="minner"><span class="mopen delimcenter" style="top: 0em;">[</span><span class="mord mathit">x</span><span class="mpunct">,</span><span class="mord mathit" style="margin-right: 0.03588em;">y</span><span class="mpunct">,</span><span class="mord mathit" style="margin-right: 0.04398em;">z</span><span class="mpunct">,</span><span class="mord mathrm">1</span><span class="mclose delimcenter" style="top: 0em;">]</span></span><span class="msupsub"><span class="vlist-t"><span class="vlist-r"><span class="vlist" style="height: 0.981231em;"><span class="" style="top: -3.2029em; margin-right: 0.05em;"><span class="pstrut" style="height: 2.7em;"></span><span class="sizing reset-size6 size3 mtight"><span class="mord mathit mtight" style="margin-right: 0.13889em;">T</span></span></span></span></span></span></span></span></span></span></span></span> é o vetor que denota um vértice pertencente ao objeto, as principais transformações podem ser denotadas como:</p>
<ul>
<li>
<h4 id="escalonamento">Escalonamento</h4>
</li>
</ul>
<p align="center">
  <img src="https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/scale.png?raw=true">
</p>
<p>A transformação de escalonamento afeta as dimensões do objedo, de forma que cada <strong>coordenada</strong> do vetor <span class="katex--inline"><span class="katex"><span class="katex-mathml"><math><semantics><mrow><mover accent="true"><mrow><mi>X</mi></mrow><mo>⃗</mo></mover></mrow><annotation encoding="application/x-tex">\vec{X}</annotation></semantics></math></span><span class="katex-html" aria-hidden="true"><span class="strut" style="height: 0.96677em;"></span><span class="strut bottom" style="height: 0.96677em; vertical-align: 0em;"></span><span class="base"><span class="mord accent"><span class="vlist-t"><span class="vlist-r"><span class="vlist" style="height: 0.96677em;"><span class="" style="top: -3em;"><span class="pstrut" style="height: 3em;"></span><span class="mord"><span class="mord mathit" style="margin-right: 0.07847em;">X</span></span></span><span class="" style="top: -3.25233em;"><span class="pstrut" style="height: 3em;"></span><span class="accent-body accent-vec" style="margin-left: 0.16668em;"><span class="">⃗</span></span></span></span></span></span></span></span></span></span></span> é multiplicada por <strong>escalares</strong> <span class="katex--inline"><span class="katex"><span class="katex-mathml"><math><semantics><mrow><msub><mi>s</mi><mi>x</mi></msub></mrow><annotation encoding="application/x-tex">s_x</annotation></semantics></math></span><span class="katex-html" aria-hidden="true"><span class="strut" style="height: 0.43056em;"></span><span class="strut bottom" style="height: 0.58056em; vertical-align: -0.15em;"></span><span class="base"><span class="mord"><span class="mord mathit">s</span><span class="msupsub"><span class="vlist-t vlist-t2"><span class="vlist-r"><span class="vlist" style="height: 0.151392em;"><span class="" style="top: -2.55em; margin-left: 0em; margin-right: 0.05em;"><span class="pstrut" style="height: 2.7em;"></span><span class="sizing reset-size6 size3 mtight"><span class="mord mathit mtight">x</span></span></span></span><span class="vlist-s">​</span></span><span class="vlist-r"><span class="vlist" style="height: 0.15em;"></span></span></span></span></span></span></span></span></span>, <span class="katex--inline"><span class="katex"><span class="katex-mathml"><math><semantics><mrow><msub><mi>s</mi><mi>y</mi></msub></mrow><annotation encoding="application/x-tex">s_y</annotation></semantics></math></span><span class="katex-html" aria-hidden="true"><span class="strut" style="height: 0.43056em;"></span><span class="strut bottom" style="height: 0.716668em; vertical-align: -0.286108em;"></span><span class="base"><span class="mord"><span class="mord mathit">s</span><span class="msupsub"><span class="vlist-t vlist-t2"><span class="vlist-r"><span class="vlist" style="height: 0.151392em;"><span class="" style="top: -2.55em; margin-left: 0em; margin-right: 0.05em;"><span class="pstrut" style="height: 2.7em;"></span><span class="sizing reset-size6 size3 mtight"><span class="mord mathit mtight" style="margin-right: 0.03588em;">y</span></span></span></span><span class="vlist-s">​</span></span><span class="vlist-r"><span class="vlist" style="height: 0.286108em;"></span></span></span></span></span></span></span></span></span> e <span class="katex--inline"><span class="katex"><span class="katex-mathml"><math><semantics><mrow><msub><mi>s</mi><mi>z</mi></msub></mrow><annotation encoding="application/x-tex">s_z</annotation></semantics></math></span><span class="katex-html" aria-hidden="true"><span class="strut" style="height: 0.43056em;"></span><span class="strut bottom" style="height: 0.58056em; vertical-align: -0.15em;"></span><span class="base"><span class="mord"><span class="mord mathit">s</span><span class="msupsub"><span class="vlist-t vlist-t2"><span class="vlist-r"><span class="vlist" style="height: 0.151392em;"><span class="" style="top: -2.55em; margin-left: 0em; margin-right: 0.05em;"><span class="pstrut" style="height: 2.7em;"></span><span class="sizing reset-size6 size3 mtight"><span class="mord mathit mtight" style="margin-right: 0.04398em;">z</span></span></span></span><span class="vlist-s">​</span></span><span class="vlist-r"><span class="vlist" style="height: 0.15em;"></span></span></span></span></span></span></span></span></span>, respectivamente. Para  <span class="katex--inline"><span class="katex"><span class="katex-mathml"><math><semantics><mrow><msub><mi>s</mi><mi>γ</mi></msub><mo>&gt;</mo><mn>1</mn></mrow><annotation encoding="application/x-tex">s_\gamma &gt; 1</annotation></semantics></math></span><span class="katex-html" aria-hidden="true"><span class="strut" style="height: 0.64444em;"></span><span class="strut bottom" style="height: 0.930548em; vertical-align: -0.286108em;"></span><span class="base"><span class="mord"><span class="mord mathit">s</span><span class="msupsub"><span class="vlist-t vlist-t2"><span class="vlist-r"><span class="vlist" style="height: 0.151392em;"><span class="" style="top: -2.55em; margin-left: 0em; margin-right: 0.05em;"><span class="pstrut" style="height: 2.7em;"></span><span class="sizing reset-size6 size3 mtight"><span class="mord mathit mtight" style="margin-right: 0.05556em;">γ</span></span></span></span><span class="vlist-s">​</span></span><span class="vlist-r"><span class="vlist" style="height: 0.286108em;"></span></span></span></span></span><span class="mrel">&gt;</span><span class="mord mathrm">1</span></span></span></span></span>, o objeto aumenta de tamanho na direção  <span class="katex--inline"><span class="katex"><span class="katex-mathml"><math><semantics><mrow><mi>γ</mi></mrow><annotation encoding="application/x-tex">\gamma</annotation></semantics></math></span><span class="katex-html" aria-hidden="true"><span class="strut" style="height: 0.43056em;"></span><span class="strut bottom" style="height: 0.625em; vertical-align: -0.19444em;"></span><span class="base"><span class="mord mathit" style="margin-right: 0.05556em;">γ</span></span></span></span></span>.  Para <span class="katex--inline"><span class="katex"><span class="katex-mathml"><math><semantics><mrow><mn>1</mn><mo>&gt;</mo><msub><mi>s</mi><mi>γ</mi></msub><mo>&gt;</mo><mn>0</mn></mrow><annotation encoding="application/x-tex">1 &gt; s_\gamma &gt; 0</annotation></semantics></math></span><span class="katex-html" aria-hidden="true"><span class="strut" style="height: 0.64444em;"></span><span class="strut bottom" style="height: 0.930548em; vertical-align: -0.286108em;"></span><span class="base"><span class="mord mathrm">1</span><span class="mrel">&gt;</span><span class="mord"><span class="mord mathit">s</span><span class="msupsub"><span class="vlist-t vlist-t2"><span class="vlist-r"><span class="vlist" style="height: 0.151392em;"><span class="" style="top: -2.55em; margin-left: 0em; margin-right: 0.05em;"><span class="pstrut" style="height: 2.7em;"></span><span class="sizing reset-size6 size3 mtight"><span class="mord mathit mtight" style="margin-right: 0.05556em;">γ</span></span></span></span><span class="vlist-s">​</span></span><span class="vlist-r"><span class="vlist" style="height: 0.286108em;"></span></span></span></span></span><span class="mrel">&gt;</span><span class="mord mathrm">0</span></span></span></span></span>, o objeto diminui de tamanho na direção <span class="katex--inline"><span class="katex"><span class="katex-mathml"><math><semantics><mrow><mi>γ</mi></mrow><annotation encoding="application/x-tex">\gamma</annotation></semantics></math></span><span class="katex-html" aria-hidden="true"><span class="strut" style="height: 0.43056em;"></span><span class="strut bottom" style="height: 0.625em; vertical-align: -0.19444em;"></span><span class="base"><span class="mord mathit" style="margin-right: 0.05556em;">γ</span></span></span></span></span>. Para <span class="katex--inline"><span class="katex"><span class="katex-mathml"><math><semantics><mrow><msub><mi>s</mi><mi>γ</mi></msub><mo>&lt;</mo><mn>0</mn></mrow><annotation encoding="application/x-tex">s_\gamma &lt; 0</annotation></semantics></math></span><span class="katex-html" aria-hidden="true"><span class="strut" style="height: 0.64444em;"></span><span class="strut bottom" style="height: 0.930548em; vertical-align: -0.286108em;"></span><span class="base"><span class="mord"><span class="mord mathit">s</span><span class="msupsub"><span class="vlist-t vlist-t2"><span class="vlist-r"><span class="vlist" style="height: 0.151392em;"><span class="" style="top: -2.55em; margin-left: 0em; margin-right: 0.05em;"><span class="pstrut" style="height: 2.7em;"></span><span class="sizing reset-size6 size3 mtight"><span class="mord mathit mtight" style="margin-right: 0.05556em;">γ</span></span></span></span><span class="vlist-s">​</span></span><span class="vlist-r"><span class="vlist" style="height: 0.286108em;"></span></span></span></span></span><span class="mrel">&lt;</span><span class="mord mathrm">0</span></span></span></span></span>, ocorre um espelhamento junto com o escalonamento do objeto na direção <span class="katex--inline"><span class="katex"><span class="katex-mathml"><math><semantics><mrow><mi>γ</mi></mrow><annotation encoding="application/x-tex">\gamma</annotation></semantics></math></span><span class="katex-html" aria-hidden="true"><span class="strut" style="height: 0.43056em;"></span><span class="strut bottom" style="height: 0.625em; vertical-align: -0.19444em;"></span><span class="base"><span class="mord mathit" style="margin-right: 0.05556em;">γ</span></span></span></span></span>. A escala pode ser <strong>isotropica</strong>, onde todos os escalares são iguais, ou <strong>anisiotrópica</strong>, onde algum escalar é diferente dos demais.</p>
<p align="center">
  <img src="https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/scale_matriz.png?raw=true">
</p>
<ul>
<li>
<h4 id="cisalhamento-shear">Cisalhamento (Shear)</h4>
</li>
</ul>
<p align="center">
  <img src="https://github.com/Gabrielnero000/Computacao-Grafica/blob/master/assets/shear.png?raw=true">
  </p>

    </div>
  </div>
</body>

</html>
