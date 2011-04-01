data <- read.csv("radix1_cdf.csv",header=FALSE)
barplot(data[,2],col="violet",ylab="Frequency",xlab="Prefix-Length(level)",names.arg=data[,1],main="Routing table exact prefix matches")
