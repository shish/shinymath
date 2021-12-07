#include <math.h>
#include "utils.h"

int generate() {
	float res = M_PI/32, n=0, m=0;

	for(m=0; m<=2*M_PI; m+=M_PI/4) {
		for(n=0; n<4*M_PI; n+=res) {
			addPoint3d(
				cos(n+m)*n,
				sin(n+m)*n,
				cos(sqrt(cos(n+m)*n*cos(n+m)*n + sin(n+m)*n*sin(n+m)*n)+rad(time))
			);
		}
		addBreakPoint();
	}

	return 1;
}

