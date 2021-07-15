from numpy.lib.function_base import _gradient_dispatcher, percentile
from utils import load_mnist_images, load_mnist_labels, show_image
import numpy as np


train_filename = ('./data/train-images-idx3-ubyte',
				  './data/train-labels-idx1-ubyte')
test_filename = ('./data/t10k-images-idx3-ubyte',
				 './data/t10k-labels-idx1-ubyte')

# 2 - layer: input layer (input_layer_len), 300 hidden units, output layer (10)
nn_layout = [
	{
		'name': 'hidden_layer',
		'input_dim': 784,
		'output_dim': 128,
		'activation': 'relu',
	},
	{
		'name': 'output_layer',
		'input_dim': 128,
		'output_dim': 10,
		'activation': 'sigmoid',
	}
]


def relu(z):
	return np.maximum(0, z)

def relu_derivative(z):
	z = np.copy(z)
	z[z <= 0] = 0
	z[z > 0] = 1
	return z


def sigmoid(z):
	return 1/(1+np.exp(-z))

def sigmoid_derivative(z):
	sig_z = sigmoid(z)
	return sig_z*(1-sig_z)


ACTIVATION_FUNCTIONS = {
	'relu': relu,
	'relu_derivative': relu_derivative,
	'sigmoid': sigmoid,
	'sigmoid_derivative': sigmoid_derivative
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
		np.random.seed(123)
		for layer in layout:
			self.net.append({
				#'dim' : (layer['input_dim'], layer['output_dim']),
				'weights': np.random.uniform(-1., 1., size=(layer['output_dim'], layer['input_dim']))/np.sqrt(layer['input_dim']*layer['output_dim']),
				'bias': np.zeros(layer['output_dim']), # np.random.uniform(-1., 1., layer['output_dim'])/layer['output_dim'], #np.random.rand(layer['output_dim'])*0.0001,
				'activation_function': ACTIVATION_FUNCTIONS[layer['activation']],
				'activation_function_derivative' : ACTIVATION_FUNCTIONS[layer['activation']+'_derivative'],
				'activation' : np.zeros(layer['input_dim']),   # output of the layer before applying activaton function
				'output' : layer['input_dim'],   # output of the layer after aplying activation function
				'delta' : None,
			})


	def load_train_data(self):
		self.train_data = load_mnist_images(self.train_filename[0])
		self.train_labels = load_mnist_labels(self.train_filename[1])

	def load_test_data(self):
		self.test_data = load_mnist_images(self.test_filename[0])
		self.test_labels = load_mnist_labels(self.test_filename[1])


	def forward(self, input):
		output = input
		for layer in self.net:
			activation = np.matmul(layer['weights'], output) + layer['bias']
			output = layer['activation_function'](activation)
			layer['activation'] = activation
			layer['output'] = output

		return output


	def backpropagate(self, input, expected_output):
		output_layer_output = self.forward(input)
		output_layer_error = output_layer_output - expected_output   # derivative of cost function
		output_layer_delta = output_layer_error * self.net[-1]['activation_function_derivative'](self.net[-1]['activation'])   # element wise matrix multiplication
		
		self.net[-1]['delta'] = output_layer_delta
		right_layer_delta = output_layer_delta
		for i in range(len(self.net)-2, -1, -1): #, layer in enumerate(reversed(self.net[:-1])):
			layer = self.net[i]
			right_layer = self.net[i+1]

			# error = layer['activation_function_derivative'](layer['activation'])*np.dot(np.insert(layer_next['weights'], 0, layer_next['bias']), next_layer_error)

			# weights = np.c_[right_layer['weights'], right_layer['bias']]   #insert bias vector as column into weights matrix
			weights = right_layer['weights']
			error = np.matmul(weights.T, right_layer_delta)
			delta = error * layer['activation_function_derivative'](layer['activation'])
			right_layer_delta = delta
			self.net[i]['delta'] = delta
	

	def update(self, input, learning_rate = 0.001):
		output = input
		for layer in self.net:
			update = np.matmul(np.atleast_2d(layer['delta']).T, np.atleast_2d(output))
			output = layer['output']
			layer['weights'] -= learning_rate*update
			layer['bias'] -= learning_rate*layer['delta']


	def train(self, epochs = 1):
		for epoch in range(epochs):
			print(f'Epoch #{epoch}')
			for i in range(0, 6000):
				out = np.zeros(10)
				out[int(self.train_labels[i])] = 1
				self.backpropagate(self.train_data[i, :, :].ravel(), out)
				self.update(self.train_data[i, :, :].ravel())


	def test(self):
		correct = 0
		wrong = 0
		print("Running test set")
		for i in range(10000):
			predicted = np.argmax(self.forward(self.test_data[i, :, :].ravel()))
			expected = int(self.test_labels[i])
			#print(f'{predicted} vs {expected}')
			if predicted == expected:
				correct += 1
			else:
				wrong += 1
		
		print(f'Correct {correct}, wrong: {wrong}')
		print(f'Accuracy {correct/10000.} %')


if __name__ == '__main__':
	nn = NN(train_filename, test_filename)
	nn.load_train_data()
	nn.load_test_data()
	nn.init_network(nn_layout)

	nn.test()
	print("-------")	
	nn.train()
	nn.test()


	print(nn.forward(nn.train_data[0, :, :].ravel()))
	print(nn.forward(nn.train_data[1, :, :].ravel()))

	print(np.argmax(nn.forward(nn.train_data[0, :, :].ravel())))
	print(np.argmax(nn.forward(nn.train_data[1, :, :].ravel())))
	#show_image(nn.train_data[1, :, :])