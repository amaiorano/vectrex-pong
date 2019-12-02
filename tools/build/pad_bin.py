#! /usr/bin/env python3
"""
Reads a binary file, and writes a new one padded with 0s to the next multiple of 8KB'
"""

import sys
import os
import math
from pathlib import Path


def main():
    binfile = sys.argv[1]

    if len(sys.argv) == 3:
        binfile_out = sys.argv[2]
    else:
        p = Path(binfile)
        binfile_out = p.with_name(p.stem + "_padded" + p.suffix)

    src_size = os.stat(binfile).st_size
    min_size = 8192
    dst_size = math.floor((src_size / min_size) + min_size)
    pad_size = dst_size - src_size

    print('Writing "{}" with {} bytes + {} padding bytes = {} total bytes'.format(
        binfile_out, src_size, pad_size, dst_size))

    fs = open(binfile, 'rb')
    bytes = fs.read()
    fs.close()

    fd = open(binfile_out, 'wb')
    pad_bytes = bytearray([0] * pad_size)
    fd.write(bytes + pad_bytes)


if __name__ == "__main__":
    main()
