from numpy.lib.function_base import _gradient_dispatcher
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
		'output_dim': 300,
		'activation': 'relu',
	},
	{
		'name': 'output_layer',
		'input_dim': 300,
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
	# print("sigmoid")
	# print(z)
	# print(np.exp(-z))
	# print(1/(1+np.exp(-z)))
	# print("------")
	return 1/(1+np.exp(-z))


def sigmoid_derivative(z):
	# print("simgoid d")
	sig_z = sigmoid(z)
	# print(sig_z)
	return sig_z*(1-sig_z)


ACTIVATION_FUNCTIONS = {
	'relu': relu,
	'relu_derivative': relu_derivative,
	'sigmoid': sigmoid,
	'sigmoid_derivative': sigmoid_derivative
}


class NN:
	def __init__(self, train_filename: tuple, test_filename: tuple, learning_rate = 0.7):
		self.train_filename = train_filename
		self.test_filename = test_filename

		self.train_data = None
		self.train_labels = None

		self.test_data = None
		self.test_labels = None
		self.net = []
		self.learning_rate = learning_rate
		self.memory = None


	def init_network(self, layout):
		np.random.seed(123)
		for layer in layout:
			self.net.append({
				'dim' : (layer['input_dim'], layer['output_dim']),
				'weights': np.random.uniform(-1., 1., size=(layer['output_dim'], layer['input_dim']))/np.sqrt(layer['input_dim']*layer['output_dim']),
				'bias': np.zeros(layer['output_dim']), #np.random.rand(layer['output_dim'])*0.0001,
				'activation_function': ACTIVATION_FUNCTIONS[layer['activation']],
				'activation_function_derivative' : ACTIVATION_FUNCTIONS[layer['activation']+'_derivative'],
				'activation' : np.zeros(layer['input_dim']),   # output of the layer before applying activaton function
				'output' : layer['input_dim'],   # output of the layer after aplying activation function
				'gradient' : None,
			})

			#print(self.net)
			print("-------------------")

	def load_train_data(self):
		self.train_data = load_mnist_images(self.train_filename[0])
		self.train_labels = load_mnist_labels(self.train_filename[1])

	def load_test_data(self):
		self.test_data = load_mnist_images(self.test_filename[0])
		self.test_labels = load_mnist_labels(self.test_filename[1])

	def forward(self, input):
		output = input
		for layer in self.net:
			# print(a)
			# print(layer['weights'])
			activation = np.matmul(layer['weights'], output) + layer['bias']
			#print(activation)
			output = layer['activation_function'](activation)
			#print(output)
			layer['activation'] = activation
			layer['output'] = output
			#print(activation)

		return output

	def backpropagation(self, input, output):
		output_layer_output = self.forward(input)
		#print(output_layer_output)
		#print("out:", output)
		#print(self.net[-1]['activation_function_derivative'](self.net[-1]['activation']))
		output_layer_error = output_layer_output-output
		#print(output_layer_error)
		output_layer_gradient = output_layer_error*self.net[-1]['activation_function_derivative'](self.net[-1]['activation'])
		
		print(output_layer_gradient)
		self.net[-1]['gradient'] = output_layer_gradient
		right_layer_gradient = output_layer_gradient
		#print(right_layer_gradient)
		for i in range(len(self.net)-2, -1, -1): #, layer in enumerate(reversed(self.net[:-1])):
			layer = self.net[i]
			right_layer = self.net[i+1]
			#print(i)
			#print(layer['dim'])
			
			# print(layer)
			#print(i)
			# layer_next = self.net[i]
			# print(layer_next['weights'].shape)
			# print(layer_next['bias'].shape)
			# print(layer_next['bias'])
			# print("--------")
			#print(np.append(layer_next['weights'], layer_next['bias'], axis=1))
			# error = layer['activation_function_derivative'](layer['activation'])*np.dot(np.insert(layer_next['weights'], 0, layer_next['bias']), next_layer_error)

			# if i == 0:
			# 	previous_layer_output = input
			# else:
			# 	previous_layer_output = self.net[i-1]['output']
			# print(layer['bias'])
			# print()
			# print(layer['weights'])
			# weights = np.c_[right_layer['weights'], right_layer['bias']]   #insert bias vector as column into weights matrix
			weights = right_layer['weights']
			# print()
			#print(weights.shape)
			error = np.dot(weights.T, right_layer_gradient)
			#print(error.shape)
			gradient = error*layer['activation_function_derivative'](layer['activation'])
			right_layer_gradient = gradient
			self.net[i]['gradient'] = gradient
			#print(gradient.shape)
		#print("............")
	


	def update(self, input):
		output = input
		for layer in self.net:
			#print(layer['gradient'].shape)
			#print(np.atleast_2d(output).shape)
			update = np.dot(np.atleast_2d(layer['gradient']).T, np.atleast_2d(output))
			output = layer['output']
			layer['weights'] -= 0.01*update
			layer['bias'] -= 0.01*layer['gradient']


if __name__ == '__main__':
	nn = NN(train_filename, test_filename)
	nn.load_train_data()
	nn.load_test_data()
	nn.init_network(nn_layout)
	
	#print(type(nn.train_labels[0]))
	
	#print(out)
	
	for a in range(0, 1):
		out = np.zeros(10)
		out[int(nn.train_labels[a])] = 1
		nn.backpropagation(nn.train_data[a, :, :].ravel(), out)
		nn.update(nn.train_data[a, :, :].ravel())
	
	#print(nn.net)

	print(nn.forward(nn.train_data[0, :, :].ravel()))
	print(nn.forward(nn.train_data[1, :, :].ravel()))
	#show_image(nn.train_data[1, :, :])
	#print(nn.net)
