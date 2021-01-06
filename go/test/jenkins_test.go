package main

import (
	"bytes"
	"fmt"
	"io"
	"strings"
	"testing"

	"../rehash/jenkins"
)

type testCase struct {
	data []byte
	hash uint32
}

var testData []testCase = []testCase{
	{[]byte("a"), 0xca2e9442},
	{[]byte("The quick brown fox jumps over the lazy dog"), 0x519e91f5},
}

func TestJenkins1(t *testing.T) {
	for _, tc := range testData {
		hash := jenkins.New()
		d := hash.Sum(tc.data)
		fmt.Printf("%x %x %x %x\n", d[0], d[1], d[2], d[3])
		sum := (uint32(d[0]) << 24) | (uint32(d[1]) << 16) | (uint32(d[2]) << 8) | uint32(d[3])
		if sum != tc.hash {
			t.Errorf("expected: 0x%08x calculated: 0x%08x\n", tc.hash, sum)
		}
	}
}

func TestJenkins2(t *testing.T) {
	for _, tc := range testData {
		hash := jenkins.New()
		reader := bytes.NewReader(tc.data)
		io.Copy(hash, reader)
		sum := hash.Sum32()
		if sum != tc.hash {
			t.Errorf("expected: 0x%08x calculated: 0x%08x\n", tc.hash, sum)
		}
	}
}

func TestJenkins3(t *testing.T) {
	expect := uint32(0x519e91f5)
	reader := strings.NewReader("The quick brown fox jumps over the lazy dog")
	hash := jenkins.New()
	io.CopyN(hash, reader, 8)
	io.CopyN(hash, reader, 8)
	io.Copy(hash, reader)
	sum := hash.Sum32()
	if sum != expect {
		t.Errorf("expected: 0x%08x calculated: 0x%08x\n", expect, sum)
	}
}

func TestJenkinsNonMemberSum(t *testing.T) {
	for _, tc := range testData {
		s := fmt.Sprintf("%08x %08x", jenkins.Sum(tc.data), tc.hash)
		fmt.Println(s)
	}
}
