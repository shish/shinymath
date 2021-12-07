#include <math.h>
#include "utils.h"

int generate() {
	float res = M_PI/32, n=0;

	for(n=2; n<=8*M_PI; n+=res) {
		addPoint2d(
			cos(n)*log(n),
			sin(n)*log(n)
		);
	}

	return 1;
}

