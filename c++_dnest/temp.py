from pylab import *
import time

sample = atleast_2d(loadtxt('sample.txt'))
data = loadtxt('data_100stars.txt')
sample = sample[:,5:]

ion()
hold(False)
for i in xrange(0, sample.shape[0]):
	mock = sample[i,:].reshape(100, 100)
	subplot(1,3,1)
	imshow(abs(data)**0.25, interpolation='none', cmap='gray')
	gca().set_xticks([])
	gca().set_yticks([])
	subplot(1,3,2)
	imshow(abs(mock)**0.25, interpolation='none', cmap='gray')
	gca().set_xticks([])
	gca().set_yticks([])
	title(str(i+1))
	subplot(1,3,3)
	imshow(mock - data, interpolation='none', cmap='gray')
	gca().set_xticks([])
	gca().set_yticks([])
	draw()
	print([i, max(sample[i,:])])
ioff()
show()

