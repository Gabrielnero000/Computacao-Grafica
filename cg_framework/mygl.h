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
}

void DrawLineUp(Pixel pi, Pixel pf){

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

void DrawTriangleLine(Pixel pi, Pixel pf){
	if((pf.x - pi.x) < 0)
		DrawTriangleLine(pf, pi);
	Pixel p = pi;
	for(p.x; p.x <= pf.x; p.x+=1){
		putPixel(p);
	}
	
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

void DrawFilledTriangle(Pixel v1, Pixel v2, Pixel v3){
	SortByY(&v1, &v2, &v3);
	double dx1, dx2, dx3;
	double dr1, dg1, db1, da1, dr2, dg2, db2, da2, dr3, dg3, db3, da3;
	double dr, dg, db, da;
	if(v2.y - v1.y > 0){
		dx1 = (double)(v2.x - v1.x) / (v2.y - v1.y);
		dr1 = (double)(v2.R - v1.R) / (v2.y - v1.y);
		dg1 = (double)(v2.G - v1.G) / (v2.y - v1.y);
		db1 = (double)(v2.B - v1.B) / (v2.y - v1.y);
		da1 = (double)(v2.A - v1.A) / (v2.y - v1.y);
	}
	else
		dx1 = dr1 = dg1 = db1 = da1 = 0;
	if(v3.y - v1.y > 0){
		dx2 = (double)(v3.x - v1.x) / (v3.y - v1.y);
		dr2 = (double)(v3.R - v1.R) / (v3.y - v1.y);
		dg2 = (double)(v3.G - v1.G) / (v3.y - v1.y);
		db2 = (double)(v3.B - v1.B) / (v3.y - v1.y);
		da2 = (double)(v3.A - v1.A) / (v3.y - v1.y);
	}
	else
		dx2 = dr2 = dg2 = db2 = da2 = 0;
	if(v3.y - v2.y > 0){
		dx3 = (double)(v3.x - v2.x) / (v3.y - v2.y);
		dr3 = (double)(v3.R - v2.R) / (v3.y - v2.y);
		dg3 = (double)(v3.G - v2.G) / (v3.y - v2.y);
		db3 = (double)(v3.B - v2.B) / (v3.y - v2.y);
		da3 = (double)(v3.A - v2.A) / (v3.y - v2.y);
	}
	else
		dx3 = dr3 = dg3 = db3 = da3 = 0;
	Pixel S, E, P;
	S = E = v1;
	if(dx1 > dx2){
		for(;S.y <= v2.y; S.y++, E.y++){
			if(E.x - S.x > 0){
				dr=(double)(E.R - S.R) / (E.x - S.x);
				dg=(double)(E.G - S.G) / (E.x - S.x);
				db=(double)(E.B - S.B) / (E.x - S.x);
				da=(double)(E.A - S.A) / (E.x - S.x);
			}else
				dr=dg=db=0;		
			
			P = S;
			for(;P.x < E.x; P.x++){
				putPixel(E);
				P.R += dr; P.G += dg; P.B += db; P.A += da;	
			}
			S.x += dx2; S.R += dr2; S.G += dg2; S.B += db2; S.A += da2;
			E.x += dx1; E.R += dr1; E.G += dg1; E.B += db1; E.A += da2;
		}
		E = v2;
		for(;S.y <= v3.y; S.y++, E.y++){
			if(E.x - S.x > 0){
				dr=(double)(E.R - S.R) / (E.x - S.x);
				dg=(double)(E.G - S.G) / (E.x - S.x);
				db=(double)(E.B - S.B) / (E.x - S.x);
				da=(double)(E.A - S.A) / (E.x - S.x);
			}else
				dr=dg=db=0;		
			
			P = S;
			for(;P.x < E.x; P.x++){
				putPixel(P);
				P.R += dr; P.G += dg; P.B += db; P.A += da;	
			}
			S.x += dx2; S.R += dr2; S.G += dg2; S.B += db2; S.A += da2;
			E.x += dx3; E.R += dr3; E.G += dg3; E.B += db3; E.A += da3;
		}
			
	}else{
		for(;S.y <= v2.y; S.y++, E.y++){
			if(E.x - S.x > 0){
				dr=(double)(E.R - S.R) / (E.x - S.x);
				dg=(double)(E.G - S.G) / (E.x - S.x);
				db=(double)(E.B - S.B) / (E.x - S.x);
				da=(double)(E.A - S.A) / (E.x - S.x);
			}else
				dr=dg=db=0;		
			
			P = S;
			for(;P.x < E.x; P.x++){
				putPixel(P);
				P.R += dr; P.G += dg; P.B += db; P.A += da;	
			}
			S.x += dx1; S.R += dr1; S.G += dg1; S.B += db1; S.A += da1;
			E.x += dx2; E.R += dr2; E.G += dg2; E.B += db2; E.A += da2;
		}
		S = v2;
		for(;S.y <= v3.y; S.y++, E.y++){
			if(E.x - S.x > 0){
				dr=(double)(E.R - S.R) / (E.x - S.x);
				dg=(double)(E.G - S.G) / (E.x - S.x);
				db=(double)(E.B - S.B) / (E.x - S.x);
				da=(double)(E.A - S.A) / (E.x - S.x);
			}else
				dr=dg=db=0;		
			
			P = S;
			for(;P.x < E.x; P.x++){
				putPixel(P);
				P.R += dr; P.G += dg; P.B += db; P.A += da;	
			}
			S.x += dx3; S.R += dr3; S.G += dg3; S.B += db3; S.A += da3;
			E.x += dx2; E.R += dr2; E.G += dg2; E.B += db2; E.A += da2;
		}
	}
}
#endif // _MYGL_H_

