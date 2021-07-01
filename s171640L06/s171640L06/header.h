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

int     n;		// ���׽��� ��� ����� ����, 
                // ���� ���� �������� ��� ������ ����
// x�� fvec�� dynamic allocation�� ���Ͽ� �޸𸮸� �Ҵ��Ѵ�
double *x;		// ���׽��� ��� ���׽��� ��� �迭(��������), ũ��� n-1
                // ���� ���� �������� ��� ���� �ʱⰪ, ���� �� ���� �ٻ簪
				// �迭 ũ��: ���׽��� ��� n-1, ���� ���� �������� ��� n                
double *fvec;   // n x n Jacobian �迭
int     info;   // �Լ� HYBRJ1�� HYBRD1�� ���� ����� �˸��� flag

#define p1 1	// problem 1
#define p2 2	// problem 2
#define p3 3	// problem 3
#define p4 4	// problem 4
#define p5 5	// problem 5

#define h1 6	// problem 6 (homework 1)
#define h2 7	// problem 7 (homework 2)

void solve_hybrj1();
void solve_hybrd1();


