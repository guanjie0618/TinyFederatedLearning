import random

a = [1,2,3,4,5,6,7,8,9]
b = [1,2,3,4,5,6,7,8,9]
c = [1,2,3,4,5,6,7,8,9]

random.seed(3)
random.shuffle(a)
random.shuffle(b)
random.shuffle(c)
print(a)
print(b)
print(c)