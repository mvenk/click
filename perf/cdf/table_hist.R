data <- read.csv("dist.txt",header=FALSE)
barplot(data[,2],col="violet",ylab="Frequency",xlab="Prefix-length",names.arg=data[,1],main="167k Routing Table")

