// sum of array

void main() {
    int64[] array
    array <- new Array(5)
    array[0] <- 2
    array[1] <- 3
    array[2] <- 0
    array[3] <- 0
    array[4] <- 0
    int64 res
    res <- sum(array, 5)
    print(res) // 5
    return
}

int64 sum(int64[] array, int64 len) {
    int64 i
    int64 res
    i <- 0
    res <- 0
    
    while (i < len) :intoWhile :exitWhile {
        :intoWhile
        int64 v
        v <- array[i]
        res <- res + v
        
        i <- i + 1
        continue
    }
    :exitWhile
    return res
}
