 void main() {
	int64[][] arr
	int64 arrsize, arrval,i,c,t

	
	arrsize <- 31
	arrval <- 3
	
	arr <- new Array(arrsize,arrval)
	arr[0] <- 43
	arr[1] <- 73
	arr[2] <- 29
	arr[3] <- 33
	arr[4] <- 47
	arr[5] <- 43
	arr[6] <- 81
	arr[7] <- 33
	arr[8] <- 63
	arr[9] <- 13
	arr[10] <- 63
	arr[11] <- 413
	arr[12] <- 93
	arr[13] <- 123
	arr[14] <- 14353
	
	i <- 0
	arrsize <- arrsize >> 1
	:loop
	t <- arr[i]
	print(t)
	i <- i +1
	while(i<arrsize) :loop :exit
	:exit
	return
}
