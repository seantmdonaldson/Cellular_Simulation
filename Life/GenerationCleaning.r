getData <- function(FILENAME) {
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

    v1 <- 1:(max-1)
    df <- data.frame(v1) 

    for (i in 1:nlines) {   
        vec <- data[i,1]
        parsed <- unlist(strsplit(vec,","))
        parsed = parsed[2:length(parsed)]
        parsed[length(parsed):(max-1)] <- NA
        df[i] = parsed
    }
    return(df)
}

getCount <- function(df){
    for (i in length(df)) {
       maxG = 0
       if (max(GenData[,i], na.rm = T) > maxG) {
          maxG = max(GenData[,i], na.rm = T)
       }
    }
    return(maxG)
}

CleanDF <- function(Data, generationCount){
    m <- matrix(nrow = length(Data)[1], ncol=(strtoi(generationCount, base = 0L)+1))
    for (j in 1:length(Data)) {
        print(j)
        for (g in 0:generationCount) {
            sum = 0
            for (i in 1:nrow(Data[j])) {
                if (!is.na(Data[i,j]) && Data[i,j] == g) {
                   sum = sum + 1
                }
            }
            m[j,(g+1)]=as.numeric(sum)
        }
        
    }
    df = as.data.frame(m)
    return(df)
}

GenData <- getData("Generation_Number.csv")
generations = getCount(GenData)
df <- CleanDF(GenData, generations)
saveRDS(df, file="Gen.Rda")
