import numpy as np
import struct
import matplotlib.pyplot as plt


def load_mnist_images(filename):
	with open(filename,'rb') as f:
		magic, size = struct.unpack('>II', f.read(8))
		nrows, ncols = struct.unpack('>II', f.read(8))
		data = np.fromfile(f, dtype=np.dtype(np.uint8).newbyteorder('>'))
		data = data.reshape((size, nrows, ncols))
		return data

def load_mnist_labels(filename):
	with open(filename, 'rb') as f:
		magic, size = struct.unpack('>II', f.read(8))
		data = np.fromfile(f, dtype=np.dtype(np.uint8).newbyteorder('>'))
		return data


def show_image(img):
	plt.imshow(img, cmap='gray')
	plt.show()