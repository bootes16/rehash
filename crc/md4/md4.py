#!/usr/bin/env python3
#
# MD4 message digest algorithm
# RFC 1320 (https://tools.ietf.org/html/rfc1320)
#
# The MD4 message digest algorithm produces a 128-bit message digest of the input.
#

#
# F, G and H are basic MD4 functions.
#
def FN(x, y, z):
    """ Bitwise if x then y else z """
    return (x & y) | (~x & z)

def GN(x, y, z):
    """ Bitwise majority if at least two of the same bit is set, the result is set. """
    return (x & y) | (x & z) | (y & z)

def HN(x, y, z):
    """ Bitwise odd-parity """
    return x ^ y ^ z

#
# ROTATE_LEFT rotates x left n bits.
#
def ROTATE_LEFT(x, n):
    return ((x << n) & 0xffffffff) | ((x >> (32 -n)) & 0xffffffff)

#
# FF, GG and HH are transformations for rounds 1, 2 and 3
# Rotation is separate from addition to prevent recomputation
#
def ffn(a, b, c, d, x, s):
    a += FN(b, c, d) + x
    a &= 0xffffffff
    return ROTATE_LEFT(a, s)

def ggn(a, b, c, d, x, s):
    a += GN(b, c, d) + x + 0x5a827999
    a &= 0xffffffff
    return ROTATE_LEFT(a, s)

def hhn(a, b, c, d, x, s):
    a += HN(b, c, d) + x + 0x6ed9eba1
    a &= 0xffffffff
    return ROTATE_LEFT(a, s)

class MessageDigest4:

    @classmethod
    def string_digest(cls, msg):
        md4 = cls()
        md4.append(msg)
        md4.finalise()
        return md4.hexdigest()

    def __init__(self):
        self._M = bytearray(b'')
        self._message_length = 0
        self.state = (0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476)

    def append(self, msg):
        self._M.extend(bytearray(msg, encoding="utf-8"))

    def append_padding(self):
        self._message_length = len(self._M)
        pad_len = 64 - ((len(self._M) + 8) % 64) - 1
        self._M.extend([0x80])
        self._M.extend([0] * pad_len)

    def append_length(self):
        self._M.extend([((self._message_length * 8) >> s) & 0xff for s in range(0,64,8)])

    def ffn(self, X, k, s):
        self._A = ROTATE_LEFT( self._A + FN(self._b, self._c, self._d) + X[k], s)

    def process_block(self, X):
        """ Process a single 16 4-bit word block in rounds 1-3. """
        A, B, C, D = self.state
        S = self.state

        for ix in [0, 4, 8, 12]:
            A = ffn(A, B, C, D, X[ix+0], 3)
            D = ffn(D, A, B, C, X[ix+1], 7)
            C = ffn(C, D, A, B, X[ix+2], 11)
            B = ffn(B, C, D, A, X[ix+3], 19)

        for ix in [0, 1, 2, 3]:
            A = ggn(A, B, C, D, X[ix+0], 3)
            D = ggn(D, A, B, C, X[ix+4], 5)
            C = ggn(C, D, A, B, X[ix+8], 9)
            B = ggn(B, C, D, A, X[ix+12], 13)

        for ix in [0, 2, 1, 3]:
            A = hhn(A, B, C, D, X[ix+0], 3)
            D = hhn(D, A, B, C, X[ix+8], 9)
            C = hhn(C, D, A, B, X[ix+4], 11)
            B = hhn(B, C, D, A, X[ix+12], 15)

        A = (A + S[0]) & 0xffffffff
        B = (B + S[1]) & 0xffffffff
        C = (C + S[2]) & 0xffffffff
        D = (D + S[3]) & 0xffffffff

        self.state = (A, B, C, D)

    def decode(self, buf):
        output = []
        for ix in range(0, len(buf), 4):
            output.append(buf[ix] | (buf[ix+1] << 8) | (buf[ix+2] << 16) | (buf[ix+3] << 24))

        return output

    def process_message_data(self):
        for i in range(0, len(self._M), 64):
            self.process_block(self.decode(self._M[i:i+64]))

    def hexdigest(self):
        digest = ""
        for s in self.state:
            digest += "{:02x}{:02x}{:02x}{:02x}".format(
                s & 0xff, (s >> 8) & 0xff, (s >> 16) & 0xff, (s >> 24) & 0xff)

        return digest

    def finalise(self):
        self.append_padding()
        self.append_length()
        self.process_message_data()
