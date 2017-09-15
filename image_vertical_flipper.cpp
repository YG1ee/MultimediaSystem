#include <stdio.h>
#include <stdlib.h>

#define WIDTH 230
#define HEIGHT 390
#define TYPE 3        // 1 - gray, 3 - RGB

char buff[HEIGHT][WIDTH][TYPE]; // 0과 255 사이의 수를 지정하기 위해 char 선언

int main() {
	FILE* fp = fopen("C:\\yong\\rawreader\\Alice_230x390_8).raw", "rb");

	fread(
		buff,				// 이미지 저장 버퍼
		sizeof(char),		// 이미지 데이터 타입
		WIDTH*HEIGHT*TYPE,	// 이미지 크기
		fp					// 파일
	);

	fclose(fp);

	char temp;

	for (int y = 0; y < HEIGHT; y += 1) {
		for (int x = 0; x < WIDTH/2; x += 1) {
			for (int k = 0; k < TYPE; k += 1) {
				temp = buff[y][x][k];
				buff[y][x][k] = buff[y][WIDTH - x][k];
				buff[y][WIDTH - x][k] = temp;
			}
		}
	}

	FILE* fp2 = fopen("C:\\yong\\rawreader\\Alice_230x390_8)_flipped.raw", "wb");

	fwrite(
		buff,
		sizeof(char),
		WIDTH*HEIGHT*TYPE,
		fp2
	);

	return 0;
}
