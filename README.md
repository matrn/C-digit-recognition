# C-digit-recognition
Handwritten digits recognition in C using neural network trained using MNIST database (http://yann.lecun.com/exdb/mnist/)

## Libraries
 - `lag` - Linear Algebra library
 - `ceural` - C neural network library

### Lag
- `mat` - stands for matrix
- `ew` - stands for element wise

### Ceural

## Performance
Even though Python is much slower than C, Python-digit-recognition is much faster. The reason behind it is that Python version uses great library NumPy, which is written in C and uses BLAS implementation of matrix operations.

## ToDo
- [ ] Cleanup & document `lag` library
- [ ] Cleanup & document `ceural` library
- [ ] Use BLAS (for example [OpenBLAS](https://github.com/xianyi/OpenBLAS)) library for linear algebra to improve speed
- [ ] Look into possible performance improvements

## Resources
- [George Hotz's AI notebook](https://github.com/geohot/ai-notebooks/blob/master/mnist_from_scratch.ipynb)
- [Artcile about backpropagation from Brilliant.org](https://brilliant.org/wiki/backpropagation/)
- [Michael Nielsen's book about neural networks and deep learning](http://neuralnetworksanddeeplearning.com/chap2.html)