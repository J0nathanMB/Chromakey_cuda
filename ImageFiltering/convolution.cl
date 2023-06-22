//__kernel void convolute(__global float* a,
//    __global float* b,
//    __global float* c,
//    const unsigned int count
//    const unsigned int fcount) {
//
//    int i = get_global_id(0);
//    int j = get_global_id(1);
//    
//    for (int k = 0; k < count; k++) {
//        c[i * k * 3 + j] = a[i * k * 3 + j];
//    }
//
//}

float min_max(float value) {
    if (value > 255)
        value = 255;
    else if (value < 0)
        value = 0;
    return value;
}

__kernel void convolute(__global float* inputImage, __global float* outputImage, __global float* filter, int width, int height ,float bias, float factor)
{
    int x = get_global_id(0);
    int y = get_global_id(1);

    float sumR = 0;
    float sumG = 0;
    float sumB = 0;
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            if ( (x + i >= 0) &&
                 (x + i < width) &&
                 (y + j >= 0) &&
                 (y + j < height)) {
                sumR += inputImage[(y + j) * width * 3 + (x + i) * 3] * filter[(j + 1) * 3 + (i + 1)];
                sumG += inputImage[(y + j) * width * 3 + (x + i) * 3 + 1] * filter[(j + 1) * 3 + (i + 1)];
                sumB += inputImage[(y + j) * width * 3 + (x + i) * 3 + 2] * filter[(j + 1) * 3 + (i + 1)];
            }

    sumR = factor * sumR + bias;
    sumG = factor * sumG + bias;
    sumB = factor * sumB + bias;

    outputImage[y * width * 3 + x * 3] = min_max(sumR);
    outputImage[y * width * 3 + x * 3 + 1] = min_max(sumG);
    outputImage[y * width * 3 + x * 3 + 2] = min_max(sumB);
}