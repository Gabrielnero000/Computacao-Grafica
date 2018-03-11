#ifndef _MYGL_H_
#define _MYGL_H_

#include "definitions.h"

void putPixel(Pixel pixel){

	FBptr[(4*(pixel.x + (pixel.y*IMAGE_WIDTH))) + 0] = pixel.R;
	FBptr[(4*(pixel.x + (pixel.y*IMAGE_WIDTH))) + 1] = pixel.G;
	FBptr[(4*(pixel.x + (pixel.y*IMAGE_WIDTH))) + 2] = pixel.B;
	FBptr[(4*(pixel.x + (pixel.y*IMAGE_WIDTH))) + 3] = pixel.A;
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
			d = d - 2 * dx;	
			p.y = p.y + flag_y;
		}
		else{
			d = d + 2 * dy;
		}
		
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
			d = d - 2 * dy;	
			p.x = p.x + flag_x;
		}
		else{
			d = d + 2 * dx;
		}
		
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

