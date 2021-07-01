#include "functionL05.h"

#define OUTPUT_STEPS   1	// display detailed solution finding steps(stdout)
							// must use redirection to save and see these outputs

#define BISECTION      1	// use bisection methof
#define NEWTON_RAPHSON 2	// use Newton-Raphson method
#define SECANT         3	// use Secant method(HW)
#define IMPROVED       4	// use improved method over the Newton-Raphson(HW)

void   pdfGeneration(void);					// nomalize samples to generate pdf
void   cdfGeneration(void);					// cdf generation
double interpolationF(double x, double U);	// given x, find the value of F_X(x)-U
double interpolationFD(double x);			// given x, fine the value of p_X(x)

double genRandN_Bisection(double U, int si);		// function using the bisection method
double genRandN_NewtonRaphson(double U, int si);	// function using the Newton-Raphson method	
double genRandN_Secant(double U, int si);			// function using the secant method(HW)
double genRandN_Improved(double U, int si);			// function using the improved method(HW)

int main(void) {
	int TN;				// # of tests

	srand((unsigned int)time(NULL));	// init rand()
	//srand(0);			// use this line when debugging
	scanf("%d", &TN);	// read # of tests		

	for (int t = 0; t < TN; ++t) {
		start_time = clock();				// start time measure

		// preparation
		getParameters_openFiles();			// get parameters and open files
		fscanf(sfp, "%d %lf", &SN, &dx);	// read # of samples and x-distance between two adjacent samples
		allocateMemory();
		readSamples();
		pdfGeneration();
		cdfGeneration();
		save_pdf_cdf();

		// start generation
		fprintf(Xfp, "%d\n", XN);			// write # of random numbers generated
		for (int d = 0; d < XN; d++) {
			double U = (double)rand() / RAND_MAX;	// generate U : [0.0,1.0]
			double X;			// random number generated

			// generate a random number
			switch (method) {
				case (BISECTION):
					X = genRandN_Bisection(U, d);	// d is needed to display steps 
					fflush(stdout);		// can be removed(just for debugging)
					break;
				case (NEWTON_RAPHSON):	
					X = genRandN_NewtonRaphson(U, d);
					fflush(stdout);		// "
					break;
				case (SECANT):	
					X = genRandN_Secant(U, d);
					fflush(stdout);		// "
					break;
				case (IMPROVED):
					X = genRandN_Improved(U, d);
					fflush(stdout);		// "
					break;
				default:
					printf("Unknown method");
					exit(-1);
			}
			fprintf(Xfp, "%.12f\n", X);		// save X

			// find the proper histogram bin for X and increase the bin by one
			
			// *** histoGram[ ]을 설정하는 이 부분을 작성하세요
			//     구한 X 값으로부터 histoGram[] 배열의 index i를 구한후
			//     histoGram[i]=histoGram[i]+1해주면 됩니다.
			int i;
			i = (int)(X / hd);
			if (i > HN - 1) i = HN - 1;
			if (i < 0) i = 0;
			histoGram[i] = histoGram[i] + 1;

		}
		end_time = clock();					// stop time measure
		saveHistogram();					// write histogram to the file
		closeFile_deallocateMemory();		// deallocate memory
	}
}

double genRandN_Bisection(double U, int si) {
	// initial interval is [0.0, 1.0] (can always find a root)
	double L = 0.0, R = 1.0;
	double x1 = (L + R) / 2;	// you may change the name of variable x1 if you want

	// *** 이 함수를 작성하세요
	//     이 함수는 근이 반드시 존재합니다
	int iter = 0;
	double x0;

	if (showSteps == OUTPUT_STEPS) {
			printf(" n(%6d)    xn1         |f(xn1)|  U = %.4lf\n", si, U);
	}

	for (iter = 0; iter <= maxIter; iter++) {
		if (showSteps == OUTPUT_STEPS) {
				printf("%2d  %.12e  %.16e\n", iter, x1, fabs(interpolationF(x1, U)));
		}

		if (fabs(interpolationF(x1, U)) < delta)
			break;
		if (iter != 0 && fabs(x1 - x0) < epsilon)
			break;

		x0 = x1;
		if (interpolationF(x1, U) * interpolationF(L, U) < 0)
			R = x0;
		else
			L = x0;
		x1 = (L + R) / 2;
	}

	return x1;		// you can change the return variable if you want
}

