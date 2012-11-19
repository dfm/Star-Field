library(rjags)

# Load the "sparse" dataset
data <- as.matrix(read.table('../c++_dnest/SimulatedData/sparse.txt', header=FALSE))
data <- list(F=data)

# Create the JAGS model
m <- jags.model(file = "model.txt", data=data)

# Run JAGS (no burn-in)
draw <- coda.samples(m, 10000, thin=10, variable.names=c("alpha"))

# Turn results into matrix
samples <- as.matrix(draw)

