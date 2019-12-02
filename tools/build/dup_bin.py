#! /usr/bin/env python3
"""
Duplicates input binary file until specified size in KB
"""

import sys
import os
import math
from pathlib import Path


def kb(bytes):
    return bytes * 1024


def main():
    binfile = sys.argv[1]
    dst_size = kb(int(sys.argv[2]))
    src_size = os.stat(binfile).st_size

    if dst_size % src_size != 0:
        print("Target size '{}' is not a multiple of soure file size '{}'".format(
            dst_size, src_size))
        return

    dup_count = int(dst_size / src_size)

    print('Duplicated "{}" {} times '.format(binfile, dup_count))

    fs = open(binfile, 'rb')
    src_bytes = fs.read()
    fs.close()

    fd = open(binfile, 'wb')
    fd.write(bytearray(list(src_bytes) * dup_count))


if __name__ == "__main__":
    main()