double genRandN_NewtonRaphson(double U, int si) {
	double L = 0.0, R = 1.0;
	double x1 = (L + R) / 2;	// just for initial buid without coding

	// *** 이 함수를 작성하세요
	//     초기값이 필요한데 이를 어찌 설정할 지 생각합시다.
	//     pdf에 따라 발산할 수도 있습니다(가능한 이를 피할 수 있도록 초기값을 정합시다)

	int iter = 0;
	double x0;

	x0 = x1;
	if (interpolationF(x1, U) * interpolationF(L, U) < 0)
		R = x0;
	else
		L = x0;
	x1 = (L + R) / 2;

	if (showSteps == OUTPUT_STEPS) {
		printf(" n(%6d)    xn1         |f(xn1)|  U = %.4lf\n", si, U);
	}

	for (iter = 0; iter <= maxIter; iter++) {
		if (showSteps == OUTPUT_STEPS) {
			printf("%2d  %.12e  %.16e\n", iter, x1, fabs(interpolationF(x1, U)));
		}

		if (fabs(interpolationF(x1, U)) < delta)
			break;
		if (iter != 0 && fabs(x1 - x0) < epsilon)
			break;

		x0 = x1;
		x1 = x1 - interpolationF(x1, U) / interpolationFD(x1);

		if (x1 > 1)
			x1 = 1;
		else if (x1 < 0)
			x1 = 0;
	}

	return x1;	// you can change the return variable if you want
}

double genRandN_Secant(double U, int si) {
	double x1=0;		// these variables are for initial build without coding
	//double x0;
							// you may change the names or delete them if needed

	// *** 이 함수를 작성하세요
	//     숙제입니다
	//     이 함수 역시 초기값 두 개가 필요합니다. 어찌 설정할지 생각해 보세요.
	//     pdf에 따라 발산할 수도 있습니다(가능한 이를 피할 수 있도록 초기값을 정합시다)

	return x1;			
}

double genRandN_Improved(double U, int si) {
	double X = 0;			// these variables are for initial build without coding
							// you may change the names or delete them if needed

	// *** 이 함수를 작성하세요
	//     숙제입니다.
	//     Bisection과 Newton-Raphson을 적절히 조합하여 어떤 경우에도 발산하지 
	//     않는 함수를 만들어 봅시다.
	//     필요하다면, 이 함수에서 호출하는 다른 함수를 작성하여 호출해도 좋습니다.

	return X;
}

void pdfGeneration(void) {
	// input pdfX[], pdfY[], SN
	// output pdfX[] (scale so that the range is 0 ~ 1)
	//        pdfY[] normalization

	// *** 이 함수를 작성하세요
	double total = 0;
	for (int i = 0; i < SN; i++) {
		total += pdfY[i];
	}

	double init_x = pdfX[0];
	for (int i = 0; i < SN; i++) {
		pdfX[i] = (pdfX[i] - init_x) / (pdfX[SN - 1] - init_x);
	}

	dx = pdfX[1] - pdfX[0];
	double init_y = pdfY[0];
	for (int i = 0; i < SN; i++){
		pdfY[i] = pdfY[i] / ((2 * total - pdfY[SN - 1] - init_y) / 2 * dx);
	}
}

void cdfGeneration(void) {
	// input:  normalized pdfX[], pdfY[], SN
	// output: cdfY[]

	// *** 이 함수를 작성하세요
	cdfY[0] = 0;
	for (int i = 1; i < SN; i++) {
		cdfY[i] = cdfY[i - 1] + (pdfY[i] + pdfY[i - 1]) * (dx / 2);
	}
}

double interpolationF(double x, double U) {
	// return the value of F_X(x) - U by interpolation
	double F = 0;		// just for building before coding

	// *** 이 함수를 작성하세요
	int i = (int)(x / dx);

	if (i < 0) i = 0;
	if (i > SN - 2) i = SN - 2;
	F = cdfY[i] + (cdfY[i + 1] - cdfY[i]) * ((x - pdfX[i]) / (pdfX[i + 1] - pdfX[i]));

	F -= U;
	return F;	// you may modify the variable name
}

double interpolationFD(double x) {
	// return the value of p_X(x) by interpolation
	double FD = 0;		// just for building before coding

	// *** 이 함수를 작성하세요
	int i = (int)(x / dx);

	if (i < 0) i = 0;
	if (i > SN - 2) i = SN - 2;

	FD = pdfY[i] + (pdfY[i + 1] - pdfY[i]) * ((x - pdfX[i]) / (pdfX[i + 1] - pdfX[i]));

	return FD;	// you may modify the variable name
}
