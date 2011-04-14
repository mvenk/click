args <- commandArgs()
input_file<-args[3]
output_file<-args[4]
data <- read.csv(input_file)
lookup_time <- data$radix.time.s.-data$setup.time.s.
lookup_percentage <- NULL
total_percentage <-NULL
memory_percentage<-NULL
for (x in c(1:length(lookup_time)))
{
     lookup_percentage <-c(lookup_percentage,(lookup_time[1]-lookup_time[x])/lookup_time[1] *100)
     total_percentage <-c(total_percentage,(data$radix.time.s[1] - data$radix.time.s[x])/data$radix.time.s[1] *100)
     memory_percentage<-c(memory_percentage,(data$radix.memory.kb[1] - data$radix.memory.kb[x])/data$radix.memory.kb[1] *100)
}
data$radix.filename<-NULL
data$setup.filename<-NULL
data$lookup_percentage <- lookup_percentage
data$total_percentage <-total_percentage
data$memory_percentage<-memory_percentage
#write.csv2(data,file="test.csv", dec=".",col.names = TRUE, sep = ",")
write.table(data, file = output_file, append = FALSE, quote = TRUE, sep = ",",
            eol = "\n", na = "NA", dec = ".", row.names = FALSE,
            col.names = TRUE, qmethod = c("escape", "double"))
