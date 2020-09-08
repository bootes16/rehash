def MakeByteArray(charArray):
    if isinstance(charArray, str):
        return bytearray(charArray, 'utf-8')

    return charArray

def Adler32(data):
    ra = MakeByteArray(data)
    a, b = 1, 0
    for i in range(len(data)):
        a = (a + ra[i]) % 65521
        b = (b + a) % 65521

    return (b << 16) | a
