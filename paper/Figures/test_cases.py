import plot_utils
import matplotlib.pyplot as pl
import numpy as np

img1 = np.loadtxt('../../c++_dnest/SimulatedData/break2.txt')
img2 = np.loadtxt('../../c++_dnest/SimulatedData/crowded.txt')

scale = lambda i: (i.max() - i.min()) ** 0.4 - (i - i.min()) ** 0.4

fig = pl.figure(figsize=(11, 5))

pl.subplot(1,2,1)
pl.imshow(scale(img1), interpolation="nearest", cmap="gray")
pl.gca().set_xticks([0, 50, 100])
pl.gca().set_xticklabels(['-1', '0', '+1'])
pl.gca().set_yticks([0, 50, 100])
pl.gca().set_yticklabels(['+1', '0', '-1'])
pl.xlabel('$x$')
pl.ylabel('$y$')
pl.title('Test Case 1')

pl.subplot(1,2,2)
pl.imshow(scale(img2), interpolation="nearest", cmap="gray")
pl.gca().set_xticks([0, 50, 100])
pl.gca().set_xticklabels(['-1', '0', '+1'])
pl.gca().set_yticks([0, 50, 100])
pl.gca().set_yticklabels(['+1', '0', '-1'])
pl.xlabel('$x$')
pl.ylabel('$y$')
pl.title('Test Case 2')

pl.savefig('test_cases.eps', bbox_inches='tight')
