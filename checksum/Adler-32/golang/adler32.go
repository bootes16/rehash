package main

//
// Adler32 compute and return the 32-bit Adler checksum for a string.
//
func Adler32(data []byte) uint32 {
    var a, b uint32 = 1, 0

    for _, c := range data {
        a = (a + uint32(c)) % 65521
        b = (b + a) % 65521
    }

    return (b << 16) | a
}
