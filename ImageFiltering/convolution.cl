float min_max(float value) {
    if (value > 255)
        value = 255;
    else if (value < 0)
        value = 0;
    return value;
}

__kernel void convolute(__global float* inputImage1, __global float* inputImage2, __global float* outputImage, __global float* filter, int width, int height ,float bias, float factor)
{
    int x = get_global_id(0);
    int y = get_global_id(1);

    float sumR = 0;
    float sumG = 0;
    float sumB = 0;

    double cR = 40.0;
    double cG = 250.0;
    double cB = 20.0;

    float tolerance = 100.0;
    
    
    if ( (x >= 0) &&
         (x < width) &&
         (y >= 0) &&
         (y < height)) {

        double pR = inputImage1[x * 3 + y * width * 3 + 0];
        double pG = inputImage1[x * 3 + y * width * 3 + 1];
        double pB = inputImage1[x * 3 + y * width * 3 + 2];     

        long d = (sqrt(pow((cR - pR), 2) + pow((cG - pG), 2) + pow((cB - pB), 2)));

        if (d > -136352428 || d < -136352429) {
            printf(" %g", d);
        }
        

        if (d <= tolerance) {
            sumR += inputImage2[x * 3 + y * width * 3 + 0];
            sumG += inputImage2[x * 3 + y * width * 3 + 1];
            sumB += inputImage2[x * 3 + y * width * 3 + 2];
        }
        else {
            sumR += pR;
            sumG += pG;
            sumB += pB;
        }
    }

    outputImage[y * width * 3 + x * 3] = min_max(sumR);
    outputImage[y * width * 3 + x * 3 + 1] = min_max(sumG);
    outputImage[y * width * 3 + x * 3 + 2] = min_max(sumB);
}