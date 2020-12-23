package main

import (
    "testing"
)

type s struct {
    csum  uint32
    input string
}

func TestAdler32(t *testing.T) {
    var data = []s{
        s{0x00000001, ""},
        s{0x00620062, "a"},
        s{0x024d0127, "abc"},
        s{0x29750586, "message digest"},
        s{0x90860b20, "abcdefghijklmnopqrstuvwxyz"},
        s{0x8adb150c, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"},
        s{0x97b61069, "12345678901234567890123456789012345678901234567890123456789012345678901234567890"},
    }

    for _, d := range data {
        result := Adler32([]byte(d.input))
        if d.csum != result {
            t.Errorf("result: %08x expected: %08x for %s\n", result, d.csum, d.input)
        }
    }
}

var data []byte = make([]byte, 64, 64)
var dataS string = "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"

func BenchmarkAdler32(b *testing.B) {
    for i := 0; i < b.N; i++ {
        Adler32(data)
    }
}
