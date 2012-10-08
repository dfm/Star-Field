from pylab import *

img1 = loadtxt('../../c++_dnest/SimulatedData/break2.txt')
img2 = loadtxt('../../c++_dnest/SimulatedData/crowded.txt')

subplot(1,2,1)
imshow((img1 - img1.min())**0.4)
gca().set_xticks([0, 50, 100])
gca().set_xticklabels(['-1', '0', '+1'])
gca().set_yticks([0, 50, 100])
gca().set_yticklabels(['+1', '0', '-1'])
xlabel('$x$', fontsize=16)
ylabel('$y$', fontsize=16)
title('Test Case 1', fontsize=14)

subplot(1,2,2)
imshow((img2 - img2.min())**0.4)
gca().set_xticks([0, 50, 100])
gca().set_xticklabels(['-1', '0', '+1'])
gca().set_yticks([0, 50, 100])
gca().set_yticklabels(['+1', '0', '-1'])
xlabel('$x$', fontsize=16)
ylabel('$y$', fontsize=16)
title('Test Case 2', fontsize=14)

savefig('test_cases.eps', bbox_inches='tight')
show()

