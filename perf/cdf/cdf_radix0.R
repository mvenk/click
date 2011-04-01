data <- read.csv("radix0_cdf.csv",header=FALSE)
barplot(data[,2],col="violet",ylab="Frequency",xlab="Prefix-length (level)",names.arg=data[,1],main="Levels accessed in radix tree")
