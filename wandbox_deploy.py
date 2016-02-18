#! /usr/bin/env python

# This script uploads a directory to Wandbox (http://melpon.org/wandbox),
# which is an online compiler environment, and prints a permalink to the
# uploaded code. We use this to provide a "Try it online" version of the
# library to make the barrier to entry as low as possible.
#
# This script was adapted from the script proposed in
# https://github.com/melpon/wandbox/issues/153.
#
# To know how to use this script: ./wandbox.py --help
#
# Copyright Louis Dionne 2015
# Modified Work Copyright Barrett Adair 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

import argparse
import fnmatch
import json
import os
import re
import urllib2

main = 'example\\intro.cpp'
directory = 'include\\'

# Strips C and C++ comments from the given string.
#
# Copied from http://stackoverflow.com/a/241506/627587.
def strip_comments(text):
    def replacer(match):
        s = match.group(0)
        if s.startswith('/'):
            return " " # note: a space and not an empty string
        else:
            return s
    pattern = re.compile(
        r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
        re.DOTALL | re.MULTILINE
    )
    return re.sub(pattern, replacer, text)


# Post the given JSON data to Wandbox's API, and return the result
# as a JSON object.
def upload(options):
    request = urllib2.Request('http://melpon.org/wandbox/api/compile.json')
    request.add_header('Content-Type', 'application/json')
    response = urllib2.urlopen(request, json.dumps(options))
    return json.loads(response.read())


# Returns a list of the '.h' headers in the given directory and in
# subdirectories.
#
# The path must be absolute, and the returned paths are all absolute too.
def headers(path):
    return [
        os.path.join(dir, file)
            for (dir, _, files) in os.walk(path)
                for file in fnmatch.filter(files, "*.hpp")
    ]

cpp = os.path.abspath(main)
	
print upload({
    'code': open(cpp).read(),
    'codes': [{
        'file': os.path.relpath(header, directory).replace('\\','/'),
        'code': strip_comments(open(header).read())
    } for header in headers(directory)],
    'options': 'warning,c++14',
    'compiler': 'clang-3.6',
    'save': True,
    'compiler-option-raw': '-I.'
})

raw_input('Press <ENTER> to continue')