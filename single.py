#!/usr/bin/env python

import argparse
import os, sys
import re
import datetime as dt

# python 3 compatibility
try:
	import cStringIO as sstream
except ImportError:
	from io import StringIO

description = "Converts phd to a single file for convenience."

# command line parser
parser = argparse.ArgumentParser(
    usage='%(prog)s [options...]', description=description)
parser.add_argument(
    '--output',
    '-o',
    nargs='+',
    help=
    'name and location of where to place file (and forward declaration file)',
    metavar='file',
    default='phd.hpp')
parser.add_argument('--quiet', help='suppress all output', action='store_true')
args = parser.parse_args()

single_file = ''
forward_single_file = ''
single_file = args.output[0]
if len(args.output) > 1:
	forward_single_file = args.output[1]
else:
	a, b = os.path.splitext(single_file)
	forward_single_file = a + '_forward' + b
script_path = os.path.normpath(os.path.dirname(os.path.realpath(__file__)))
working_dir = os.getcwd()
os.chdir(script_path)

intro = """// phd
// This file was generated with a script.
// Generated {time} UTC
// This header was generated with phd {version} (revision {revision})
// https://github.com/ThePhD/phd

#pragma once

#ifndef {guard}
#define {guard}

"""

module_path = os.path.join(script_path)

includes = set([])
standard_include = re.compile(r'#include <(.*?)>')
local_include = re.compile(r'#(\s*?)include "(.*?)"')
library_include = re.compile(r'#(\s*?)include <(phd/.*?)>')
pragma_once_cpp = re.compile(r'(\s*)#(\s*)pragma(\s+)once')
ifndef_cpp = re.compile(r'#ifndef PHD_.*?_HPP')
define_cpp = re.compile(r'#define PHD_.*?_HPP')
endif_cpp = re.compile(r'#endif // PHD_.*?_HPP')


def get_include(line, base_path, root_path):
	local_match = local_include.match(line)
	if local_match:
		# local include found
		full_path = os.path.normpath(
		    os.path.join(base_path, local_match.group(2))).replace(
		        '\\', '/')
		return full_path
	library_match = library_include.match(line)
	if library_match:
		# library include found
		full_path = os.path.normpath(
		    os.path.join(root_path, 'include',
		                 library_match.group(2))).replace('\\', '/')
		return full_path

	return None


def is_include_guard(line):
	return ifndef_cpp.match(line) or define_cpp.match(
	    line) or endif_cpp.match(line) or pragma_once_cpp.match(line)


def get_revision():
	return os.popen('git rev-parse --short HEAD').read().strip()


def get_version():
	return os.popen('git describe --tags --abbrev=0').read().strip()


def process_file(filename, out):
	global includes
	filename = os.path.normpath(filename)
	relativefilename = filename.replace(script_path + os.sep, "").replace(
	    "\\", "/")

	if filename in includes:
		return

	includes.add(filename)

	if not args.quiet:
		print('processing {}'.format(filename))

	out.write('// beginning of {}\n\n'.format(relativefilename))
	empty_line_state = True

	with open(filename, 'r', encoding='utf-8') as f:
		for line in f:
			# skip comments
			if line.startswith('//'):
				continue

			# skip include guard non-sense
			if is_include_guard(line):
				continue

			# get relative directory
			base_path = os.path.dirname(filename)

			# check if it's a standard file
			std = standard_include.search(line)
			if std:
				std_file = os.path.join('std', std.group(0))
				if std_file in includes:
					continue
				includes.add(std_file)

			# see if it's an include file
			name = get_include(line, base_path, script_path)

			if name:
				process_file(name, out)
				continue

			empty_line = len(line.strip()) == 0

			if empty_line and empty_line_state:
				continue

			empty_line_state = empty_line

			# line is fine
			out.write(line)

	out.write('// end of {}\n\n'.format(relativefilename))


version = get_version()
revision = get_revision()
include_guard = 'PHD_SINGLE_INCLUDE_HPP'

processed_files = [
    os.path.join(script_path, x) for x in ['include/phd/phd.hpp']
]
result = ''

if not args.quiet:
	print('Current version: {version} (revision {revision})\n'.format(
	    version=version, revision=revision))
	print('Creating single header for phd')

ss = StringIO()
ss.write(
    intro.format(
        time=dt.datetime.utcnow(),
        revision=revision,
        version=version,
        guard=include_guard))
for processed_file in processed_files:
	process_file(processed_file, ss)

ss.write('#endif // {}\n'.format(include_guard))
result = ss.getvalue()
ss.close()

if not args.quiet:
	print('finished creating single header for phd\n')

with open(single_file, 'w', encoding='utf-8') as f:
	if not args.quiet:
		print('writing {}...'.format(single_file))
	f.write(result)
