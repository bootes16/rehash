package main

import (
    "fmt"
    "os"
)

func checksumFile(fileName string) {
    file, err := os.Open(fileName)
    if err == nil {
        fmt.Printf("Error openning file %s\n", fileName)
        return
    }
    defer file.Close()

    fmt.Printf("%d %d %s\n", Adler32([]byte(fileName)), len(fileName), fileName)
}

func main() {
    for _, arg := range os.Args[1:] {
        checksumFile(arg)
    }
}
