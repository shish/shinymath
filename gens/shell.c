#include <math.h>
#include "utils.h"

int generate() {
	float res = M_PI/32, n=0;

	for(n=0; n<=4*M_PI; n+=res) {
		addPoint2d(
			cos(n)*n,
			sin(n)*n
		);
	}

	return 1;
}

