#ifndef NEURALNETWORK_H_INCLUDED
#define NEURALNETWORK_H_INCLUDED

#include "Matrix.h"
#include "Snake.h"

template <unsigned int I, unsigned int H, unsigned int O>
class NeuralNetwork
{
public:
    const int inputNodes, hiddenNodes, outputNodes;
    Matrix<H, I> hWeights;
    Matrix<O, H> oWeights;
    Matrix<H, 1> hBias;
    Matrix<O, 1> oBias;
    NeuralNetwork();
    NeuralNetwork(const NeuralNetwork<I, H, O> &a);
    void initializeRandom();
    float *feedForward(float data[I]);

    static void copy(NeuralNetwork<I, H, O> &dest, NeuralNetwork<I, H, O> &source);
};

template <unsigned int I, unsigned int H, unsigned int O>
NeuralNetwork<I, H, O>::NeuralNetwork() : inputNodes(I), hiddenNodes(H), outputNodes(O)
{}

template <unsigned int I, unsigned int H, unsigned int O>
NeuralNetwork<I, H, O>::NeuralNetwork(const NeuralNetwork<I, H, O> &a) : inputNodes(I), hiddenNodes(H), outputNodes(O)
{
    Matrix<H, I>::copy(hWeights, a.hWeights);
    Matrix<O, H>::copy(oWeights, a.oWeights);
    Matrix<H, 1>::copy(hBias, a.hBias);
    Matrix<O, 1>::copy(oBias, a.oBias);
}

template <unsigned int I, unsigned int H, unsigned int O>
void NeuralNetwork<I, H, O>::initializeRandom()
{
    hWeights.initializeRandom();
    oWeights.initializeRandom();
    hBias.initializeRandom();
    oBias.initializeRandom();
}

template <unsigned int I, unsigned int H, unsigned int O>
float *NeuralNetwork<I, H, O>::feedForward(float data[I])
{
    Matrix<I, 1> input(data);
    Matrix<H, 1> hidden = dotProduct(hWeights, input);
    hidden.addMatrix(hBias);
    hidden.map(reLu);

    Matrix<O, 1> output = dotProduct(oWeights, hidden);
    output.addMatrix(oBias);
    output.map(sigmoid);

    return output.toArray();
}

template <unsigned int I, unsigned int H, unsigned int O>
void NeuralNetwork<I, H, O>::copy(NeuralNetwork<I, H, O> &dest, NeuralNetwork<I, H, O> &source)
{
    Matrix<H, I>::copy(dest.hWeights, source.hWeights);
    Matrix<O, H>::copy(dest.oWeights, source.oWeights);
    Matrix<H, 1>::copy(dest.hBias, source.hBias);
    Matrix<O, 1>::copy(dest.oBias, source.oBias);
}

#endif // NEURALNETWORK_H_INCLUDED
