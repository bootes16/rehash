//
// Package jenkins implements the Jenkins 32-bit non-cryptographic hash.
//
package jenkins

import (
	"hash"
)

const size = 4
const blockSize = 1

type digest struct {
	count int
	hash  uint32
}

// Reset sets the context to initial values.
func (d *digest) Reset() {
	d.count = 0
	d.hash = 0
}

// New creates and returns a new Jenkins hash context.
func New() hash.Hash32 {
	return new(digest)
}

// // update the hash with the input data.
// func (d *digest) update(data []byte) {
// 	for _, c := range data {
// 		d.hash += uint32(c)
// 		d.hash += d.hash << 10
// 		d.hash ^= d.hash >> 6
// 	}
// 	d.count += len(data)
// }

// finalise the 32-bit hash value and return value.
func finalise(hash uint32) uint32 {
	hash += hash << 3
	hash ^= hash >> 11
	hash += hash << 15
	return hash
}

// Writer interface for jenkins hash.
func (d *digest) Write(data []byte) (n int, err error) {
	for _, c := range data {
		d.hash += uint32(c)
		d.hash += d.hash << 10
		d.hash ^= d.hash >> 6
	}
	d.count += len(data)
	return len(data), nil
}

// Sum32 finalises and retuns the hash as a 32-bit unsigned int.
func (d *digest) Sum32() uint32 {
	return finalise(d.hash)
}

func uint32toslice(hash uint32) []byte {
	out := make([]byte, 4)
	// out = append(nil, byte(hash>>24), byte(hash>>16), byte(hash>>8), byte(hash))
	out[3] = byte(hash)
	out[2] = byte(hash >> 8)
	out[1] = byte(hash >> 16)
	out[0] = byte(hash >> 24)
	return out
}

// Sum copies the context d, updates the copy with any new data,
// finalises and returns a byte slice digest of the hash.
func (d *digest) Sum(data []byte) []byte {
	d2 := *d
	if data != nil {
		d2.Write(data)
	}

	hash := finalise(d2.hash)
	return uint32toslice(hash)
}

// Sum computes new hash from data and returns byte slice digest.
func Sum(data []byte) []byte {
	var d digest
	d.Write(data)
	hash := finalise(d.hash)
	return uint32toslice(hash)
}

// Size returns the size of the output data in bytes.
func (d *digest) Size() int {
	return size
}

// BlockSize returns the algorithm block size.
func (d *digest) BlockSize() int {
	return blockSize
}
