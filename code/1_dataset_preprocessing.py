import tensorflow as tf
from tensorflow.keras.datasets import mnist
import numpy as np
import matplotlib.pyplot as plt

(x_train, y_train), (x_test, y_test) = mnist.load_data()

plt.imshow(x_train[0], cmap = 'gray')
print('label', y_train[0])

dataset = {'train': (x_train, y_train, 7291),
           'test': (x_test, y_test, 2000)}

for split_name, (x_raw, y_raw, n) in dataset.items():
    # create empty array
    X = np.zeros((n, 28, 28, 1), dtype=np.float32)
    Y = -np.ones((n, 10), dtype=np.float32)
    imgs = x_raw[0:n].astype(np.float32)
    # scale to [-1, 1]
    imgs = imgs / 127.5 - 1.0
    imgs = imgs[..., None]  # add channel dim (28,28,1)
    X[:] = imgs
    # labels: {-1, +1} one-hot
    ys = y_raw[0:n]  # Take first 7291 samples

    for i in range(n):
        Y[i, ys[i]] = 1.0

    # save in a portable format
    np.savez(f"{split_name}1989.npz", X=X, Y=Y)

    print(f"Saved {split_name}1989.npz    shape X={X.shape}, Y={Y.shape}")