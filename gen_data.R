p = 5
n = 200
X.tr = matrix(ncol=p, rnorm(p*n))
B = matrix(ncol=1, runif(p, min=3,max=5))
Y.tr = round(X.tr%*%B + 3 + rnorm(n))
minY = min(Y.tr)
if(minY<1){
   Y.tr = Y.tr-minY+1
}

rsv = paste("rs",seq(1,p),sep="")
dX = rbind(rsv,X.tr)
dY = rbind(c("F"),Y.tr)
write.table(file="test.dat",t(dY), quote = FALSE, append=FALSE, sep=" ",row.names=FALSE, col.names=FALSE)
write.table(file="test.dat",t(dX), quote = FALSE, append=TRUE, sep=" ",row.names=FALSE, col.names=FALSE)

write.table(file="test.r.dat",cbind(Y.tr,X.tr),quote = FALSE, append=FALSE, sep=" ",row.names=FALSE, col.names=FALSE)


summary(glm(Y.tr~X.tr,family="poisson"))

