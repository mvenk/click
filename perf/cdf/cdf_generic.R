args <- commandArgs()
file<-args[3]
title<-args[4]
data <- read.csv(file,header=FALSE)
output_file = paste(title,".jpg")
jpeg(output_file, width = 600,height =400 ,pointsize = 12, bg = "white")
barplot(data[,2],col="violet",ylab="Frequency",xlab="Prefix-length (level)",names.arg=data[,1],main=title)
dev.off()
print (paste("output file is",output_file))
q()
