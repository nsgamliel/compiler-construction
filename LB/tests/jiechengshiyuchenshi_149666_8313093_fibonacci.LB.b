// fibonacci
// iterative approach

int64 main() {
    int64 res
    res <- fibonacci(20)
    print(res) // 6765
    return
}

int64 fibonacci(int64 k) {
    if (k = 1) :initialConditionTrue :initialConditionFalse
    
    :initialConditionFalse
    if (k = 2) :initialConditionTrue :initialConditionFalse2
    
    :initialConditionFalse2
    int64 a
    int64 b
    int64 i
    int64 res
    a <- 1
    b <- 1
    i <- 3
    res <- 0
    
    while (i <= k) :intoWhile :exitWhile {
        :intoWhile
        res <- a
        res <- res + b
        a <- b
        b <- res
        i <- i + 1
        continue
    }
    :exitWhile
    return res
    
    :initialConditionTrue
    return 1
}
