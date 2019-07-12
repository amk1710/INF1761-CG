# INF1761-CG
Trabalhos da disciplina INF1761, computação gráfica, cursado em 2016.2 por André Mazal Krauss. Professores Marcelo Gattass e Jéferson Coelho

	Console Application 1 (https://github.com/amk1710/INF1761-CG/tree/master/ConsoleApplication1) - Renderização com a pipeline OpenGL de arquivos no formato .off. Há dois modos intercambiáveis de renderização: iluminação por fragmentos e iluminação por vértices. C++.  Desenvolvido como parte da disciplina INF1761(Computação Gráfica) 
		com os professores Marcelo Gattass e Jéferson Coelho. Usa código disponibilizado pelo Jéferson como material da disciplina, além da glew e da IUP. O código que desenvolvi
		majoritariamente nos arquivos IUPGLCanvasDummy, e nos shaders: porFrag.vert(vertex shader da iluminação por fragmentos), porFrag.frag(fragment shader da iluminação por fragmentos),
		porVert.frag(fragment shader da iluminação por vértices) e porVert.vert(vertex shader da iluminação por vértices).

		A roda do mouse é usada para zoom in e zoom out, enquanto os botões direito e esquerdo rotacionam o objeto.
		Imagens finais podem ser encontradas na pasta "Imagens Finais".
	
	Ray Tracing (https://github.com/amk1710/INF1761-CG/tree/master/RayTracing): Implementação em c++ de um algoritmo de geração de imagens por traçado de raios. Não exaustivamente recursivo. Um exemplo de imagem final é "exemplo.bmp". Desenvolvido como parte da disciplina INF1761(Computação Gráfica)
		com os professores Marcelo Gattass e Jéferson Coelho.

	SLIC - SuperPixels - Implementação em C++ de um algoritmo para particionar uma imagem bitmap em n superpixels, que seguem os contornos da imagem. Um exemplo de imagem final é "exemplo-mapa_mundi.bmp". As imagens originais e mais exemplos são encontrados na pasta Imagens

O código que escrevi para este trabalho está disponível abertamente através da licença MIT.
Este trabalho usa software desenvolvido por terceiros e disponibilizado abertamente. Para informação sobre licenças, checar o arquivo license_information.txt.

The software I developed in this repository is avaiable under the terms of the MIT license.
The work in this repository uses openly avaiable third-party software. For licensing information, please check license_information.txt.