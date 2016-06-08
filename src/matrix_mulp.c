/*
 * For more reference about usage of AVX instructions in C.
 *
 * http://www.cnblogs.com/wangguchangqing/p/5466301.html
 * http://www.codeproject.com/Articles/874396/Crunching-Numbers-with-AVX-and-AVX
 *
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <immintrin.h>

int main(void){
	//Get keys
	float I[8][16];
	char str[16*8];
	scanf("%128s", str);
	assert(strlen(str)==128);
	char ch;
	for ( int i = 0 ; i < 16*8; i++ ){
		ch = str[i];
		I[i/16][i%16] = (float) ch;
	}
	//define table M
	float M[16][8]={
		{1,2,3,4,5,6,7,8},		{1,2,3,4,5,6,7,8},		{1,2,3,4,5,6,7,8},		{1,2,3,4,5,6,7,8},
		{1,2,3,4,5,6,7,8},		{1,2,3,4,5,6,7,8},		{1,2,3,4,5,6,7,8},		{1,2,3,4,5,6,7,8},
		{1,2,3,4,5,6,7,8},		{1,2,3,4,5,6,7,8},		{1,2,3,4,5,6,7,8},		{1,2,3,4,5,6,7,8},
		{1,2,3,4,5,6,7,8},		{1,2,3,4,5,6,7,8},		{1,2,3,4,5,6,7,8},		{1,2,3,4,5,6,7,8}
	};

	//in order to make M useable, just do some transform
	float It[16][8];
	for ( int i = 0 ; i < 16 ; i++ )
		for ( int j = 0 ; j < 8 ; j++ )
			It[i][j]=I[j][i];
	
	float temp[8]={0};
	float R[16][16];
	//Do the matrix multiply
	__m256 ymm0, ymm1; //define the registers used
	for ( int i = 0 ; i < 16 ; i++ )
		for ( int j = 0 ; j < 16 ; j++ )	{
			ymm0 = __builtin_ia32_loadups256(M[i]); //load the 8 floats in a into ymm0
			ymm1 = __builtin_ia32_loadups256(It[j]); //load the 8 floats in b into ymm1
			//multiply ymm0 and ymm1, store the result in ymm0
			ymm0 = __builtin_ia32_mulps256(ymm0, ymm1);
			__builtin_ia32_storeups256(temp, ymm0); //copy the 8 floats in ymm0 to c
			R[i][j]=0;
			for ( int k = 0 ; k < 8 ; k++ )
				R[i][j]+=temp[k];
			}

	for ( int i = 0 ; i < 16 ; (i++, printf("\n")) )
		for ( int j = 0 ; j < 16 ; j++ )	
			printf("%d\t", (unsigned int)R[i][j]);
}
