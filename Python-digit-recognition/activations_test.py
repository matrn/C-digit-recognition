from main import sigmoid, sigmoid_derivative, relu, relu_derivative
import numpy as np


def trelu():
	inp = np.array([2, 2])
	print(relu(inp))

	inp = np.array([0, 0])
	print(relu(inp))

	inp = np.array([-1, -1])
	print(relu(inp))

def trelu_d():
	inp = np.array([2, 2])
	print(relu_derivative(inp))

	inp = np.array([0, 0])
	print(relu_derivative(inp))

	inp = np.array([-1, -1])
	print(relu_derivative(inp))


def tsigmoid():
	inp = np.array([-1, -2])
	print(sigmoid(inp))

	inp = np.array([-1, -2])
	print(sigmoid(inp))

	inp = np.array([1, 0])
	print(sigmoid(inp))

	inp = np.array([0, 1])
	print(sigmoid(inp))

	inp = np.array([2, 2])
	print(sigmoid(inp))

	
def tsigmoid_d():
	inp = np.array([-2, -1])
	print(sigmoid_derivative(inp))

	inp = np.array([-1, -2])
	print(sigmoid_derivative(inp))

	inp = np.array([0, 1])
	print(sigmoid_derivative(inp))

	inp = np.array([1, 0])
	print(sigmoid_derivative(inp))

	inp = np.array([2, 2])
	print(sigmoid_derivative(inp))


if __name__ == '__main__':
	print("RELU:")
	trelu()

	print("RELU_d:")
	trelu_d()

	print("SIGMOID:")
	tsigmoid()

	print("SIGMOID_d:")
	tsigmoid_d()