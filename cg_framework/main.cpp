#include "main.h"

//-----------------------------------------------------------------------------
void MyGlDraw(void)
{
	pixel p;
	p.x = 100;
	p.y = 100;
	p.R = 255;
	p.G = 255;
	p.B = 255;
	p.A = 255;

	pixel p2 = p;
	p2.x = 300;
	p2.y = 150;

	pixel p3 = p;
	p3.x = 150;
	p3.y = 300;

	putPixel(p);
	putPixel(p2);	
	putPixel(p3);

	DrawLine(p, p2);		
	
	DrawTriangle(p, p2, p3);


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

