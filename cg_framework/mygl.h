#ifndef _MYGL_H_
#define _MYGL_H_

#include "definitions.h"
#include <math.h>

void putPixel(Pixel pixel){

	FBptr[(4*(pixel.x + (pixel.y*IMAGE_WIDTH))) + 0] = pixel.R;
	FBptr[(4*(pixel.x + (pixel.y*IMAGE_WIDTH))) + 1] = pixel.G;
	FBptr[(4*(pixel.x + (pixel.y*IMAGE_WIDTH))) + 2] = pixel.B;
	FBptr[(4*(pixel.x + (pixel.y*IMAGE_WIDTH))) + 3] = pixel.A;
}

void ColorInterpolate(Pixel pi, Pixel *p, Pixel pf){
	int dx = pf.x - pi.x;
	int dy = pf.y - pf.y;
	if(dx < 0 || dy < 0){
		ColorInterpolate(pf, p, pi);
		return;
	}
	int colors[4];
	int colors_aux[4];
	colors[0] = pf.R - pi.R;
	colors[1] = pf.G - pi.G;
	colors[2] = pf.B - pi.B;
	colors[3] = pf.A - pi.A;
	int flag;
	if(abs(dx) >= abs(dy)){
		flag = p->x - pi.x;
		for(int i = 0; i < 4; ++i)
            		colors_aux[i] = colors[i]/dx;
	}else{
		flag = p->y - pi.y;
		for(int i = 0; i < 4; ++i)
            		colors_aux[i] = colors[i]/dy;
	}
	p->R = pi.R + (colors_aux[0] * flag);
	p->G = pi.G + (colors_aux[1] * flag);
	p->B = pi.B + (colors_aux[2] * flag);
	p->A = pi.A + (colors_aux[3] * flag);
}

void DrawLineDown(Pixel pi, Pixel pf){

	Pixel p = pi;
	Pixel *p_ptr = &p;
	int flag_y = 1;
	int dx = pf.x - pi.x;
	int dy = pf.y - pi.y;
	int d = 2 * dy - dx;
	
	if(dy < 0){
		flag_y = -1;
		dy = -dy;
	}

	for(p.x; p.x <= pf.x; p.x+=1){
		putPixel(p);
		if(d > 0){
			d -= 2 * dx;	
			p.y = p.y + flag_y;
		}
		d += + 2 * dy;
		ColorInterpolate(pi, p_ptr, pf);
	}
}

void DrawLineUp(Pixel pi, Pixel pf){

	Pixel p = pi;
	Pixel *p_ptr = &p;
	int flag_x = 1;
	int dx = pf.x - pi.x;
	int dy = pf.y - pi.y;
	int d = 2 * dx - dy;
	
	if(dx < 0){
		flag_x = -1;
		dx = -dx;
	}

	for(p.y; p.y <= pf.y; p.y+=1){
		putPixel(p);
		if(d > 0){
			d -= 2 * dy;	
			p.x = p.x + flag_x;
		}
		d += 2 * dx;
		ColorInterpolate(pi, p_ptr, pf);
	}
}

void DrawLine(Pixel pi, Pixel pf){

	if(abs(pf.x - pi.x) > abs(pf.y - pi.y)){
		if(pf.x > pi.x){
			DrawLineDown(pi, pf);
		}else{
			DrawLineDown(pf, pi);
		}
	}else{
		if(pf.y > pi.y){
			DrawLineUp(pi, pf);
		}else{
			DrawLineUp(pf, pi);
		}
	}
}

void DrawTriangle(Pixel v1, Pixel v2, Pixel v3){
	DrawLine(v1, v2);
	DrawLine(v2, v3);
	DrawLine(v3, v1);
}

#endif // _MYGL_H_

