df = readRDS("Gen.Rda")

mn <- vector()
for (i in 1:length(df)) {
    count = 0
    sum = 0
   for (j in 1:nrow(df[i])) {
    if (df[j,i] != 0) {
        sum = sum + df[j,i]
        count = count + 1
    }
   }
   avg = sum/count
   mn[i] = avg
}
r <- t.test(mn)
#write.csv(mn, "GenerationMeans.csv")
jpeg(file="genHist.jpeg")
hist(mn, main="", breaks=7, xlab= "Mean Generation Size")
dev.off()
