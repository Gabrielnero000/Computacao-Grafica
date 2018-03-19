#include "main.h"

//-----------------------------------------------------------------------------
void MyGlDraw(void)
{
	pixel p;
	p.x = 100;
	p.y = 40;
	p.R = 255;
	p.G = 0;
	p.B = 0;
	p.A = 0;

	pixel p2;
	p2.x = 300;
	p2.y = 510;
	p2.R = 0;
	p2.G = 255;
	p2.B = 0;
	p2.A = 0;

	pixel p3 = p;
	p3.x = 25;
	p3.y = 250;
	p3.R = 0;
	p3.G = 0;
	p3.B = 255;
	p2.A = 0;

	DrawTriangle(p, p2, p3);
	
	DrawFilledTriangle(p, p2, p3);
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

