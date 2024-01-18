

pop = read.csv("Population_data.csv", header=FALSE, sep=',')
print(t.test(pop[,2]))