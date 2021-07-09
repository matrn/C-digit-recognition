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
				'weights': np.random.rand(layer['output_dim'], layer['input_dim'])*0.01,
				'bias': np.random.rand(layer['output_dim'])*0.01,
				'activation_function': ACTIVATION_FUNCTIONS[layer['activation']],
				'activation_function_derivative' : ACTIVATION_FUNCTIONS[layer['activation']+'_derivative'],
				'activation' : np.zeros(layer['input_dim']),   # output of the layer before applying activaton function
				'output' : layer['input_dim']   # output of the layer after aplying activation function
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
			# print(a)
			# print(layer['weights'])
			activation = np.matmul(layer['weights'], output) + layer['bias']
			output = layer['activation_function'](activation)
			layer['activation'] = activation
			layer['output'] = output
			# print(a)

		return output

	def backpropagation(self, input, output):
		output_layer_output = self.forward(input)
		print(output_layer_output)
		#print(input, output)
		print(self.net[-1]['activation_function_derivative'](self.net[-1]['activation']))
		output_layer_error = self.net[-1]['activation_function_derivative'](self.net[-1]['activation'])*(output_layer_output-output)
		print(output_layer_error)
		output_layer_gradient = output_layer_error*self.net[-2]['output']
		print(output_layer_gradient)

		next_layer_error = output_layer_error
		for i, layer in enumerate(reversed(self.net[:-1])):
			layer_next = self.net[i+1]
			error = layer['activation_function_derivative'](layer['activation'])*np.dot(np.insert(layer_next['weights'], 0, layer_next['bias']), next_layer_error)
			if i == 0:
				previous_layer_output = input
			else:
				previous_layer_output = self.net[i-1]['output']
			
			gradient = error*previous_layer_output
			next_layer_error = error




if __name__ == '__main__':
	nn = NN(train_filename, test_filename)
	nn.load_train_data()
	nn.load_test_data()
	nn.init_network(nn_layout)
	out = np.zeros(10)
	#print(type(nn.train_labels[0]))
	out[int(nn.train_labels[0])] = 1
	#print(out)
	nn.backpropagation(nn.train_data[0, :, :].ravel(), out)
	print(nn.forward(nn.train_data[0, :, :].ravel()))
	print(nn.net)
