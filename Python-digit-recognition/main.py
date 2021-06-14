from utils import load_mnist_images, load_mnist_labels, show_image
import numpy as np


train_filename = ('./data/train-images-idx3-ubyte', './data/train-labels-idx1-ubyte')
test_filename = ('./data/t10k-images-idx3-ubyte', './data/t10k-labels-idx1-ubyte')


# 2 - layer: input layer (input_layer_len), 300 hidden units, output layer (10)
nn_layout = [
	{
		'name':'hidden_layer',
		'input_dim': 784,
		'output_dim':300,
		'activation':'relu',
	},
	{
		'name':'output_layer',
		'input_dim':300,
		'output_dim':10,
		'activation':'sigmoid',
	}

]

def relu(val):
	print(val)
	return np.maximum(0, val)

def sigmoid(val):
	print(val)
	return 1/(1+np.exp(-val))


ACTIVATION_FUNCTIONS = {
	'relu' : relu,
	'sigmoid' : sigmoid
}


class NN:
	def __init__(self, train_filename: tuple, test_filename: tuple):
		self.train_filename = train_filename
		self.test_filename = test_filename

		self.train_data = None
		self.train_labels = None

		self.test_data = None
		self.test_labels = None
		self.net = []
	

		
	
	def init_network(self, layout):
		for layer in layout:
			self.net.append({
				'weights' : np.random.rand(layer['output_dim'], layer['input_dim'])*0.01,
				'bias' : np.random.rand(layer['output_dim'])*0.01,
				'activation':ACTIVATION_FUNCTIONS[layer['activation']]
			})


	def load_train_data(self):
		self.train_data = load_mnist_images(self.train_filename[0])
		self.train_labels = load_mnist_labels(self.train_filename[1])

	def load_test_data(self):
		self.test_data = load_mnist_images(self.test_filename[0])
		self.test_labels = load_mnist_labels(self.test_filename[1])

	def forward(self, input):
		prev = input
		for layer in self.net:
			print(layer['weights'].shape)
			prev = layer['activation'](np.matmul(layer['weights'],prev) + layer['bias'])
			print(prev)
		
		return prev



if __name__ == '__main__':
	nn = NN(train_filename, test_filename)
	nn.load_train_data()
	nn.load_test_data()
	nn.init_network(nn_layout)
	print(nn.forward(nn.train_data[0,:,:].ravel()))