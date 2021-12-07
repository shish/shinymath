#include <math.h>
#include "utils.h"

// -0.001 because otherwise 2pi > 4 * 0.5pi

int generate() {
	float res = M_PI/32, n=0;

	for(n=0; n<=2*M_PI; n+=res) {
		addPoint2d(
			cos(n)*fmod(n, (M_PI/2)-0.001),
			sin(n)*fmod(n, (M_PI/2)-0.001)
		);
	}

	return 1;
}

