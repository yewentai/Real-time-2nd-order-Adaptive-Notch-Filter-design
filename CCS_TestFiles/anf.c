#include "anf.h"

int anf(int y, int *s, int *a, int *rho, unsigned int* index)
{
    /*
     y in Q? : newly captured sample
     s in Q? : x[3] databuffer - Hint: Reserve a sufficiently number of integer bits such that summing intermediate values does not cause overflow (so no shift is needed after summing numbers)
     a in Q? : the adaptive coefficient
     e in Q? : output signal
     rho in Q? : fixed {rho, rho^2} or variable {rho, rho_inf} pole radius
     index : points to (t-1) sample (t current time index) in s -> circular buffer
     */
    // Define lambda as a fixed-point constant, replace 0.6 with the actual lambda value in floating-point
    // If rho is not squared beforehand, square it and normalize back to Q31
    const int rho_square = (long)rho * (long)rho >> Q_FORMAT;

    int e; // Error signal
    long temp; // Temporary variable for intermediate calculations

    // Update the buffer index for the circular buffer implementation
    int k = *index;
    k = (k - 1 + 3) % 3; // Assuming a buffer size of 3

    // ANF-LMS Algorithm implementation
    // Step 2: Update s(m) = y(m) + rho(m)a(m - 1)s(m - 1) - rho(m)^2 s(m - 2)
    temp = ((long)rho * a[k] >> Q_FORMAT) * s[(k + 1) % 3]; // rho*a(m-1)*s(m-1) in Q31
    s[k] = y + temp - (rho_square * s[(k + 2) % 3] >> Q_FORMAT); // Add y(m) and subtract rho^2*s(m-2)

    // Step 4: Compute the error signal e(m) = s(m) - a(m - 1)s(m - 1) + s(m - 2)
    e = s[k] - ((a[(k + 1) % 3] * s[(k + 1) % 3]) >> Q_FORMAT) + s[(k + 2) % 3];

    // Step 5: Update the adaptive coefficient a(m) = a(m - 1) + 2*mu*e(m)s(m - 1)
    a[k] = a[(k + 1) % 3] + ((long)mu * e >> Q_FORMAT) * s[(k + 1) % 3];

    // Ensure a(t) is within the valid range to keep zeros on the unit circle
    if (a[k] >= (1 << Q_FORMAT) || a[k] < -(1 << Q_FORMAT)) {
        a[k] = a[(k + 1) % 3]; // If out of range, revert to the previous value
    }

    // Update the index for the next iteration
    *index = k;

    return e; // Return the error signal
}
