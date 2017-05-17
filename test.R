## test the code from IRLS with Poisson regression

## proj.dir <- "~/src/IRLS"; setwd(proj.dir)
rm(list=ls()); set.seed(1859)

N <- 200                    # nb of samples
P <- 5                      # nb of predictors
mu <- 0.2                   # intercept
## X <- replicate(P, sample(0:2,N,replace=T))
X <- replicate(P, rnorm(N)) # values of the predictors
beta <- rnorm(P)            # effect sizes of the predictors
beta[sample(1:P,1)] <- 0    # choose some predictors with no effect
y <- rep(NA, N)

sigma <- 0.5                # variance of an extra Gaussian term for over-dispersion
offset <- rnorm(N, sd=1)    # fix sd=0 to have no offset

for(i in 1:N){
  lambda <- exp(offset[i] + mu + sum(X[i,] * beta) + rnorm(1,sd=sigma))
  y[i] <- rpois(1, lambda)
}

con <- file(description="test_y.dat", open="wb")
writeBin(object=as.numeric(y), con=con)
close(con)

con <- file(description="test_X.dat", open="wb")
for(i in 1:N)
  writeBin(object=as.vector(c(1, X[i,]), mode="numeric"), con=con)
close(con)

con <- file(description="test_offset.dat", open="wb")
writeBin(object=offset, con=con)
close(con)

if(sigma == 0){
  fit <- glm(formula=y ~ X, family=poisson, offset=offset)
} else
  fit <- glm(formula=y ~ X, family=quasipoisson, offset=offset)
summary(fit)$dispersion
coefficients(summary(fit))

if(file.exists("test_irls"))
  file.remove("test_irls")
system("make test_irls")

if(sigma == 0){
  system("./test_irls --off test_offset.dat")
} else
  system("./test_irls --off test_offset.dat -q")

quit(save="no")

##-----------------------------------------------------------------------------
## custom implementation of IRLS for quick prototyping in R
## Freely available online explanations at:
## http://sfb649.wiwi.hu-berlin.de/fedc_homepage/xplore/ebooks/html/spm/spmhtmlnode27.html

irls <- function(y, X, threshold=1e-6, family, offset=NULL, verbose=0){
  stopifnot(is.matrix(X), is.vector(y), nrow(X) == length(y),
            length(grep("poisson", family)) != 0)
  if(! is.null(offset)){
    stopifnot(length(offset) == nrow(X))
  } else
    offset <- rep(0, nrow(X))

  ## internal functions ---------------
  init.mu <- function(y, family){
    if(family != "binomial"){
      mu <- y
      mu[mu == 0] <- 0.01
    } else{
      mu <- (y + 0.5) / (k + 1) # k: nb of trials
    }
    return(mu)
  }
  compute.z <- function(y, mu, family, offset){
    if(length(grep("poisson", family)) != 0){
      return(log(mu) + (y - mu) * (1/mu) - offset)
    }
  }
  compute.weights <- function(mu, family){
    if(length(grep("poisson", family)) != 0){
      return(as.vector(mu))
    }
  }
  weighted.leastsquare.fit <- function(X, w, z){
    fit <- lm(z ~ X[,-1], weights=w)
    chisq <- sum(w * fit$residuals^2)
    return(list(rank=fit$rank, chisq=chisq, beta.hat=as.numeric(coefficients(fit))))
  }
  compute.mu <- function(beta, X, offset){
    return(exp(X %*% beta + offset))
  }
  compute.variance <- function(X, w, family, offset, y=NULL, beta=NULL, rank=NULL){
    if(length(grep("poisson", family)) != 0){
      if(family == "poisson"){
        return(list(cov=solve(t(X) %*% diag(w) %*% X), sigma2=1))
      } else if(family == "quasipoisson"){
        mu <- as.vector(compute.mu(beta, X, offset))
        sigma2 <- (1/(nrow(X) - rank)) * sum((y - mu)^2 / mu)
        return(list(cov=sigma2 * solve(t(X) %*% diag(w) %*% X), sigma2=sigma2))
      }
    }
  }
  compute.pvalue <- function(family, beta.hat, se.beta.hat, N, rank){
    if(family == "poisson"){
      return(2 * pnorm(-abs(beta.hat / se.beta.hat)))
    } else if(family == "quasipoisson"){
      return(2 * pt(-abs(beta.hat / se.beta.hat), df=N-rank))
    }
  }
  ##-----------------------------------

  mu <- init.mu(y, family)
  old.chisq <- -1
  nb.iters <- 0

  while(TRUE){
    if(verbose > 0)
      message(paste0("iter ", nb.iters))
    z <- compute.z(y, mu, family, offset)
    w <- compute.weights(mu, family)
    wls.fit <- weighted.leastsquare.fit(X, w, z)
    if(verbose > 0){
      tmp <- paste0("chisq=", format(wls.fit$chisq, digits=10))
      tmp <- paste0(tmp, " beta.hat=(", format(wls.fit$beta.hat[1], digits=6))
      for(beta in wls.fit$beta.hat[-1])
        tmp <- paste0(tmp, ",", format(beta, digits=6))
      tmp <- paste0(tmp, ")")
      message(tmp)
    }
    if(abs(wls.fit$chisq - old.chisq) < threshold){
      var.beta.hat <- compute.variance(X, w, family, offset, y,
                                       wls.fit$beta.hat,
                                       wls.fit$rank)
      pval.beta.hat <- compute.pvalue(family, wls.fit$beta.hat,
                                      sqrt(diag(var.beta.hat$cov)), length(y),
                                      wls.fit$rank)
      break
    }
    old.chisq <- wls.fit$chisq
    mu <- compute.mu(wls.fit$beta.hat, X, offset)
    nb.iters <- nb.iters + 1
  }

  return(list(beta.hat=wls.fit$beta.hat,
              se.beta.hat=sqrt(diag(var.beta.hat$cov)),
              pval.beta.hat=pval.beta.hat,
              sigma2=var.beta.hat$sigma2,
              chisq=wls.fit$chisq,
              nb.iters=nb.iters))
}

if(sigma == 0){
  fit.custom <- irls(y, cbind(rep(1,N),X), family="poisson", offset=offset, verbose=0)
} else
  fit.custom <- irls(y, cbind(rep(1,N),X), family="quasipoisson", offset=offset, verbose=0)
