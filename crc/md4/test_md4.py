#!/usr/bin/env python3
#
# MD4 message digest algorithm
# RFC 1320 (https://tools.ietf.org/html/rfc1320)
#
# The MD4 message digest algorithm produces a 128-bit message digest of the input.
#
import pytest
from md4 import *

def test_F():
    assert FN(0xffff0000, 0x00ffff00, 0x000000ff) == 0x00ff00ff

def test_G():
    assert GN(0xff000000, 0x0ff000ff, 0x00ff00ff) == 0x0ff000ff

def test_H():
    assert HN(0xffa00000, 0x005ff500, 0x00000aff) == 0xffffffff
    assert HN(0xffff0000, 0x00ffff00, 0x0000ffff) == 0xff0000ff

def test_ROTATE_LEFT():
    assert ROTATE_LEFT(0xff000000, 4) == 0xf000000f
    assert ROTATE_LEFT(0x000ff000, 16) == 0xf000000f
    assert ROTATE_LEFT(0xf000000f, 32) == 0xf000000f

def test_ffn():
    A, B, C, D = 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476
    assert ffn(A, B, C, D, 0x00008061, 3) == 0x00040300


class TestMD4:
    @classmethod
    def setup_method(cls):
        cls.md4 = MessageDigest4()

    def test_md_buffer_init(self):
        assert self.md4.state[0] == 0x67452301
        assert self.md4.state[1] == 0xefcdab89
        assert self.md4.state[2] == 0x98badcfe
        assert self.md4.state[3] == 0x10325476

    def test_append_padding(self):
        msg = bytearray(b'sixteen bytes...')
        self.md4._M = bytearray(msg)
        self.md4.append_padding()
        msg.append(0x80)
        msg.extend([0] * (64 - 8 - 1 - 16))
        assert self.md4._M == msg

    def test_append_length(self):
        msg = bytearray(b'sixteen bytes...')
        self.md4._M = bytearray(msg)
        self.md4.append_padding()
        self.md4.append_length()
        msg.append(0x80)
        msg.extend([0] * (64 - 8 - 1 - 16))
        msg.extend([16 * 8,0,0,0,0,0,0,0])
        assert self.md4._M == msg
        assert len(self.md4._M) == 64

    def test_process_block(self):
        buf = [
            0x00008061, 0x00000000, 0x00000000, 0x00000000,
            0x00000000, 0x00000000, 0x00000000, 0x00000000,
            0x00000000, 0x00000000, 0x00000000, 0x00000000,
            0x00000000, 0x00000000, 0x00000008, 0x00000000
            ]

        self.md4.process_block(buf)
        assert '{:x} {:x} {:x} {:x}'.format(*self.md4.state) == 'b32ce5bd 463ee31d fb055e24 24fbd6db'

    # @pytest.mark.skip(reason = 'Not ready yet.')
    def test_process_message_data(self):
        msg = bytearray(b'a')
        msg.append(0x80)
        msg.extend([0] * (64 - 8 - 1 - 1))
        msg.extend([8, 0, 0, 0, 0, 0, 0, 0])
        self.md4._M = msg

        self.md4.process_message_data()
        assert '{:x} {:x} {:x} {:x}'.format(*self.md4.state) == 'b32ce5bd 463ee31d fb055e24 24fbd6db'

    def test_decode(self):
        msg = bytearray(b'a')
        msg.append(0x80)
        msg.extend([0] * (64 - 8 - 1 - 1))
        msg.extend([8, 0, 0, 0, 0, 0, 0, 0])

        block_data = [
            0x00008061, 0x00000000, 0x00000000, 0x00000000,
            0x00000000, 0x00000000, 0x00000000, 0x00000000,
            0x00000000, 0x00000000, 0x00000000, 0x00000000,
            0x00000000, 0x00000000, 0x00000008, 0x00000000
            ]

        block = self.md4.decode(msg)
        assert block == block_data

    def test_hexdigest(self):
        self.md4.state = [0xb32ce5bd, 0x463ee31d, 0xfb055e24, 0x24fbd6db]
        assert self.md4.hexdigest() == 'bde52cb31de33e46245e05fbdbd6fb24'

    def test_all(self):
        self.md4.append('a')
        self.md4.append_padding()
        self.md4.append_length()
        self.md4.process_message_data()
        assert self.md4.hexdigest() == 'bde52cb31de33e46245e05fbdbd6fb24'

    def test_string_digest(self):
        assert MessageDigest4.string_digest('') == '31d6cfe0d16ae931b73c59d7e0c089c0'
        assert MessageDigest4.string_digest('a') == 'bde52cb31de33e46245e05fbdbd6fb24'
        assert MessageDigest4.string_digest('abc') == 'a448017aaf21d8525fc10ae87aa6729d'
        assert MessageDigest4.string_digest('message digest') == 'd9130a8164549fe818874806e1c7014b'
        assert MessageDigest4.string_digest('abcdefghijklmnopqrstuvwxyz') == 'd79e1c308aa5bbcdeea8ed63df412da9'
        assert MessageDigest4.string_digest('ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789') == '043f8582f241db351ce627e153e7f0e4'
        assert MessageDigest4.string_digest('12345678901234567890123456789012345678901234567890123456789012345678901234567890') == 'e33b4ddc9c38f2199c3e7b164fcc0536'