import requests
import gzip
import shutil
from pathlib import Path
import os


train_imgs_url = "http://yann.lecun.com/exdb/mnist/train-images-idx3-ubyte.gz"
train_labels_url = "http://yann.lecun.com/exdb/mnist/train-labels-idx1-ubyte.gz"

test_imgs_url = "http://yann.lecun.com/exdb/mnist/t10k-images-idx3-ubyte.gz"
test_labels_url = "http://yann.lecun.com/exdb/mnist/t10k-labels-idx1-ubyte.gz"


def download_file(url):
	filename = url.split('/')[-1]
	with requests.get(url, stream=True) as r:
		r.raise_for_status()
		with open(filename, 'wb') as f:
			for chunk in r.iter_content(chunk_size=4092):
				f.write(chunk)
	return filename

def gunzip(gz_filename, gz_dir = ".", out_dir = "."):
	out_filename = gz_filename.replace('.gz', '')
	with gzip.open(gz_filename, 'rb') as f_in:
		with open(out_filename, 'wb') as f_out:
			shutil.copyfileobj(f_in, f_out)
	
	shutil.move(gz_filename, os.path.join(gz_dir, gz_filename))
	shutil.move(out_filename, os.path.join(out_dir, out_filename))


Path("data/gz").mkdir(parents=True, exist_ok=True)

print("Downloading train images...")
gunzip(download_file(train_imgs_url), gz_dir="./data/gz", out_dir="./data")

print("Downloading train labels...")
gunzip(download_file(train_labels_url), gz_dir="./data/gz", out_dir="./data")

print("Downloading test images...")
gunzip(download_file(test_imgs_url), gz_dir="./data/gz", out_dir="./data")

print("Downloading test labels...")
gunzip(download_file(test_labels_url), gz_dir="./data/gz", out_dir="./data")