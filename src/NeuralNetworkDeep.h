#ifndef NEURALNETWORKDEEP_H_INCLUDED
#define NEURALNETWORKDEEP_H_INCLUDED

#include "Matrix.h"
#include "Snake.h"

template <unsigned int I, unsigned int N, unsigned int H, unsigned int O>
class NeuralNetworkDeep
{
public:
    const int inputNodes, numberHidden, hiddenNodes, outputNodes;
    Matrix<H, I> iWeights;
    Matrix<H, H> hWeights[N];
    Matrix<O, H> oWeights;
    Matrix<H, 1> iBias;
    Matrix<H, 1> hBias[N];
    Matrix<O, 1> oBias;
    NeuralNetworkDeep();
    NeuralNetworkDeep(const NeuralNetworkDeep<I, N, H, O> &a);
    void initializeRandom();
    float *feedForward(float data[I]);

    static void copy(NeuralNetworkDeep<I, N, H, O> &dest, NeuralNetworkDeep<I, N, H, O> &source);
};

template <unsigned int I, unsigned int N, unsigned int H, unsigned int O>
NeuralNetworkDeep<I, N, H, O>::NeuralNetworkDeep() : inputNodes(I), numberHidden(N), hiddenNodes(H), outputNodes(O)
{}

template <unsigned int I, unsigned int N, unsigned int H, unsigned int O>
NeuralNetworkDeep<I, N, H, O>::NeuralNetworkDeep(const NeuralNetworkDeep<I, N, H, O> &a) : inputNodes(I), numberHidden(N), hiddenNodes(H), outputNodes(O)
{
    Matrix<H, I>::copy(iWeights, a.iWeights);
    Matrix<O, H>::copy(oWeights, a.oWeights);
    Matrix<H, 1>::copy(iBias, a.iBias);
    Matrix<O, 1>::copy(oBias, a.oBias);
    for (unsigned int i = 0; i < N; i++){
        Matrix<H, H>::copy(hWeights[i], a.hWeights[i]);
        Matrix<H, 1>::copy(hBias[i], a.hBias[i]);
    }
}

template <unsigned int I, unsigned int N, unsigned int H, unsigned int O>
void NeuralNetworkDeep<I, N, H, O>::initializeRandom()
{
    iWeights.initializeRandom();
    oWeights.initializeRandom();
    iBias.initializeRandom();
    oBias.initializeRandom();
    for (unsigned int i = 0; i < N; i++){
       hWeights[i].initializeRandom();
       hBias[i].initializeRandom();
    }
}

template <unsigned int I, unsigned int N, unsigned int H, unsigned int O>
float *NeuralNetworkDeep<I, N, H, O>::feedForward(float dataArray[I])
{
    Matrix<I, 1> input(dataArray);
    Matrix<H, 1> hidden[N + 1];
    dotProductTo(hidden[0], iWeights, input);
    hidden[0].addMatrix(iBias);
    hidden[0].map(reLu);

    for (unsigned int i = 0; i < N; i++){
        dotProductTo(hidden[i + 1], hWeights[i], hidden[i]);
        hidden[i + 1].addMatrix(hBias[i]);
        hidden[i + 1].map(sigmoid);
    }

    Matrix<O, 1> output = dotProduct(oWeights, hidden[N]);
    output.addMatrix(oBias);
    output.map(sigmoid);

    return output.toArray();
}

template <unsigned int I, unsigned int N, unsigned int H, unsigned int O>
void NeuralNetworkDeep<I, N, H, O>::copy(NeuralNetworkDeep<I, N, H, O> &dest, NeuralNetworkDeep<I, N, H, O> &source)
{
    Matrix<H, I>::copy(dest.iWeights, source.iWeights);
    Matrix<O, H>::copy(dest.oWeights, source.oWeights);
    Matrix<H, 1>::copy(dest.iBias, source.iBias);
    Matrix<O, 1>::copy(dest.oBias, source.oBias);
    for (unsigned int i = 0; i < N; i++){
        Matrix<H, H>::copy(dest.hWeights[i], source.hWeights[i]);
        Matrix<H, 1>::copy(dest.hBias[i], source.hBias[i]);
    }
}

#endif // NEURALNETWORKDEEP_H_INCLUDED
