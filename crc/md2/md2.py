#!/usr/bin/env python3
#
# RFC-1319
#
import sys

"""
RFC-1319
Permutation of 0..255 constructed from the digits of pi. It gives a
"random" nonlinear byte substitution operation.
"""
PI_SUBST = [
  41, 46, 67, 201, 162, 216, 124, 1, 61, 54, 84, 161, 236, 240, 6,
  19, 98, 167, 5, 243, 192, 199, 115, 140, 152, 147, 43, 217, 188,
  76, 130, 202, 30, 155, 87, 60, 253, 212, 224, 22, 103, 66, 111, 24,
  138, 23, 229, 18, 190, 78, 196, 214, 218, 158, 222, 73, 160, 251,
  245, 142, 187, 47, 238, 122, 169, 104, 121, 145, 21, 178, 7, 63,
  148, 194, 16, 137, 11, 34, 95, 33, 128, 127, 93, 154, 90, 144, 50,
  39, 53, 62, 204, 231, 191, 247, 151, 3, 255, 25, 48, 179, 72, 165,
  181, 209, 215, 94, 146, 42, 172, 86, 170, 198, 79, 184, 56, 210,
  150, 164, 125, 182, 118, 252, 107, 226, 156, 116, 4, 241, 69, 157,
  112, 89, 100, 113, 135, 32, 134, 91, 207, 101, 230, 45, 168, 2, 27,
  96, 37, 173, 174, 176, 185, 246, 28, 70, 97, 105, 52, 64, 126, 15,
  85, 71, 163, 35, 221, 81, 175, 58, 195, 92, 249, 206, 186, 197,
  234, 38, 44, 83, 13, 110, 133, 40, 132, 9, 211, 223, 205, 244, 65,
  129, 77, 82, 106, 220, 55, 200, 108, 193, 171, 250, 36, 225, 123,
  8, 12, 189, 177, 74, 120, 136, 149, 139, 227, 99, 232, 109, 233,
  203, 213, 254, 59, 0, 29, 57, 242, 239, 183, 14, 102, 88, 208, 228,
  166, 119, 114, 248, 235, 117, 75, 10, 49, 68, 80, 180, 143, 237,
  31, 26, 219, 153, 141, 51, 159, 17, 131, 20
]

class MessageDigest2:
    @classmethod
    def usage(cls):
        print("Usage: md2.py <file> [<file> ...]")
        return cls()


    @classmethod
    def sum(cls, filename):
        csum = cls(filename)
        csum.compute_message_digest()
        print("{} {}".format(filename, csum.hexdigest()))
        return csum

    @classmethod
    def test(cls):
        """
        >>> md2 = MessageDigest2()
        >>> md2._M = bytearray(b'The quick brown fox jumps over the lazy dog')
        >>> md2.compute_message_digest()
        >>> md2.hexdigest()
        '03d85a0d629d2c442e987525319fc471'
        >>> md2._M = bytearray(b'The quick brown fox jumps over the lazy cog')
        >>> md2.compute_message_digest()
        >>> md2.hexdigest()
        '6b890c9292668cdbbfda00a4ebf31f05'
        >>> md2._M = bytearray(b'')
        >>> md2.compute_message_digest()
        >>> md2.hexdigest()
        '8350e5a3e24c153df2275c9f80692773'
        """
        pass 

    def __init__(self, filename=None):
        self._filename = filename
        if self._filename is not None:
            self.get_message()  

    def get_message(self):
        with open(self._filename, 'rb') as fp:
            self._M = bytearray(fp.read())

    def pad_message(self):
        """
        >>> md2 = MessageDigest2()
        >>> md2._M = bytearray(b'0123456789')
        >>> md2.pad_message()
        >>> md2._M
        bytearray(b'0123456789\\x06\\x06\\x06\\x06\\x06\\x06')
        >>> md2._M = bytearray(b'aaaabbbbccccdddd')
        >>> md2.pad_message()
        >>> md2._M
        bytearray(b'aaaabbbbccccdddd\\x10\\x10\\x10\\x10\\x10\\x10\\x10\\x10\\x10\\x10\\x10\\x10\\x10\\x10\\x10\\x10')
        """
        pad_bytes = 16 - (len(self._M) % 16)
        self._M.extend([pad_bytes] * pad_bytes)

    def compute_checksum(self):
        """
        >>> md2 = MessageDigest2()
        >>> md2._M = bytearray(b'After killing\\r\\na spider, how lonely I feel\\r\\nin the cold of night!\\x0f\\x0f\\x0f\\x0f\\x0f\\x0f\\x0f\\x0f\\x0f\\x0f\\x0f\\x0f\\x0f\\x0f\\x0f')
        >>> md2.compute_checksum()
        >>> md2._C
        [167, 222, 15, 251, 92, 111, 246, 45, 79, 203, 11, 129, 245, 139, 5, 178]
        """
        self._C = [0] * 16
        L = 0
        j = 0
        for c in self._M:
            L = PI_SUBST[c ^ L] ^ self._C[j]
            self._C[j] = L
            j = (j + 1) % 16

    def append_checksum(self):
        self.compute_checksum()
        self._M.extend(self._C)

    def compute_block_round(self, m):
        for j in range(16):
            self._X[16 + j] = m[j]
            self._X[32 + j] = self._X[16 + j] ^ self._X[j]

        t = 0
        for j in range(18):
            for k in range(48):
                t = self._X[k] ^ PI_SUBST[t]
                self._X[k] = t

            t = (t + j) % 256


    def compute_block_rounds(self):
        self._X = [0] * 48
        for i in range(0,len(self._M),16):
            self.compute_block_round(self._M[i:i+16])

    def compute_message_digest(self):
        self.pad_message()
        self.append_checksum()
        self.compute_block_rounds()

    def get_checksum_bytes(self):
        return self._C

    def hexdigest(self):
        """
        >>> md2 = MessageDigest2()
        >>> md2._X = bytearray(b'0123456789abcdef')
        >>> md2.hexdigest()
        '30313233343536373839616263646566'
        """
        s = ""
        for val in self._X[0:16]:
            s += "{:02x}".format(val)
        return s

if __name__ == '__main__':
    if len(sys.argv) > 1:
        for file in sys.argv[1:]:
            MessageDigest2.sum(file)
    else:
        MessageDigest2.usage()