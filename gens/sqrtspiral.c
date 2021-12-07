#include <math.h>
#include "utils.h"

int generate() {
	float res = M_PI/32, n=0;
	
	for(n=0; n<=8*M_PI; n+=res) {
		addPoint2d(
			cos(n)*sqrt(n),
			sin(n)*sqrt(n)
		);
	}

	return 1;
}

