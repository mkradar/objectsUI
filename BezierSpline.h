#ifndef BEZIERSPLINE_H
#define BEZIERSPLINE_H

#include "LineVertex.h"

/// <summary>
/// Bezier Spline methods
/// </summary>
class BezierSpline
{
    /// <summary>
    /// Get open-ended Bezier Spline Control Points.
    /// </summary>
    /// <param name="knots">Input Knot Bezier spline points.</param>
    /// <param name="firstControlPoints">Output First Control points
    /// array of knots.Length - 1 length.</param>
    /// <param name="secondControlPoints">Output Second Control points
    /// array of knots.Length - 1 length.</param>
    /// <exception cref="ArgumentNullException"><paramref name="knots"/>
    /// parameter must be not null.</exception>
    /// <exception cref="ArgumentException"><paramref name="knots"/>
    /// array must contain at least two points.</exception>
public:
    static void GetCurveControlPoints(LineVertex* knots, LineVertex* firstControlPoints, LineVertex* secondControlPoints, int size)
    {
        int n = size - 1;

        if(n < 3)
        {
            return;
        }

        // Calculate first Bezier control points
        // Right hand side vector
        double rhs[n];

        // Set right hand side X values
        for (int i = 1; i < n - 1; ++i)
        {
            rhs[i] = 4 * knots[i].x + 2 * knots[i + 1].x;
        }
        rhs[0] = knots[0].x + 2 * knots[1].x;
        rhs[n - 1] = (8 * knots[n - 1].x + knots[n].x) / 2.0;
        // Get first control points X-values
        double* x = GetFirstControlPoints(rhs, n);

        // Set right hand side Y values
        for (int i = 1; i < n - 1; ++i)
            rhs[i] = 4 * knots[i].y + 2 * knots[i + 1].y;
        rhs[0] = knots[0].y + 2 * knots[1].y;
        rhs[n - 1] = (8 * knots[n - 1].y + knots[n].y) / 2.0;
        // Get first control points Y-values
        double* y = GetFirstControlPoints(rhs, n);

        // Fill output arrays.
        for (int i = 0; i < n; ++i)
        {
            // First control point
            firstControlPoints[i].setXY(x[i], y[i]);
            // Second control point
            if (i < n - 1)
                secondControlPoints[i].setXY(2 * knots
                    [i + 1].x - x[i + 1], 2 *
                    knots[i + 1].y - y[i + 1]);
            else
                secondControlPoints[i].setXY((knots
                    [n].x + x[n - 1]) / 2,
                    (knots[n].y + y[n - 1]) / 2);
        }

        delete x;
        delete y;
    }

    /// <summary>
    /// Solves a tridiagonal system for one of coordinates (x or y)
    /// of first Bezier control points.
    /// </summary>
    /// <param name="rhs">Right hand side vector.</param>
    /// <returns>Solution vector.</returns>
private:
    static double* GetFirstControlPoints(double* rhs, int size)
    {
        int n = size;
        double* x = new double[n]; // Solution vector.
        double tmp[n]; // Temp workspace.

        double b = 2.0;
        x[0] = rhs[0] / b;
        for (int i = 1; i < n; i++) // Decomposition and forward substitution.
        {
            tmp[i] = 1 / b;
            b = (i < n - 1 ? 4.0 : 3.5) - tmp[i];
            x[i] = (rhs[i] - x[i - 1]) / b;
        }
        for (int i = 1; i < n; i++)
            x[n - i - 1] -= tmp[n - i] * x[n - i]; // Backsubstitution.

        return x;
    }
};
#endif // BEZIERSPLINE_H
