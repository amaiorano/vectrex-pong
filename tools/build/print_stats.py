#! /usr/bin/env python3
"""
Prints Vectrex rom stats
"""

import sys
import re


def main():
    map_file = sys.argv[1]
    crt0_file = sys.argv[2]

    data_size = None
    bss_size = None
    text_size = None

    f = open(map_file)
    for line in f.readlines():
        m = re.match('[ ]*(.*)  l_.data.*', line)
        if m:
            data_size = m.groups()[0]
            continue

        m = re.match('[ ]*(.*)  l_.bss.*', line)
        if m:
            bss_size = m.groups()[0]
            continue

        m = re.match('[ ]*(.*)  l_.text.*', line)
        if m:
            text_size = m.groups()[0]
            continue

        if data_size and bss_size and text_size:
            break

    ram_max_size = None
    f = open(crt0_file)
    for line in f.readlines():
        m = re.match('.*.bank ram.*SIZE=(.*)?,.*', line)
        if m:
            ram_max_size = m.groups()[0]
            break

    # Convert hex to decimal
    data_size = int(data_size, 16)
    bss_size = int(bss_size, 16)
    text_size = int(text_size, 16)
    ram_max_size = int(ram_max_size, 16)

    ram_usage_size = data_size + bss_size
    ram_usage_pct = int(ram_usage_size / ram_max_size * 100)

    print('=== Stats ===')
    print('text: {} bytes'.format(text_size))
    print('data: {} bytes'.format(data_size))
    print('bss : {} bytes'.format(bss_size))
    print('max : {} bytes'.format(ram_max_size))
    print('ram : data + bss = {} / {} = {}%'.format(ram_usage_size,
                                                    ram_max_size, ram_usage_pct))


if __name__ == "__main__":
    main()
