def MakeByteArray(charArray):
    if isinstance(charArray, str):
        return bytearray(charArray, 'utf-8')

    return charArray

def Fletcher16(data):
    """
    >>> Fletcher16(b'\x00\x00\x00\x00')
    1
    """
    ra = MakeByteArray(data)
    sum1 = 0
    sum2 = 0

    for i in range(len(ra)):
        val = ra[i]
        sum1 = (sum1 + val) % 255
        sum2 = (sum2 + sum1) % 255

    return (sum2 << 8) | sum1

def Fletcher32(data):
    ra = MakeByteArray(data)
    c0 = 0
    c1 = 0

    for i in range(0, len(ra), 2):
        val = (ra[i+1] << 8) + ra[i] if (i+1) != len(ra) else ra[i]
        c0 = (c0 + val) % 65535
        c1 = (c1 + c0) % 65535

    return (c1 << 16) | c0

def Fletcher64(data):
    ra = MakeByteArray(data)
    c0, c1 = 0, 0
    sz = 4 * (len(ra) // 4)
    rm = len(ra) % 4
    for i in range(0, sz, 4):
        val = ra[i] + (ra[i+1] << 8) + (ra[i+2] << 16) + (ra[i+3] << 24)
        c0 = (c0 + val) % 0xffffffff
        c1 = (c1 + c0) % 0xffffffff

    if rm != 0:
        val = ra[sz]

        if rm >= 2:
            val += ra[sz+1] << 8

        if rm == 3:
            val += ra[sz+2] << 16

        c0 = (c0 + val) % 0xffffffff
        c1 = (c1 + c0) % 0xffffffff

    return (c1 << 32) | c0

    if __name__ == "__main__":
        import doctest
        doctest.testmod(name='Fletcher16')