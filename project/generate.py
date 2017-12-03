from random import randint
file = open('data/value_1000000.txt','w')
for i in range(0, 1000000):
	weight = randint(1, 10)
	input = str(weight) + " "
	file.write(input)
file.close

