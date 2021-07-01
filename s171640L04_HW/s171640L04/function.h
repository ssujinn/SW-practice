#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

//#define DELTA 0.000001
//#define Nmax 50
//#define EPSILON 0.00001

#define OUTPUT_STEPS	  1
#define CONVERGENCE_CHECK 1

// 파라메터는 최대 3 개까지이다(변경가능)
#define PARAMETER_N		3
char pidx[PARAMETER_N+1] = "abc";

#define NEWTON_R				1
#define SECANT					2
#define BISECTION				3
// 새로운 풀이 방법을 추가하면 여기에 그 방법의 id를 부여한다

const char *NRstr = "Newton-Raphson method";
const char *SCstr = "Secant method";
const char *BSstr = "Bisection method";
// 새로운 풀이 방법을 추가하면 여기에 그 방법을 설명하는 문자열을 포함한다


double(*_F)(double, double, double, double);
double(*_FP)(double, double, double, double);

//f1 = x^2 -4x +4 -lnx = 0
#define PROBLEM1 1
const char *LabF1str = "f1(x) = x^2 -ax +4 -ln(bx) = 0";
double _LabF1(double x, double a, double b, double c) {
	return x*x - a*x + 4 - log(b*x);
}
double _LabFP1(double x, double a, double b, double c) {
	return 2*x - a - (1/x);
}

// 문제가 추가될 때 마다 위 문제 1과 같은 내용이 추가되어야 합니다.
// 두번째 문제라면 
// #define PROBLEM2 2, LabF2str, _LabF2, _LabFP2 등 추가

//f2 = x^2 -2 = 0
#define PROBLEM2 2
const char *LabF2str = "f2(x) = x^2 -2 = 0";
double _LabF2(double x, double a, double b, double c) {
	return x * x - 2;
}
double _LabFP2(double x, double a, double b, double c) {
	return 2 * x;
}

//f3 = (a^4 +2a^2 +1)x^4 +(4a^3b +4ab +3a^2 -1)x^3 +(6a^2b^2 +6ab +2b^2)x^2 +(4ab^3 +3b^2)x +b^4 = 0
#define PROBLEM3 3
const char *LabF3str = "f3(x) = (a^4 +2a^2 +1)x^4 +(4a^3b +4ab +3a^2 -1)x^3 +(6a^2b^2 +6ab +2b^2)x^2 +(4ab^3 +3b^2)x +b^4 = 0";
double _LabF3(double x, double a, double b, double c) {
	return (a*a*a*a + 2 * a*a + 1)*x*x*x*x + (4 * a*a*a*b + 4 * a*b + 3 * a*a - 1)*x*x*x + (6 * a*a*b*b + 6 * a*b + 2 * b*b)*x*x + (4 * a*b*b*b + 3 * b*b)*x + b * b*b*b;
}
double _LabFP3(double x, double a, double b, double c) {
	return 4 * (a*a*a*a + 2 * a*a + 1)*x*x*x + 3 * (4 * a*a*a*b + 4 * a*b + 3 * a*a - 1)*x*x + 2 * (6 * a*a*b*b + 6 * a*b + 2 * b*b)*x + (4 * a*b*b*b + 3 * b*b);

}

//f4 = lnx - 1 = 0
#define PROBLEM4 4
const char *LabF4str = "f4(x) = lnx - 1 = 0";
double _LabF4(double x, double a, double b, double c) {
	return log(x) - 1;
}
double _LabFP4(double x, double a, double b, double c) {
	return 1 / x;
}

//f5 = M(1-1/(1+r)^k) - Wr = 0
#define PROBLEM5 5
const char *LabF5str = "f5(x) = M(1-1/(1+r)^k) - Wr = 0";
double _LabF5(double x, double a, double b, double c) {
	return b * (1 - 1 / pow((1 + x), c)) - a * x;
}
double _LabFP5(double x, double a, double b, double c) {
	return c * b * (1 / pow((1 + x), c + 1)) - a;
}