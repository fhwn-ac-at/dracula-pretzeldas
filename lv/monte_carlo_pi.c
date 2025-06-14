// Benjamin Eder, 210894

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Point {
    float x;
    float y;
} Point;

Point random_point() {
    float x = 1 - 2 * drand48();
    float y = 1 - 2 * drand48();

    return (Point){.x = x, .y = y};
}

bool is_point_in_unit_circle(Point p) {
    return ((p.x * p.x) + (p.y * p.y)) < 1;
}

Point random_point_in_unit_circle() {
    Point p;
    do {
        p = random_point();
    } while (!is_point_in_unit_circle(p));

    return p;
}

float approximate_pi(const size_t num_samples) {
    float approx = 0.f;
    size_t inside = 0;
    for (size_t i = 0; i < num_samples + 1; i++) {
        Point p = random_point();
        if (is_point_in_unit_circle(p)) {
            inside++;
        }

        // debug output
        if (i % (num_samples / 1000) == 0) {
            approx = 4.0f * (float)inside / (float)i;
            printf("\rProgress at %zu/%zu (%.2f%%): inside: %zu, approx: %.8f",
                   i, num_samples, i / (float)num_samples * 100, inside,
                   approx);
            fflush(stdout);
        }
    }
    printf("\n");
    approx = 4.0f * (float)inside / (float)num_samples;
    return approx;
}

int main(void) {
    srand48(time(NULL));

    Point p = random_point();

    if (is_point_in_unit_circle(p)) {
        printf("Point{x: %.2f, y: %.2f} is in unit circle\n", p.x, p.y);
    } else {
        printf("Point{x: %.2f, y: %.2f} is in not unit circle\n", p.x, p.y);
    }

    const size_t NUM_SAMPLES = 100000;

    float approx_pi = approximate_pi(NUM_SAMPLES);

    printf("PI is approximately %.8f!\n", approx_pi);
}