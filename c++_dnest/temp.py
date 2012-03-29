from pylab import *
import time

sample = atleast_2d(loadtxt('sample.txt'))
data = loadtxt('data_100stars.txt')
sample = sample[:,5:]
ion()
hold(False)
for i in xrange(0, sample.shape[0]):
	subplot(1,3,1)
	imshow(data.reshape(100,100), interpolation='none')
	subplot(1,3,2)
	imshow(sample[i,:].reshape(100,100), interpolation='none')
	title(str(i+1))
	subplot(1,3,3)
	imshow((sample[i,:] - data).reshape(100,100), interpolation='none')
	draw()
	print([i, max(sample[i,:])])
ioff()
show()

