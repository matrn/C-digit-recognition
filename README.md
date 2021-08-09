# C-digit-recognition
Handwritten digits recognition in C using neural network trained using [MNIST database](http://yann.lecun.com/exdb/mnist/)


# Installation

## Linux
- `sudo apt install libgtk-3-dev`
- `make`

## Windows
🤷‍♂️


# Libraries
Dependency of the libraries is in this order: `GUI -> ceural -> lag`
 - `lag` - [Linear Algebra library](#Lag)
 - `ceural` - [C neural network library](#Ceural)

## Lag
- `mat` - stands for matrix
- `ew` - stands for element wise
- Matrix part of the library automatically checks if destination and source is same where shouldn't be same and warns using `assert()`.

## Ceural
Ceural library is created for multi-layer networks for MNIST dataset but with small modifications it can be used for other datasets.

# GUI

## Control
- `left mouse button & drag` to draw
- `right mouse button` to clear the draw space
- `middle mouse button or Recognise button in the GUI` to run recognition process

## Processing of the drawn image
Preprocessing used in [MNIST](http://yann.lecun.com/exdb/mnist/) database: *The original black and white (bilevel) images from NIST were size normalized to fit in a 20x20 pixel box while preserving their aspect ratio. The resulting images contain grey levels as a result of the anti-aliasing technique used by the normalization algorithm. the images were centered in a 28x28 image by computing the center of mass of the pixels, and translating the image so as to position this point at the center of the 28x28 field.*

1. **crop calculation** - crop of the whole draw space is calculated from sides until it reaches non white pixels. After that maximum of the width & height of the cropped image is then taken and `crop_x` & `crop_y` & `crop_w` & `crop_h` is recalculated to preserve image ratio.
2. **sub image generation** - using previous values and drawn image stored in pixbuf sub image is created
3. **scaling** - previous cropped image is scaled to the `20x20` image
4. **conversion into grayscale** - pixbuf is converted into `uint8_t` grayscale image
5. **adding frame** - `4, 4, 4, 4` frame is added to the `20x20` image resulting into `28x28` image
6. **computation of the center of mass of the pixels** - is done using mean values accross X & Y
7. **move of the submatrix** - submatrix (drawn number) is moved in the framed image
8. **neural network forward propagation** - this preprocessed image is fed to the neural network

# Accuracy
After training the test set accuracy is `94.49 %`. This accuracy not bad considering the test error rate in [MNIST database website](http://yann.lecun.com/exdb/mnist/) of the 2-layer NN. Sadly accuracy is not as good in practice as it's in the test data set.

Accuracy is calculated using formula [`accuracy = (TP+TN)/(TP+TN+FP+FN)`](https://en.wikipedia.org/wiki/Accuracy_and_precision) which is `accuracy = correct/total`


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
- [x] Center digit by center of mass of the pixels before feeding it to the neural network from GUI input
- [ ] Use BLAS (for example [OpenBLAS](https://github.com/xianyi/OpenBLAS)) library for linear algebra to improve speed
- [ ] Look into possible accuracy improvements


# Resources
- [MNIST database](http://yann.lecun.com/exdb/mnist/)
- [George Hotz's AI notebook](https://github.com/geohot/ai-notebooks/blob/master/mnist_from_scratch.ipynb)
- [Michael Nielsen's book about neural networks and deep learning](http://neuralnetworksanddeeplearning.com/chap2.html)
- [Artcile about backpropagation from Brilliant.org](https://brilliant.org/wiki/backpropagation/)