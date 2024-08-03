#include "increment.hh"

int increment(int num) {
    return num + 1;
}

int addition(int num1, int num2, ...) {
    int sum = num1 + num2;
    
    va_list args;
    va_start(args, num2);
    
    int nextNum;
    while ((nextNum = va_arg(args, int)) != 0) {
        sum += nextNum;
    }
    
    va_end(args);
    
    return sum;
}