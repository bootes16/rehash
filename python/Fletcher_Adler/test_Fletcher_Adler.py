import pytest
from random import randint
from Fletch import Fletch
import Fletcher_simple
from test_vectors import *

@pytest.fixture
def randBytes():
    return bytes([ randint(0,255) for _ in range(16) ])

@pytest.mark.parametrize("input, sum", list(zip(TestVectorSet1['input'], TestVectorSet1['Fletcher16'])))
def test_Fletcher16(input, sum):
    assert Fletch.Fletch16().add(input).crc() == sum

def test_Fletcher16_random(randBytes):
    assert Fletch.Fletch16().add(randBytes).crc() == Fletcher_simple.Fletcher16(randBytes)


@pytest.mark.parametrize("input, sum", list(zip(TestVectorSet1['input'], TestVectorSet1['Fletcher32'])))
def test_Fletcher32(input, sum):
    assert Fletch.Fletch32().add(input).crc() == sum

@pytest.mark.parametrize("input, sum", list(zip(TestVectorSet1['input'], TestVectorSet1['Fletcher64'])))
def test_Fletcher64(input, sum):
    assert Fletch.Fletch64().add(input).crc() == sum

@pytest.mark.parametrize("input, sum", list(zip(TestVectorSet2['input'], TestVectorSet2['Adler32'])))
def test_Adler32(input, sum):
    assert Fletch.Adler32().add(input).crc() == sum
