#ifndef HFTMATH_H
#define HFTMATH_H

#include "strategyanalysismod_global.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <math.h>

#define DBL_MAX 1.7976931348623158e+308 /* max value */
#define DBL_MIN 2.2250738585072014e-308 /* min positive value */
#define PI 3.1415926535898

#define EIGEN_DUMP(vn, v) do { \
    std::stringstream s; \
    s << v; \
    qDebug() << vn << ": "; \
    qDebug() << s.str().c_str(); \
} while(0)

#define EIGEN_DUMP_VAR_FILE(fn, vn, v) do { \
    std::ofstream f(fn); \
    f << vn << ": \n"; \
    f << v; \
} while(0)


/*return: average value of <result>*/
double  SAM_EXPORT VectorSubstract(QVector<double>& s1, QVector<double>& s2, QVector<double>& result, double* min = NULL, double* max = NULL);
double  SAM_EXPORT VectorDiv(QVector<double>& s1, QVector<double>& s2, QVector<double>& result,
                double max_limt = DBL_MAX, double min_limit= DBL_MIN);
void    SAM_EXPORT VectorMinMaxElm(QVector<double>& v, double* min, double* max);

int     SAM_EXPORT EmuNormalizeDistribution(double u, double std_devi,
                    QVector<double>& s, double xmin, double xmax, double ymax, int samples=1000);

double  SAM_EXPORT VectorExpect(QVector<double>& v);
double  SAM_EXPORT VectorExpect(QVector<double>& v, int startPos, int endPos);
double  SAM_EXPORT VectorStdDeviation(QVector<double>& s, double expect);
double  SAM_EXPORT VectorSkew(QVector<double>& v, double expect, double stddev);
double  SAM_EXPORT VectorKurt(QVector<double>& v, double expect, double stddev);

#endif // HFTMATH_H
