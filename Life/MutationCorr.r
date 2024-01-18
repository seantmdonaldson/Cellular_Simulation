repoMeans = "Reproduction_means.csv"
foodMeans = "FoodQuality_means.csv"
BurnMeans = "BurnRate_means.csv"
BirthPenMeans = "BirthPenalty_means.csv"

rmeans = read.csv(repoMeans, header=FALSE, sep=',')
fmeans = read.csv(foodMeans, header=FALSE, sep=',')
bmeans = read.csv(BurnMeans, header=FALSE, sep=',')
bpmeans = read.csv(BirthPenMeans, header=FALSE, sep=',')
pop = read.csv("Population_data.csv", header=FALSE, sep=',')
food = read.csv("Food_data.csv", header=FALSE, sep=',')

x = rmeans[1,1]
n=6
means = matrix(ncol = n, nrow=length(rmeans))

for (i in 1:length(rmeans)) {
   means[i,1] = rmeans[1,i]
   means[i,2] = fmeans[1,i]
   means[i,3] = bmeans[1,i]
   means[i,4] = bpmeans[1,i]
   means[i,5] = pop[i,2]
   means[i,6] = food[i,2]
}

names <- c("RL", "FQ", "BR", "BP", "P", "FT")


m2 = matrix(ncol = n, nrow = n)

for (j in 1:n) {
    for (i in 1:n) {
        m2[j,i] = cor(means[,j], means[,i])
    }
}

df = data.frame(m2)
colnames(df) = names
rownames(df) = names
#write.csv(df, "Correlation.csv")
print(t.test(pop[,2]))