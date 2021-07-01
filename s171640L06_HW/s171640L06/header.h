#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#define TOLERANCE 0.000000001

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern "C"
{
	int rpoly_(double *, int *, double *, double *, long int *);
	int hybrj1_(void fcn(int *, double *, double *, double *, int *, int *), int *, double *, double *, double *, int *, double *, int *, double *, int *);
	int hybrd1_(void fcn(int *, double *, double *, int *), int *, double *, double *, double *, int *, double *, int *);
};

const double EPSILON = 0.0000001;

void(*fcn_d)(int *, double *, double *, int *);
void(*fcn_j)(int *, double *, double *, double *, int *, int *);

int     n;		// 다항식인 경우 계수의 개수, 
                // 비선형 연립 방정식인 경우 미지수 개수
// x와 fvec는 dynamic allocation을 통하여 메모리를 할당한다
double *x;		// 다항식인 경우 다항식의 계수 배열(내림차순), 크기는 n-1
                // 비선형 연립 방정식인 경우 근의 초기값, 실행 후 근의 근사값
				// 배열 크기: 다항식인 경우 n-1, 비선형 연립 방정식인 경우 n                
double *fvec;   // n x n Jacobian 배열
int     info;   // 함수 HYBRJ1과 HYBRD1의 실행 결과를 알리는 flag

#define p1 1	// problem 1
#define p2 2	// problem 2
#define p3 3	// problem 3
#define p4 4	// problem 4
#define p5 5	// problem 5

#define h1 6	// problem 6 (homework 1)
#define h2 7	// problem 7 (homework 2)

void solve_hybrj1();
void solve_hybrd1();


