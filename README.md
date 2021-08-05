# C-digit-recognition
Handwritten digits recognition in C using neural network trained using [MNIST database](http://yann.lecun.com/exdb/mnist/)

# Installation

## Linux
- `sudo apt install libgtk-3-dev`
- `make`

# Libraries
 - `lag` - [Linear Algebra library](##Lag)
 - `ceural` - [C neural network library](##Ceural)

## Lag
- `mat` - stands for matrix
- `ew` - stands for element wise
- Matrix part of the library automatically checks if destination and source is same where shouldn't be same and warns using `assert()`.

## Ceural
Ceural library is created for multi-layer networks for MNIST dataset but with small modifications it can be used for other datasets.

# Accuracy
After training & testing and using formula [`accuracy = (TP+TN)/(TP+TN+FP+FN)`](https://en.wikipedia.org/wiki/Accuracy_and_precision) which is `accuracy = correct/total`, we can get accuracy `94.49 %` on the test set which is not bad considering the test error rate in [MNIST database website](http://yann.lecun.com/exdb/mnist/) of the 2-layer NN.
Sadly accuracy is not as good in practice as in the test data set. Maybe it's due to missing centering of the digit but more debugging is needed.

# Performance
Even though Python is much slower than C, Python-digit-recognition is much faster. The reason behind it is that Python version uses great library NumPy, which is written in C and uses BLAS implementation of matrix operations.

# ToDo
- [ ] Cleanup & document `lag` library
- [ ] Cleanup & document `ceural` library
- [ ] Cleanup & document `gui`
- [ ] Add icons into `gui`
- [ ] Add command line options to train & test & save & load NN
- [x] Check NN definition during the NN weights & biases load process
- [ ] Create documentation
- [ ] Write everything into README
- [ ] Choose license
- [ ] Finish top-level 'Makefile' to create final binary for linux
- [ ] Create Windows compilation script & test it on Windows
- [ ] Center digit by center of mass of the pixels before feeding it to the neural network from GUI input
- [ ] Use BLAS (for example [OpenBLAS](https://github.com/xianyi/OpenBLAS)) library for linear algebra to improve speed
- [ ] Look into possible accuracy improvements

# Resources
- [George Hotz's AI notebook](https://github.com/geohot/ai-notebooks/blob/master/mnist_from_scratch.ipynb)
- [Artcile about backpropagation from Brilliant.org](https://brilliant.org/wiki/backpropagation/)
- [Michael Nielsen's book about neural networks and deep learning](http://neuralnetworksanddeeplearning.com/chap2.html)