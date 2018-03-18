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
	int dy = pf.y - pi.y;
	int dx_aux = pf.x - p->x;
	int dy_aux = pf.y - p->y;
	double t =  sqrt(pow(dx_aux, 2) + pow(dy_aux, 2)) / sqrt(pow(dx, 2) + pow(dy, 2));
	p->R = pf.R + (pi.R - pf.R) * t;
	p->G = pf.G + (pi.G - pf.G) * t;
	p->B = pf.B + (pi.B - pf.B) * t;
	p->A = pf.A + (pi.A - pf.A) * t;
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

