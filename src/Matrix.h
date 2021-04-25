#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <functional>

template <unsigned int A, unsigned int B>
class Matrix
{
public:
    const int rows, cols;
    float data[A][B];
    Matrix();
    Matrix(float _data[A][B]);
    Matrix(float _data[A]);
    Matrix(const Matrix<A, B> &a);
    void initializeRandom(int a = -1, int b = 1);
    void initializeRandomInt(int a = 10);
    void add(float a);
    void addMatrix(const Matrix<A, B> &a);
    void multiply(float m);
    void multiplyMatrix(const Matrix<A, B> &m);
    void map(const std::function<float(float)>& func);
    void map1f(const std::function<float(float, float)>& func, float dato);
    Matrix<B, A> transpose() const;
    float *toArray() const;
    Matrix<A, B> operator+ (const Matrix<A, B> &a) const;
    Matrix<A, B> operator- (const Matrix<A, B> &a) const;
    Matrix<A, B> operator* (const Matrix<A, B> &a) const;
    Matrix<A, B> operator/ (const Matrix<A, B> &a) const;
    float& operator() (unsigned int r, unsigned int c);
    const float& operator() (unsigned int r, unsigned int c) const;
    void print() const;

    static void copy(Matrix<A, B> &dest, const Matrix<A, B> &source);
};

float frand(float a = 0, float b = 1)
{
    return ((double)std::rand() / RAND_MAX) * (b - a) + a;
}

int irand(int bound = 10)
{
    return std::rand() % bound;
}

float sigmoid(float x)
{
    return 1 / (1 + std::exp(-x));
}

float reLu(float x)
{
    if (x > 0){
        return x;
    } else{
        return 0;
    }
}

template <unsigned int A, unsigned int B, unsigned int C>
Matrix<A, C> dotProduct(const Matrix<A, B> &a, const Matrix<B, C> &b)
{
    Matrix<A, C> m;
    for (int r = 0; r < m.rows; r++){
        for (int c = 0; c < m.cols; c++){
            m.data[r][c] = 0;
            for (int k = 0; k < a.cols; k++){
                m.data[r][c] += a.data[r][k] * b.data[k][c];
            }
        }
    }
    return m;
}

template <unsigned int A, unsigned int B, unsigned int C>
void dotProductTo(Matrix<A, C> &dest, const Matrix<A, B> &a, const Matrix<B, C> &b)
{
    for (unsigned int r = 0; r < A; r++){
        for (unsigned int c = 0; c < C; c++){
            dest.data[r][c] = 0;
            for (int k = 0; k < a.cols; k++){
                dest.data[r][c] += a.data[r][k] * b.data[k][c];
            }
        }
    }
}

template <unsigned int A, unsigned int B>
Matrix<A, B>::Matrix() : rows(A), cols(B)
{}

template <unsigned int A, unsigned int B>
Matrix<A, B>::Matrix(float _data[A][B]) : rows(A), cols(B)
{
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            data[r][c] = _data[r][c];
        }
    }
}

template <unsigned int A, unsigned int B>
Matrix<A, B>::Matrix(float _data[A]) : rows(A), cols(1)
{
    for (int r = 0; r < rows; r++){
        data[r][0] = _data[r];
    }
}

template <unsigned int A, unsigned int B>
Matrix<A, B>::Matrix(const Matrix<A, B> &a) : rows(A), cols(B)
{
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            data[r][c] = a.data[r][c];
        }
    }
}

template <unsigned int A, unsigned int B>
void Matrix<A, B>::initializeRandom(int a, int b)
{
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            data[r][c] = ((float)std::rand() / RAND_MAX) * (b - a) + a;
        }
    }
}

template <unsigned int A, unsigned int B>
void Matrix<A, B>::initializeRandomInt(int a)
{
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            data[r][c] = std::rand() % a;
        }
    }
}

template <unsigned int A, unsigned int B>
void Matrix<A, B>::add(float a)
{
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            data[r][c] += a;
        }
    }
}

template <unsigned int A, unsigned int B>
void Matrix<A, B>::addMatrix(const Matrix<A, B> &a)
{
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            data[r][c] += a.data[r][c];
        }
    }
}

template <unsigned int A, unsigned int B>
void Matrix<A, B>::multiply(float m)
{
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            data[r][c] *= m;
        }
    }
}

template <unsigned int A, unsigned int B>
void Matrix<A, B>::multiplyMatrix(const Matrix<A, B> &m)
{
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            data[r][c] *= m.data[r][c];
        }
    }
}

template <unsigned int A, unsigned int B>
void Matrix<A, B>::map(const std::function<float(float)>& func)
{
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            data[r][c] = func(data[r][c]);
        }
    }
}

template <unsigned int A, unsigned int B>
void Matrix<A, B>::map1f(const std::function<float(float, float)>& func, float dato)
{
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            data[r][c] = func(data[r][c], dato);
        }
    }
}

template <unsigned int A, unsigned int B>
Matrix<B, A> Matrix<A, B>::transpose() const
{
    Matrix<B, A> t;
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            t.data[c][r] = data[r][c];
        }
    }
    return t;
}

template <unsigned int A, unsigned int B>
void Matrix<A, B>::copy(Matrix<A, B> &dest, const Matrix<A, B> &source)
{
    for (unsigned int r = 0; r < A; r++){
        for (unsigned int c = 0; c < B; c++){
            dest.data[r][c] = source.data[r][c];
        }
    }
}

template <unsigned int A, unsigned int B>
float *Matrix<A, B>::toArray() const
{
    float *result = new float[A * B];
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            result[r * B + c] = data[r][c];
        }
    }
    return result;
}

template <unsigned int A, unsigned int B>
Matrix<A, B> Matrix<A, B>::operator+ (const Matrix<A, B> &a) const
{
    Matrix<A, B> result;
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            result.data[r][c] = data[r][c] + a.data[r][c];
        }
    }
    return result;
}

template <unsigned int A, unsigned int B>
Matrix<A, B> Matrix<A, B>::operator- (const Matrix<A, B> &a) const
{
    Matrix<A, B> result;
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            result.data[r][c] = data[r][c] - a.data[r][c];
        }
    }
    return result;
}

template <unsigned int A, unsigned int B>
Matrix<A, B> Matrix<A, B>::operator* (const Matrix<A, B> &a) const
{
    Matrix<A, B> result;
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            result.data[r][c] = data[r][c] * a.data[r][c];
        }
    }
    return result;
}

template <unsigned int A, unsigned int B>
Matrix<A, B> Matrix<A, B>::operator/ (const Matrix<A, B> &a) const
{
    Matrix<A, B> result;
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            result.data[r][c] = data[r][c] / a.data[r][c];
        }
    }
    return result;
}

template <unsigned int A, unsigned int B>
float& Matrix<A, B>::operator() (unsigned int r, unsigned int c)
{
  return data[r][c];
}

template <unsigned int A, unsigned int B>
const float& Matrix<A, B>::operator() (unsigned int r, unsigned int c) const
{
  return data[r][c];
}

template <unsigned int A, unsigned int B>
void Matrix<A, B>::print() const
{
    printf("-----------------------------------------------\n");
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            printf("%f ", data[r][c]);
        }
        printf("\n");
    }
}

#endif // MATRIX_H_INCLUDED
