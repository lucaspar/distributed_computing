// pgcc -acc -Minfo=accel -fast -ta=tesla -o jacobi_parallel jacobi_parallel.c && echo "Compiled Successfully"
// ./jacobi_parallel
#include <math.h>
#include <string.h>
#include "timer.h"

#define NN 4096
#define NM 4096

float A[NN][NM];
float Anew[NN][NM];

int main(int argc, char** argv)
{
    const int n = NN;
    const int m = NM;
    const int iter_max = 1000;

    const double tol = 1.0e-6;
    double error     = 1.0;

    memset(A, 0, n * m * sizeof(float));
    memset(Anew, 0, n * m * sizeof(float));

    for (int j = 0; j < n; j++)
    {
        A[j][0]    = 1.0;
        Anew[j][0] = 1.0;
    }

    printf("Jacobi relaxation Calculation: %d x %d mesh\n", n, m);

    StartTimer();
    int iter = 0;

    #pragma acc data copy(A), create(Anew)
    while ( error > tol && iter < iter_max )
    {
        #pragma acc kernels
        {
            error = 0.0;

            for( int j = 1; j < n-1; j++)
            {
                #pragma acc loop device_type(nvidia) gang(8) vector(32)
                for( int i = 1; i < m-1; i++ )
                {
                    Anew[j][i] = 0.25 * ( A[j][i+1] + A[j][i-1]
                                        + A[j-1][i] + A[j+1][i]);
                    error = fmax( error, fabs(Anew[j][i] - A[j][i]));
                }
            }

            for( int j = 1; j < n-1; j++)
            {
                #pragma acc loop device_type(nvidia) gang(8) vector(32)
                for( int i = 1; i < m-1; i++ )
                {
                    A[j][i] = Anew[j][i];
                }
            }
        }

        if(iter % 100 == 0) printf("%5d, %0.6f\n", iter, error);

        iter++;

    }

    double runtime = GetTimer();

    printf(" total: %f s\n", runtime / 1000);

    return 0;
}
