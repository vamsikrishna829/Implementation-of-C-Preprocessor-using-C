#include "sample_header.h"

#define PI 3.14

#define SQUARE(x) ((x) * (x))

int main(void)
{
    int number = 5;

    float area = PI * SQUARE(number);

    int result = HEADER_VALUE + SQUARE(number);

    return result + (int)area;
}
