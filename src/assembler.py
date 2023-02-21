#!/bin/env python3
"""
author: Nathan Vrieland 
This file needs at least python 3.10
"""
import sys
import os
""" 
all ints are represented as hex strings 
ADD = +
DIV = /
HALT = h
Identifier = $
IN = i
MUL = *
OUT = o
STO = s
SUB = -
DUMP = d
"""

outlist = []
idlist = {}
num_identifiers = 0
line_num = 1

def main():
    global line_num
    code = []
    if len(sys.argv) > 1:
        filepath = sys.argv[1]
    else:
        filepath = input("please enter filename: ")

    with open(filepath, "r") as infile:
        filelist = infile.read().split("\n")
        for x in filelist:
            code.append(x.split(' '))

    for x in code:
        match x[0]:
            case ';':
                line_num += 1
                continue
            case "ADD":
                ADD(x)
            case "SUB":
                SUB(x)
            case "DIV":
                DIV(x)
            case "MUL":
                MUL(x)
            case "HALT":
                HALT(x)
            case "IN":
                IN(x)
            case "OUT":
                OUT(x)
            case "STO":
                STO(x)
            case "DUMP":
                DUMP(x)
        line_num += 1
        outlist.append("\n")

    if len(sys.argv) >= 4 and sys.argv[2] == "-o":
        outpath = sys.argv[3]
    else:
        outpath = 'smallvm.out'
    with open(outpath, 'w') as outfile:
        outstring = "".join(outlist)
        stringlen = len(outstring) + 9 # 8 dedicated digits to store length of program and length of stack memory and an extra newline
        # format() converts ints into hex strings like 0x0000, [2:] removes the 0x 
        outstring = f"{format(stringlen, '#06x')[2:]}{format(num_identifiers, '#06x')[2:]}\n" + outstring
        outfile.write(outstring)

def ADD(line: list):
    global outlist, idlist
    if len(line) != 4:
        throwSyntaxError()
    outlist.append('+')
    outlist.append(mathExpression(line))


def SUB(line: list):
    global outlist, idlist
    if len(line) != 4:
        throwSyntaxError()
    outlist.append('-')
    outlist.append(mathExpression(line))

def DIV(line: list):
    global outlist, idlist
    if len(line) != 4:
        throwSyntaxError()
    outlist.append('/')
    outlist.append(mathExpression(line))

def MUL(line: list):
    global outlist, idlist
    if len(line) != 4:
        throwSyntaxError()
    outlist.append('*')
    outlist.append(mathExpression(line))

def HALT(line: list):
    global outlist, idlist
    outlist.append('h')

def IN(line: list):
    global outlist, idlist
    if len(line) != 2:
        throwSyntaxError()
    outlist.append('i')
    outlist.append(getmem(line[1], True))

def OUT(line: list):
    global outlist, idlist
    outlist.append('o')
    if line[1][0] == '"':
        # Use a list comprehension to put together string literal
        # this is what python is really really good at
        outlist.append("".join([x + ' ' for x in line[1:]])[:-1]) 
    elif line[1][0].isalpha():
        outlist.append(getmem(line[1], True))
    else:
        throwSyntaxError()
            

def STO(line: list):
    global outlist, idlist
    if len(line) != 3:
        throwSyntaxError()
    outlist.append('s')
    outlist.append(f"{getmem(line[1], True)} {getmem(line[2], False)}")

def DUMP(line: list):
    global outlist
    outlist.append("d")


def getmem(token: str, expect_identifier: bool):
    global idlist
    if token[0].isalpha():
        if token in idlist:
            return f"${hex(idlist[token])[2:]}"
        else:
            return f"${hex(addmem(token))[2:]}"
    elif expect_identifier: # throws error if identifier expected, but not found
        throwSyntaxError()
    elif token[0].isdigit():
        return hex(int(token))[2:]
    

def addmem(token: str):
    global idlist, num_identifiers
    idlist.update({token: num_identifiers})
    num_identifiers += 1
    return num_identifiers - 1
        
def throwSyntaxError():
    global line_num
    assert False, f"synatax error in line {line_num}"

def mathExpression(line: list):
    return f"{getmem(line[1], True)} {getmem(line[2], False)} {getmem(line[3], False)}"

if __name__ == "__main__":
    main()