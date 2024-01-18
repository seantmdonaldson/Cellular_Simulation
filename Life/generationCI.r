
file = "GenerationMeans.csv"

data = read.csv(file, header=FALSE, sep=",")

data = as.numeric(data[,2])
data = data[2:length(data)]

s = sd(data)
z = 1.96
x = mean(data)

CI = c(x - z*(s/sqrt(length(data))), x + z*(s/sqrt(length(data))))

print(x)
print(CI)
print(t.test(data))
