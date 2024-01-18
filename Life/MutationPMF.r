library(latex2exp)
library(Rlab)

p <- (3/26)
n <- 54
x <- 0:n

pmf <- dbinom(0:n, size = n, prob = p)
jpeg(file="pmf.jpeg")
plot( pmf, type = "h", lwd = 2, col = "#2a2a2a", las = 1)
dev.off()
print(max(pmf))
print(pmf)

cdf <- pbinom(0:n, size = n, prob = p)
jpeg(file="cdf.jpeg")
plot(cdf, type = "h", lwd = 2,col = "#2a2a2a")
dev.off()