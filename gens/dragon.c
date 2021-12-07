#include <math.h>
#include "utils.h"

#define ABS(x) ((x)<0?-(x):(x))

int generate() {
	float res = 0.05;
	float x;

	// head top
	for(x=0; x<5; x+=res) {
		addPoint2d(
			x,
			sin(2*(x+3))/(x+1)+x/10+1
		);
	}
	addBreakPoint();
	
	// head bottom
	for(x=0.5; x<5; x+=res) {
		addPoint2d(
			x,
			-(sin(2*(x+3))/(x+1)+x/10)+0.9
		);
	}
	addBreakPoint();
	
	// tongue
	for(x=3; x<7; x+=res) {
		addPoint2d(
			x,
			sin(8*x-time)/8+1
		);
	}
	addBreakPoint();
	
	// eye
	for(x=0.5; x<1; x+=res) {
		addPoint2d(
			x,
			sin(8*(x-0.5))/8+1.2
		);
	}
	addBreakPoint();

	// body top
	for(x=-4; x<1; x+=res) {
		addPoint2d(
			x,
			-sin(x)+0.5
		);
	}
	addBreakPoint();
	
	// body top scales
	for(x=-M_PI; x<0; x+=res) {
		addPoint2d(
			x,
			(-sin(x)+0.5)+ABS(sin(8*x)/3)
		);
	}
	addBreakPoint();
	
	// body bottom
	for(x=-2.8; x<-0.4; x+=res) {
		addPoint2d(
			x,
			sin(x)/2+1
		);
	}
	addBreakPoint();
	
	// mohawk1
	for(x=0.05; x<M_PI/4-0.05; x+=0.02) {
		if(sin(12*x))
		if(ABS(1/sin(12*x)) < 3)
		addPoint2d(
			x,
			ABS(1/sin(12*x))+sin(x)
		);
	}
	addBreakPoint();

	return 1;
}

