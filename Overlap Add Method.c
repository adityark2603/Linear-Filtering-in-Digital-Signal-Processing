#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to perform convolution of two signals
void convolution(float *input, int input_length, float *filter, int filter_length, float *output) {
    int output_length = input_length + filter_length - 1;
    memset(output, 0, output_length * sizeof(float));

    for (int i = 0; i < input_length; i++) {
        for (int j = 0; j < filter_length; j++) {
            output[i + j] += input[i] * filter[j];
        }
    }
}

// Function to perform overlap-add method
void overlap_add(float *input, int input_length, float *filter, int filter_length, int block_size, float *output) {
    int output_length = input_length + filter_length - 1;
    memset(output, 0, output_length * sizeof(float));

    int num_blocks = (input_length + block_size - 1) / block_size;

    for (int i = 0; i < num_blocks; i++) {
        int start = i * block_size;
        int end = (start + block_size > input_length) ? input_length : start + block_size;
        int block_length = end - start;

        float *block = (float *)malloc((block_length + filter_length - 1) * sizeof(float));
        convolution(&input[start], block_length, filter, filter_length, block);

        for (int j = 0; j < block_length + filter_length - 1; j++) {
            output[start + j] += block[j];
        }

        free(block);
    }
}

int main() {
    // Example input signal
    float input[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int input_length = sizeof(input) / sizeof(input[0]);

    // Example FIR filter coefficients
    float filter[] = {1, 0.5, 0.25};
    int filter_length = sizeof(filter) / sizeof(filter[0]);

    // Block size for overlap-add method
    int block_size = 4;

    // Output signal
    int output_length = input_length + filter_length - 1;
    float *output = (float *)malloc(output_length * sizeof(float));

    // Perform overlap-add method
    overlap_add(input, input_length, filter, filter_length, block_size, output);

    // Print the output signal
    printf("Output signal after overlap-add method:\n");
    for (int i = 0; i < output_length; i++) {
        printf("%f ", output[i]);
    }
    printf("\n");

    free(output);
    return 0;
}
