#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <stdlib.h>

#define N 8   // FFT Size
#define L 4   // Length of each segment
#define M 4   // Filter length (L + M - 1 = N)

// Function to perform FFT using Cooley-Tukey Algorithm
void fft(complex double *x, int n) {
    if (n <= 1) return;

    // Divide even and odd indices
    complex double even[n/2], odd[n/2];
    for (int i = 0; i < n/2; i++) {
        even[i] = x[i*2];
        odd[i] = x[i*2 + 1];
    }

    // Recursive FFT
    fft(even, n/2);
    fft(odd, n/2);

    for (int k = 0; k < n/2; k++) {
        complex double t = cexp(-I * 2 * M_PI * k / n) * odd[k];
        x[k] = even[k] + t;
        x[k + n/2] = even[k] - t;
    }
}

// Function to perform Inverse FFT
void ifft(complex double *x, int n) {
    for (int i = 0; i < n; i++) x[i] = conj(x[i]);
    fft(x, n);
    for (int i = 0; i < n; i++) x[i] = conj(x[i]) / n;
}

// Overlap-Add Convolution
void overlap_add(double *x, int x_len, double *h, int h_len) {
    int seg_count = (x_len + L - 1) / L;
    double y[x_len + M - 1];
    for (int i = 0; i < x_len + M - 1; i++) y[i] = 0;

    complex double X[N], H[N], Y[N];

    // Zero-padding and computing FFT of filter
    for (int i = 0; i < M; i++) H[i] = h[i];
    for (int i = M; i < N; i++) H[i] = 0;
    fft(H, N);

    for (int s = 0; s < seg_count; s++) {
        // Load segment with zero-padding
        for (int i = 0; i < N; i++) X[i] = (s * L + i < x_len) ? x[s * L + i] : 0;
        
        fft(X, N);

        // Perform multiplication in frequency domain
        for (int i = 0; i < N; i++) Y[i] = X[i] * H[i];

        ifft(Y, N);

        // Add valid part to the output
        for (int i = 0; i < N; i++) y[s * L + i] += creal(Y[i]);
    }

    // Print output sequence
    printf("Output signal after overlap-save method:: \n");
    for (int i = 0; i < x_len + M - 1; i++) printf("%.2f ", y[i]);
    printf("\n");
}

int main() {
    double x[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}; // Input signal
    double h[] = {0.25, 0.5, 0.25, 0}; // Filter coefficients
    int x_len = sizeof(x) / sizeof(x[0]);
    int h_len = sizeof(h) / sizeof(h[0]);
    
    overlap_add(x, x_len, h, h_len);
    
    return 0;
}
