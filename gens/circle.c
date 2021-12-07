#include <math.h>
#include "utils.h"

int generate() {
	float res = M_PI/32, n=0;
	
	for(n=0; n<=2*M_PI; n+=res) {
		addPoint2d(
			cos(n),
			sin(n)
		);
	}

	return 1;
}

