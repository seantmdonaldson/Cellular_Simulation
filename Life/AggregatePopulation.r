
popData = read.csv("Population_data.csv", header=FALSE, sep=",")


CleanData = popData[,2]
df = data.frame(CleanData)

CI = t.test(df$CleanData)
