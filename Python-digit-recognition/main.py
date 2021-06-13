from numpy.core.numeric import load
from utils import load_mnist_images, load_mnist_labels, show_image


train_filename = ('./data/train-images-idx3-ubyte', './data/train-labels-idx1-ubyte')
test_filename = ('./data/t10k-images-idx3-ubyte', './data/t10k-labels-idx1-ubyte')



class NN:
	def __init__(self, train_filename: tuple, test_filename: tuple):
		self.train_filename = train_filename
		self.test_filename = test_filename

		self.train_data = None
		self.train_labels = None

		self.test_data = None
		self.test_labels = None

	
	def load_train_data(self):
		self.train_data = load_mnist_images(self.train_filename[0])
		self.train_labels = load_mnist_labels(self.train_filename[1])

	def load_test_data(self):
		self.test_data = load_mnist_images(self.test_filename[0])
		self.test_labels = load_mnist_labels(self.test_filename[1])


if __name__ == '__main__':
	nn = NN(train_filename, test_filename)
	nn.load_train_data()
	nn.load_test_data()