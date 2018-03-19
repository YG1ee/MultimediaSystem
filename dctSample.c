#include <stdio.h>
#include <math.h>
#define pi 3.141592
#define size 8

int main() {
	int u, v, x, y;
	double sampleArray1[8][8] = {
		139, 144, 149, 153, 155, 155, 155, 155,
		144, 151, 153, 156, 159, 159, 156, 156,
		150, 155, 160, 163, 158, 156, 156, 156,
		159, 161, 162, 160, 160, 159, 159, 159,
		159, 160, 161, 162, 162, 155, 155, 155,
		161, 161, 161, 161, 160, 157, 157, 157,
		162, 162, 161, 163, 162, 157, 157, 157,
		162, 162, 161, 161, 163, 158, 158, 158
	};

	const unsigned char quantTable[8][8] = {
		16, 11, 10, 16, 24, 40, 51, 61,
		12, 12, 14, 19, 26, 58, 60, 55,
		14, 13, 16, 24, 40, 57, 69, 56,
		14, 17, 22, 29, 51, 87, 80, 62,
		18, 22, 37, 56, 68, 109, 103, 77,
		24, 35, 55, 64, 81, 104, 113, 92,
		49, 64, 78, 87, 103, 121, 120, 101,
		72, 92, 95, 98, 112, 100, 103, 99
	};

	int quantC[8][8];

	////// DCT
	double C[8];
	C[0] = pow(2.0, -0.5);
	for (x = 1; x < 8; x += 1)
		C[x] = 1.0;

	double F[8][8];
	for (v = 0; v < 8; v += 1)
		for (u = 0; u < 8; u += 1)
			F[u][v] = 0;

	for (u = 0; u < 8; u += 1) {
		for (v = 0; v < 8; v += 1) {
			for (y = 0; y < 8; y += 1)
				for (x = 0; x < 8; x += 1)
					F[u][v] += sampleArray1[x][y]
					* cos((2 * x + 1) * u * pi / 16)
					* cos((2 * y + 1) * v * pi / 16);
			F[u][v] *= (double)1 / 4 * C[u] * C[v];
		}
	}

	printf("\n                            DCT Coefficients\n");
	for (u = 0; u < 8; u += 1) {
		for (v = 0; v < 8; v += 1)
			printf("%9.4lf ", F[u][v]);
		printf("\n");
	}

    printf("\n      Quantized Coefficients\n");
	for (u = 0; u < 8; u += 1) {
		for (v = 0; v < 8; v += 1) {
			quantC[u][v] = (int)F[u][v] / quantTable[u][v];
			printf("%3d ", quantC[u][v]);
		}
		printf("\n");
	}

	////// zigzag scan

    int AC[64];

	AC[0] = quantC[0][0];

    x = 1;

    u = 0;
    v = 0;
    
    for (int diagonal = 1; diagonal < 15; diagonal += 1) {
        if (diagonal < 7) {
            // 왼쪽 절반
            if (diagonal % 2) {
                // 홀수 diagonal
                u = 0; v = diagonal;
                while (v >= 0) {
                    AC[x] = quantC[u][v];
                    x++; u++; v--;
                }
                u++;
            }
            else {
                // 짝수 diagonal
                u = diagonal; v = 0;
                while (u >= 0) {
                    AC[x] = quantC[u][v];
                    x++; u--; v++;
                }
                v++;
            }
        }
        else {
            // 가운데 대각선과 오른쪽 절반
            if (diagonal % 2) {
                // 홀수 diagonal
                u = diagonal - 7; v = 7;
                while (7 >= u) {
                    AC[x] = quantC[u][v];
                    x++; u++; v--;
                }
                v++;
            }
            else {
                u = 7; v = diagonal - 7;
                while (7 >= v) {
                    AC[x] = quantC[u][v];
                    x++; u--; v++;
                }
                u++;
            }
        }
    }

	printf("\n        AC Coefficients");
    
    x = 0;
    for (int z = 0; z < 8; z += 1) {
        for (int y = 0; y < 8; y += 1) {
            if (!(y % 8))
                printf("\n");
            printf("%3d ", AC[x++]);
        }
    }

	////// IDCT
	double f[8][8];
	for (x = 0; x < 8; x += 1)
		for (y = 0; y < 8; y += 1)
			f[x][y] = 0;

	for (y = 0; y < 8; y += 1) {
		for (x = 0; x < 8; x += 1) {
			for (v = 0; v < 8; v += 1)
				for (u = 0; u < 8; u += 1)
					f[x][y] += F[u][v]
					* (double)1 / 4 * C[u] * C[v]
					* cos((2 * x + 1) * u * pi / 16)
					* cos((2 * y + 1) * v * pi / 16);
		}
	}

	printf("\n\n   Invert-DCT Original Sample\n");
	for (x = 0; x < 8; x += 1) {
		for (y = 0; y < 8; y += 1)
			printf("%.0lf ", round(f[x][y]));
		printf("\n");
	}
    printf("\n");


	return 0;
}
