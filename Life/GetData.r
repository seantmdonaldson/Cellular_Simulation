library(data.table)
file = "Generation_Number.csv"
no_col <- max(count.fields(file, sep = ","))
data <- read.table(file,sep="\t",fill=TRUE)

f <- file(file, open="rb")
nlines <- 0L
while (length(chunk <- readBin(f, "raw", 65536)) > 0) {
   nlines <- nlines + sum(chunk == as.raw(10L))
}

max = 0
for (i in 1:nlines) {   
    vec <- data[i,1]
    parsed <- unlist(strsplit(vec,","))
    if (length(parsed) > max) {
       max = length(parsed)
    }
}

t <- 1:(max-1)
df <- data.frame(t) 

for (i in 1:nlines) {   
    vec <- data[i,1]
    parsed <- unlist(strsplit(vec,","))
    parsed = parsed[2:length(parsed)]
    parsed[length(parsed):(max-1)] <- NA
    df[i] = parsed
}

#View(df)