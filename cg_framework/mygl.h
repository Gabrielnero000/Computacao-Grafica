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

Pixel DrawLineDown(Pixel pi, Pixel pf){

	Pixel p = pi;
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
		ColorInterpolate(pi, &p, pf);		
	}
	p.x--;
	p.y--;
	return p;
}

Pixel DrawLineUp(Pixel pi, Pixel pf){

	Pixel p = pi;
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
		ColorInterpolate(pi, &p, pf);
	}
	p.x--;
	p.y--;
	return p;
}

Pixel DrawLine(Pixel pi, Pixel pf){

	if(abs(pf.x - pi.x) > abs(pf.y - pi.y)){
		if(pf.x > pi.x){
			return DrawLineDown(pi, pf);
		}else{
			return DrawLineDown(pf, pi);
		}
	}else{
		if(pf.y > pi.y){
			return DrawLineUp(pi, pf);
		}else{
			return DrawLineUp(pf, pi);
		}
	}
}

void DrawTriangle(Pixel v1, Pixel v2, Pixel v3){
	DrawLine(v1, v2);
	DrawLine(v2, v3);
	DrawLine(v3, v1);
}

void SortByY(Pixel *p1, Pixel *p2, Pixel *p3){
	Pixel p_aux1, p_aux2, p_aux3;
	if(p1->y <= p2->y && p1->y <= p3->y){
		p_aux1 = *p1;
		if(p2->y <= p3->y){
			p_aux2 = *p2;
			p_aux3 = *p3;			
		}else{
			p_aux2 = *p3;
			p_aux3 = *p2;	
		}	
	}else if(p2->y <= p1->y && p2->y <= p3->y){
		p_aux1 = *p2;
		if(p1->y <= p3->y){
			p_aux2 = *p1;
			p_aux3 = *p3;			
		}else{
			p_aux2 = *p3;
			p_aux3 = *p1;	
		}	
	}else if(p3->y <= p2->y && p3->y <= p1->y){
		p_aux1 = *p3;
		if(p2->y <= p1->y){
			p_aux2 = *p2;
			p_aux3 = *p1;			
		}else{
			p_aux2 = *p1;
			p_aux3 = *p2;	
		}
	}
	*p1 = p_aux1;
	*p2 = p_aux2;
	*p3 = p_aux3;
}

void DrawFilledTriangleUpper(Pixel v1, Pixel v2, Pixel v3){
	pixel aux = v2;
	for(;aux.x <= v3.x; aux.x++){
		ColorInterpolate(v3, &aux, v2);
		DrawLine(v1, aux);
	}
}

void DrawFilledTriangleLower(Pixel v1, Pixel v2, Pixel v3){
	pixel aux = v1;
	for(;aux.x <= v2.x; aux.x++){
		ColorInterpolate(v2, &aux, v1);
		DrawLine(v3, aux);
	}
	
}

void DrawFilledTriangle(Pixel v1, Pixel v2, Pixel v3){
	SortByY(&v1, &v2, &v3);
	if(v1.y == v2.y){
		DrawFilledTriangleLower(v1, v2, v3);	
	}else if(v2.y == v3.y){
		DrawFilledTriangleUpper(v1, v2, v3);
	}else{
		Pixel v4;
		v4.x = (int)(v1.x + ((float)(v2.y - v1.y) / (float)(v3.y - v1.y)) * (v3.x - v1.x));
		v4.y = v2.y;
		ColorInterpolate(v1, &v4, v3);
		DrawFilledTriangleUpper(v1, v2, v4);
		DrawFilledTriangleLower(v2, v4, v3);
	}
}
#endif // _MYGL_H_

