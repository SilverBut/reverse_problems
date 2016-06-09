/*
 * For more reference about usage of AVX instructions in C,
 * see Intel SDM.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <immintrin.h>

int main(void){
	//Get keys
	float I[8][8];
	char str[8*8+1];
	printf("Show me your slogan:\n");
	read(1, str, 65);
	str[64]=0;
	char ch;
	for ( int i = 0 ; i < 8*8; i++ ){
		ch = str[i];
		I[i/8][i%8] = (float) ch;
	}
	//define table M
	int M[8][8]={
	{1,  2,  3,  4,  5,  6,  7,  8},
	{28, 29, 30, 31, 32, 33, 34, 9}, 
	{27, 48, 49, 50, 51, 52, 35, 10},
	{26, 47, 60, 61, 62, 53, 36, 11},
	{25, 46, 59, 64, 63, 54, 37, 12},
	{24, 45, 58, 57, 56, 55, 38, 13},
	{23, 44, 43, 42, 41, 40, 39, 14},
	{22, 21, 20, 19, 18, 17, 16, 15}
	};

	//Do the matrix multiply
	//In the real program we need to make sure:
	//I*X=M*10000
	//To calc out X, we need let
	//X=10000*inv(I)*M
	//The flag, which is supposed to be I, is
	//s = "Octave is your good friend. And your flag is:cpctf{yNEvCsbaiy6D}"
	//{{79,99,116,97,118,101,32,105},
	//{115,32,121,111,117,114,32,103},
	//{111,111,100,32,102,114,105,101},
	//{110,100,46,32,65,110,100,32},
	//{121,111,117,114,32,102,108,97},
	//{103,32,105,115,58,99,112,99},
	//{116,102,123,121,78,69,118,67},
	//{115,98,97,105,121,54,68,125}}
	//And M is listed above, so X should be
	//X=M*I'=:
	float X[8][8]=  {
	{  5587.7698 ,  4997.9490  , 4449.5170 ,  4976.6199 ,  4953.1235 ,  4003.2397 ,  4272.6035  ,  379.0892},
 { -1558.8325 , -1741.6827  ,-1507.6634 , -1203.9928 , -1198.5585 , -1882.8237 , -1814.9850  ,   85.6500},
 {  1094.8493 ,  2283.8758  , -604.6258 ,  -331.5853 ,  -241.8573 ,  1189.0558 ,  2754.6992  , -796.6230},
 { -1973.4429 , -2663.7833  ,-1020.5978 , -1253.2978 , -1399.7474 , -2143.4816 , -1703.1346  ,  651.4668},
 {  -775.0505 , -1371.8768  ,-2332.5517 , -3109.6597 , -3010.1081 , -2119.3774 ,  -864.9422  ,  381.7449},
 { -1102.6262 ,  -293.0515  , 1829.8255 ,  2048.6931 ,  2253.0793 ,  1671.9980 ,    31.6349  , -171.2145},
 {  -125.3235 ,  2230.6386  , 2720.4889 ,  2092.7069 ,  1969.8384 ,  2447.3727 ,  1060.6120  ,  625.3633},
 {   -55.7751 ,  -846.2359  ,    2.4238 ,   182.2030 ,    55.1276 ,    28.8538 , -1688.2357  ,  219.2737}
	};
	
	//in order to make X useable, just do some transform
	float Xt[8][8];
	for ( int i = 0 ; i < 8 ; i++ )
		for ( int j = 0 ; j < 8 ; j++ )
			Xt[i][j]=X[j][i];
	
	float temp[8]={0};
	float sum;
	__m256 ymm0, ymm1;
	for ( int i = 0 ; i < 8 ; i++ )
		for ( int j = 0 ; j < 8 ; j++ )	{
			ymm0 = __builtin_ia32_loadups256(I[i]); //load the 8 floats in a into ymm0
			ymm1 = __builtin_ia32_loadups256(Xt[j]); //load the 8 floats in b into ymm1
			//multiply ymm0 and ymm1, store the result in ymm0
			ymm0 = __builtin_ia32_mulps256(ymm0, ymm1);
			__builtin_ia32_storeups256(temp, ymm0); //copy the 8 floats in ymm0 to c
			sum = 0;
			for ( int k = 0 ; k < 8 ; k++ )
				sum+=temp[k];
			if ( M[i][j]*10000-(int)sum > 1 || (int)sum-M[i][j]*10000 > 1 ){
				printf("Sorry.\n");
				exit(-1);
			}
		}
	printf("Excellent!\n");

	return 0;
}
