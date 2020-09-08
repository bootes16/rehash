TestVectorSet1 = {
    'input' : [
        'abcde',
        'abcdef',
        'abcdefgh',
        b'\x00\x00\x00\x00',
        b'\xff\xff\xff\xff'
    ],
    'Fletcher16' : [
        51440,
        8279,
        1575,
        0,
        0
    ],
    'Fletcher32' : [
        4031760169,
        1448095018,
        3957429649,
        0,
        0
    ],
    'Fletcher64' : [
        14467467625952928454,
        14467579776138987718,
        3543817411021686982,
        0,
        0
    ]
}

TestVectorSet2 = {
    'input' : [
        "",
        "a",
        "abc",
        "message digest",
        "abcdefghijklmnopqrstuvwxyz",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
        "12345678901234567890123456789012345678901234567890123456789012345678901234567890"
    ],
    'Adler32' : [
        0x00000001,
        0x00620062,
        0x024d0127,
        0x29750586,
        0x90860b20,
        0x8adb150c,
        0x97b61069
    ]
}
