#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#define IMAGE_WIDTH  512 // número de colunas da imagem.
#define IMAGE_HEIGHT 512 // número de linhas da imagem.

unsigned char * FBptr;

typedef struct pixel{
	int x;
	int y;
	int R;
	int G;
	int B;
	int A;
} Pixel;

#endif // _DEFINITIONS_H_

