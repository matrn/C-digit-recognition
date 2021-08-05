# C-digit-recognition
Handwritten digits recognition in C using neural network trained using MNIST database (http://yann.lecun.com/exdb/mnist/)

## Libraries
 - `lag` - Linear Algebra library
 - `ceural` - C neural network library

### Lag
- `mat` - stands for matrix
- `ew` - stands for element wise
- Matrix part of the library automatically checks if destination and source is same where shouldn'ŧ be same and warns using `assert()`.

### Ceural

## Performance
Even though Python is much slower than C, Python-digit-recognition is much faster. The reason behind it is that Python version uses great library NumPy, which is written in C and uses BLAS implementation of matrix operations.

## ToDo
- [ ] Cleanup & document `lag` library
- [ ] Cleanup & document `ceural` library
- [ ] Cleanup & document `gui`
- [ ] Add icons into `gui`
- [ ] Create documentation
- [ ] Write everything into README
- [ ] Choose license
- [ ] Finish top-level 'Makefile' to create final binary for linux
- [ ] Create Windows compilation script & test it on Windows
- [ ] Center digit by center of mass of the pixels before feeding it to the neural network from GUI input
- [ ] Use BLAS (for example [OpenBLAS](https://github.com/xianyi/OpenBLAS)) library for linear algebra to improve speed
- [ ] Look into possible accuracy improvements

## Resources
- [George Hotz's AI notebook](https://github.com/geohot/ai-notebooks/blob/master/mnist_from_scratch.ipynb)
- [Artcile about backpropagation from Brilliant.org](https://brilliant.org/wiki/backpropagation/)
- [Michael Nielsen's book about neural networks and deep learning](http://neuralnetworksanddeeplearning.com/chap2.html)