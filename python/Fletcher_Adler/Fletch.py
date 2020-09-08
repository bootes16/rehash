class Fletch:

    @classmethod
    def Fletch16(cls):
        return cls(255, 8, hexFmt='{:04x}', strFmt='{0:04x} {0}')

    @classmethod
    def Fletch32(cls):
        return cls(65535, 16, 2, hexFmt='{:08x}', strFmt='{0:08x} {0}')

    @classmethod
    def Fletch64(cls):
        return cls(0xffffffff, 32, 4, hexFmt='{:016x}', strFmt='{0:016x} {0}')

    @classmethod
    def Adler32(cls):
        return cls(65521, 16, c0=1, hexFmt='{:08x}', strFmt='{0:08x} {0}')

    def __init__(self, modulus, crcBits, wordBytes=1, c0=0, c1=0, hexFmt='{:x}', strFmt='{0:x} {0}'):
        self.modulus = modulus
        self.crcBits = crcBits
        self.wordBytes = wordBytes
        self.c0 = c0
        self.c1 = c1
        self.hexFmt = hexFmt
        self.strFmt = strFmt

    def addValue(self, value):
        self.c0 = (self.c0 + value) % self.modulus
        self.c1 = (self.c1 + self.c0) % self.modulus
        return self

    def _addArray(self, ra):
        sz = len(ra) // self.wordBytes
        sz *= self.wordBytes
        rem = len(ra) % self.wordBytes
        for i in range(0, sz, self.wordBytes):
            self._addAsLittleEndian(*ra[i:i+self.wordBytes])

        if 0 < rem:
            self._addAsLittleEndian(*ra[-rem:])

        return self

    def add(self, *ra):
        if 0 == len(ra):
            return self

        if len(ra) > 1:
            return self._addArray(ra)

        val = ra[0]
        if isinstance(val, int):
            return self._addArray(val);

        if isinstance(val, str):
            return self._addArray(bytearray(val, 'utf-8'))

        return self._addArray(val)

    def _addAsLittleEndian(self, *byteVals):
        leWord = 0
        for i,ch in enumerate(byteVals):
            leWord += ch << (i*8)

        self.addValue(leWord)
        return self

    def __add__(self, value):
        return self.addValue(value)

    def crc(self):
        return (self.c1 << self.crcBits) | self.c0

    def hexDigest(self):
        return self.hexFmt.format(self.crc())

    def __str__(self):
        return self.strFmt.format(self.crc())

    def __repr__(self):
        return self.__str__()