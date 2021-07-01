#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <memory.h>
#include <time.h>

// definition for input samples, pdf, cdf
int     SN;			 // # of input samples(in this lab SN = 100)
double  dx;			 // interval between two adjacent samples(dx = xi-x(i-1))
                     // later it is changed to the interval between two in the normalized pdf
double *pdfX, *pdfY; // (x,y) values in the input samples
                     // later normalized pdf array (x,y)

double *cdfY;		 // cdf sequence array(size is SN and x-values are the same as pdfX array)

// definition for histogram
int     HN;			 // histogram array size(in this lab HN = 100)
double  histoMax;	 // maximum value of histogram(1.0 in this lab)
double  hd;			 // difference between the two adjacent bins
int    *histoGram;	 // histogram array of size HN

// root finding for F_X(x)=U
int		method;			// Generation method(1: bisection, 2: Newton-Raphson,
						//                   3: Secant(HW),    4: Improved method(HW)
double  epsilon, delta;	// error criteria(the same meaning as in Lab04)
int     maxIter;        // max # of iterations(  "                    )
int     showSteps;		// show intermediate steps if 1 (  "          )

// variables for generation time checking
clock_t start_time, end_time;

// number of random numbers to be generated
int XN;     // # of random numbers to be generated

// file pointers
FILE *sfp;			// input sample file pointer
FILE *pfp, *cfp;	// normalized pdf and cdf output file pointers
FILE *Xfp;			// output file pointer for random data generated
FILE *Hfp;			// output file pointer for histogram

// Read parameters and open files
// parameters are read from stdin by redirection
void getParameters_openFiles(void) {
	char sampledFName[128];		// sampled seq filename
	char pdfFname[128];
	char cdfFname[128];
	char randomXFName[128];		// generated random seq filename
	char histogramFname[128];	// histogram filename
	scanf("%d %d %lf", &XN, &HN, &histoMax);	// read #ofRNs, histogram_size, histo max
	hd = histoMax / HN;
	// read method, maxIter, epsilon, delta, showSteps
	scanf("%d %d %lf %lf %d", &method, &maxIter, &epsilon, &delta, &showSteps);
	scanf("%s", sampledFName);
	scanf("%s", pdfFname);
	scanf("%s", cdfFname);
	scanf("%s", randomXFName);
	scanf("%s", histogramFname);
	sfp = (FILE *)fopen(sampledFName, "r");
	if (sfp == NULL) {
		printf("Sampled seq file %s not found", sampledFName);
		exit(-1);
	}
	pfp = (FILE *)fopen(pdfFname, "w");
	cfp = (FILE *)fopen(cdfFname, "w");
	Xfp = (FILE *)fopen(randomXFName, "w");
	Hfp = (FILE *)fopen(histogramFname, "w");
	if (Xfp == NULL || Hfp == NULL || pfp == NULL || cfp == NULL) {
		printf("Output file open error");
		exit(-1);
	}
}

// read input sampled file
void readSamples(void) {
	for (int s = 0; s < SN; s++) {  // read samples
		fscanf(sfp, "%lf %lf", &(pdfX[s]), &(pdfY[s]));
	}
}

// memory allocation function
void allocateMemory(void) {
	pdfX = new double[SN];
	pdfY = new double[SN];
	cdfY = new double[SN];
	histoGram = new int[HN];
	if (pdfX == NULL || pdfY == NULL || cdfY == NULL || histoGram == NULL) {
		printf("Sampled sequence storage allocation error");
		exit(-1);
	}
	// init histoGram
	memset(histoGram, 0, HN * sizeof(int));
}

// deallocate memory and close files
void closeFile_deallocateMemory(void) {
	fclose(sfp);
	fclose(pfp);
	fclose(cfp);
	fclose(Xfp);
	fclose(Hfp);
	delete[] pdfX;
	delete[] pdfY;
	delete[] cdfY;
	delete[] histoGram;
}

// save histogram
void saveHistogram(void) {
	double halfDelta = hd / 2;
	double elapsed = (double)(end_time - start_time);
	fprintf(Hfp, "%d\t%f\t%d\t%.2f ms\n", SN, hd, XN, elapsed);
	for (int h = 0; h < HN; h++) {
		fprintf(Hfp, "%.6f\t %d\n", halfDelta, histoGram[h]);
		halfDelta += hd;
	}
}

// save pdf and cdf
void save_pdf_cdf(void) {
	// save pdf to File
	fprintf(pfp, "%d\t%f\n", SN, dx);
	for (int s = 0; s < SN; s++) {
		fprintf(pfp, "%f\t%f\n", pdfX[s], pdfY[s]);
	}
	// save cdf to File
	fprintf(cfp, "%d\t%f\n", SN, dx);
	for (int s = 0; s < SN; s++) {
		fprintf(cfp, "%f\t%f\n", pdfX[s], cdfY[s]);
	}
}