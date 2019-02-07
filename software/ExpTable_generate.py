#!/usr/bin/python3
import math

## Number of bits to use for the values:
bits = 8
## Datatype for the values:
datatype = "byte"
## Maximum line length:
maxLineLength = 70


maxValue = 2**bits - 1
valueLength = len(str(maxValue))

def f(x):
    global maxValue
    a = 0.49
    b = maxValue / math.log(maxValue / a)
    return a * math.exp(x / b)

print("#ifndef ExpTable_hh")
print("#define ExpTable_hh")
print("#include <avr/pgmspace.h>")
print("")
print("/**")
print(" * A pre-computed table with values of an exponential function.")
print(" * Generated with ExpTable_generate.py using")
print(" *   bits = {}".format(bits))
print(" *   datatype = {}".format(datatype))
print(" *   maxLineLength = {}".format(maxLineLength))
print("*/")
print("const {} expTable[{}] PROGMEM = {{".format(datatype, maxValue + 1))

line = "  "
for x in range(maxValue + 1):
    value = min(round(f(x)), maxValue)
    if (len(line) + valueLength + 2) > maxLineLength:
        print(line)
        line = "  "
    line += "{:{width}}".format(value, width=valueLength)
    if x != maxValue:
        line += ","
        if (len(line) + valueLength + 2) <= maxLineLength:
            line += " "
if line != "":
    print(line)

print("};")
print("")
print("#endif")
