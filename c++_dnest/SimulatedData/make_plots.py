import numpy as np
import matplotlib.pyplot as plt

files = ['sparse.txt', 'cluster.txt', 'crowded.txt']
names = ['Sparse', 'Cluster', 'Crowded']

images = [np.loadtxt(f) for f in files]

for k in xrange(0, len(files)):
	plt.subplot(1,3,k+1)
	img = images[k]
	img = (img - img.min())/(img.max() - img.min())
	plt.imshow(img**0.5, interpolation='nearest', cmap='gray')
	plt.title(names[k])
	plt.gca().set_xticks([0, 99./2, 99])
	plt.gca().set_yticks([0, 99./2, 99])
	plt.gca().set_xticklabels(['-1', '0', '1'])
	plt.gca().set_yticklabels(['1', '0', '-1'])
	if k != 0:
		plt.gca().set_yticks([])
	if k == 0:
		plt.ylabel('$y$', fontsize=16)
	if k == 1:
		plt.xlabel('$x$', fontsize=16)

plt.savefig('test_cases.eps', bbox_inches='tight')
plt.show()

