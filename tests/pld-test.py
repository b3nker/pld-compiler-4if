#!/usr/bin/env python3

import re
import argparse
import glob
import os
import shutil
import sys
import subprocess


def command(string, logfile=None):
    """execute `string` as a shell command, optionnaly logging stdout+stderr to a file. return exit status.)"""
    if args.verbose:
        print("pld-test.py: "+string)
    try:
        output = subprocess.check_output(
            string, stderr=subprocess.STDOUT, shell=True)
        ret = 0
    except subprocess.CalledProcessError as e:
        ret = e.returncode
        output = e.output

    if logfile:
        f = open(logfile, 'w')
        print(output.decode(sys.stdout.encoding) +
              '\n'+'return code: '+str(ret), file=f)
        f.close()

    return ret


def dumpfile(name):
    print(open(name).read(), end='')

######################################################################################
# ARGPARSE step: make sense of our command-line arguments


argparser = argparse.ArgumentParser(
    description="Compile each test-case  with both GCC and PLD, and then compare the results.",
    epilog=""
)

argparser.add_argument('input', metavar='PATH', nargs='+', help='For each argument:'
                       + ' if it points to a file, use this file as a test-case;'
                       + ' if it points to a directory, use all *.c files in subtree as test-cases.')

argparser.add_argument('-d', '--debug', action="count", default=0,
                       help='Increase quantity of debugging messages (only useful to debug the test script itself)')
argparser.add_argument('-v', '--verbose', action="count", default=0,
                       help='Increase verbosity level. You can use this option multiple times.')
argparser.add_argument('-w', '--wrapper', metavar='PATH',
                       help='Invoke the PLD compiler through the shell script at PATH. (default: use `pld-wrapper.sh` from the same directory as pld-test.py)')
argparser.add_argument('-c', '--compact', action="count", default=0,
                       help='Do not display error details in test summary.')


args = argparser.parse_args()

if args.debug >= 2:
    print('debug: command-line arguments '+str(args))

orig_cwd = os.getcwd()
if "pld-test-output" in orig_cwd:
    # we are invoked from within a preexisting DEST subtree => let's use it in place
    DEST = "./"
    while os.path.basename(os.path.realpath('.')) != 'pld-test-output':
        os.chdir('..')
        DEST = "../"+DEST
    os.chdir(orig_cwd)
    DEST = os.path.normpath(DEST)
    if args.debug:
        print('debug: DEST tree is at `'+DEST+"'")
else:
    # create a new DEST subtree if required
    if not os.path.isdir('pld-test-output'):
        os.mkdir('pld-test-output')
    DEST = 'pld-test-output'

# Then we process the inputs arguments i.e. filenames or subtrees
inputfilenames = {}
for path in args.input:
    path = os.path.normpath(path)  # collapse redundant slashes etc.
    for dirpath, dirnames, filenames in os.walk(path):
        # if os.path.samefile(dirpath,args.outdir):
        #     print("error: INPUT directory:`"+path+"' contains OUTDIR `"+args.outdir+"'")
        #     sys.exit(1)
        if dirpath not in inputfilenames:
            inputfilenames[dirpath] = []
        inputfilenames[dirpath] += [name for name in filenames if name[-2:] == '.c']

inputfilenames = {dir: files for dir, files in inputfilenames.items() if files}

# debug: after treewalk
if args.debug:
    print("debug: list of files after tree walk:", " ".join(inputfilenames))

# sanity check
if len(inputfilenames) == 0:
    print("error: found no test-case in: "+" ".join(args.input))
    sys.exit(1)

# Here we check that we can actually read the files.  Our goal is to
# fail as early as possible when the CLI arguments are wrong.
for directory in inputfilenames:
    for inputfilename in inputfilenames[directory]:
        try:
            f = open(os.path.join(directory, inputfilename), "r")
            f.close()
        except Exception as e:
            print("error: "+e.args[1]+": "+inputfilename)
            sys.exit(1)

# Last but not least: we now locate the "wrapper script" that we will
# use to invoke the padawan compiler

if args.wrapper:
    wrapper = os.path.realpath(os.getcwd()+"/" + args.wrapper)
else:
    wrapper = os.path.dirname(os.path.realpath(__file__))+"/pld-wrapper.sh"

if not os.path.isfile(wrapper):
    print("error: cannot find "+os.path.basename(wrapper) +
          " in directory: "+os.path.dirname(wrapper))
    exit(1)

if args.debug:
    print("debug: wrapper path: "+wrapper)

######################################################################################
# PREPARE step: delete the old content of DEST, then copy all test-cases under DEST

# Deleting old content
for filename in os.listdir(DEST):
    file_path = os.path.join(DEST, filename)
    try:
        if os.path.isfile(file_path) or os.path.islink(file_path):
            os.unlink(file_path)
        elif os.path.isdir(file_path):
            shutil.rmtree(file_path)
    except Exception as e:
        print('Failed to delete %s. Reason: %s' % (file_path, e))

jobs = {dir: [] for dir in inputfilenames}

for directory in inputfilenames:
    for inputfilename in inputfilenames[directory]:
        if args.debug >= 2:
            print("debug: PREPARING " + directory + "/" + inputfilename)

        # each test-case gets copied and processed in its own subdirectory:
        # ../somedir/subdir/file.c becomes ./pld-test-output/somedir-subdir-file/input.c
        subdir = (DEST + '/' + directory + '/' +
                  inputfilename.lstrip('./')[:-2].replace('/', '-'))
        if not os.path.isdir(subdir):
            os.makedirs(subdir)
            shutil.copyfile(os.path.join(
                directory, inputfilename), subdir+'/input.c')
        jobs[directory].append(subdir)

# eliminate duplicate paths from the 'jobs' list
unique_jobs = {dir: list(set(jobs[dir])) for dir in jobs}
jobs = unique_jobs

# debug: after deduplication
if args.debug:
    print("debug: list of test-cases after deduplication:", " ".join(jobs))


######################################################################################
# TEST step: actually compile all test-cases with both compilers

n_test = 0

n_skipped = 0

n_fail_by_type = {
    'accepts_invalid': 0,
    'rejects_valid': 0,
    'incorrect_asm': 0,
    'different_results': 0,
}

messages = {
    'accepts_invalid': 'your compiler accepts an invalid program',
    'rejects_valid': 'your compiler rejects a valid program',
    'incorrect_asm': 'your compiler produces incorrect assembly',
    'different_results': 'different results at execution',
    'gcc_linker_fail': 'GCC failed to compile test file',
}

failed_tests = []


def get_terminal_width():
    return shutil.get_terminal_size((0, 0)).columns


def test_exec_print_directory(directory):
    print()
    print("\x1b[1m" + "--- " + directory + " ---" + "\x1b[m")


def test_exec_print_run(jobname):
    s = "    " + os.path.basename(jobname)
    w = max(1, 40 - len(s) - 4)
    print(s + ' '*w, end="")


def test_exec_print_ok(jobname):
    print("\x1b[32m" + "ok" + "\x1b[m")


def test_exec_print_fail_msg(jobname, errcode):
    print("\x1b[31m" + "fail" + "\x1b[m")
    print("\x1b[2m" + "    -> " + messages[errcode] + "\x1b[m")


def test_exec_print_skip_msg(jobname, errcode):
    print("\x1b[33m" + "skip" + "\x1b[m")
    print("\x1b[2m" + "    -> " + messages[errcode] + "\x1b[m")


def register_failed_test(directory, jobname, errcode):
    test_exec_print_fail_msg(jobname, errcode)
    n_fail_by_type[errcode] += 1
    failed_tests.append([directory, jobname, errcode])


def register_skipped_test(directory, jobname, errcode):
    test_exec_print_skip_msg(jobname, errcode)


def file_readable(file):
    return (os.path.isfile(file) and os.access(file, os.R_OK))


def natural_sorted(l):
    def convert(text):
        return int(text) if text.isdigit() else text.lower()

    def alphanum_key(key):
        return [convert(c) for c in re.split('([0-9]+)', key)]

    return sorted(l, key=alphanum_key)


for directory in jobs:
    test_exec_print_directory(directory)
    for jobname in natural_sorted(jobs[directory]):
        n_test += 1

        os.chdir(orig_cwd)
        os.chdir(jobname)

        test_exec_print_run(jobname)

        # JEDI compiler, aka GCC
        gccstatus = command("gcc -S -o asm-gcc.s input.c", "gcc-compile.txt")
        if gccstatus == 0:
            # test-case is a valid program. we should run it
            ldstatus = command("gcc -o exe-gcc asm-gcc.s", "gcc-link.txt")
            if ldstatus:
                # linker failed to produce exe-gcc
                register_skipped_test(directory, jobname, 'gcc_linker_fail')
                n_skipped += 1
                continue
            exegccstatus = command("./exe-gcc", "gcc-execute.txt")
            if args.verbose >= 2:
                dumpfile("gcc-execute.txt")

        # PADAWAN compiler
        pldstatus = command(wrapper+" asm-pld.s input.c", "pld-compile.txt")

        if gccstatus != 0 and pldstatus != 0:
            # padawan correctly rejects invalid program -> test-case ok
            test_exec_print_ok(jobname)
            continue
        elif gccstatus != 0 and pldstatus == 0:
            # padawan wrongly accepts invalid program -> error
            register_failed_test(directory, jobname, "accepts_invalid")
            continue
        elif gccstatus == 0 and pldstatus != 0:
            # padawan wrongly rejects valid program -> error
            register_failed_test(directory, jobname, "rejects_valid")
            if args.verbose:
                dumpfile("pld-compile.txt")
            continue
        else:
            # padawan accepts to compile valid program -> let's link it
            ldstatus = command("gcc -o exe-pld asm-pld.s", "pld-link.txt")
            if ldstatus:
                register_failed_test(directory, jobname, "incorrect_asm")
                if args.verbose:
                    dumpfile("pld-link.txt")
                continue

        # both compilers  did produce an  executable, so now we  run both
        # these executables and compare the results.

        exepldstatus = command("./exe-pld", "pld-execute.txt")
        if open("gcc-execute.txt").read() != open("pld-execute.txt").read():
            register_failed_test(directory, jobname, "different_results")
            if args.verbose:
                print("GCC:")
                dumpfile("gcc-execute.txt")
                print("you:")
                dumpfile("pld-execute.txt")
            continue

        test_exec_print_ok(jobname)

n_fail = len(failed_tests)
n_succ = n_test - n_fail - n_skipped
print()

terminal_width = get_terminal_width()

if n_fail == 0:
    print("\x1b[1;32m" +
          "[All tests successful]".center(terminal_width, '-') + "\x1b[m")
    print()
    exit(0)
elif n_succ == 0:
    print("\x1b[1;31m" +
          "[All tests failed]".center(terminal_width, '-') + "\x1b[m")
    print()
    exit(2)
else:
    print("\x1b[1;33m" +
          "[Some tests failed]".center(terminal_width, '-') + "\x1b[m")
    print()
    print("    ",
          n_succ, " × ", "\x1b[32m", "ok", "\x1b[m",
          ", ",
          n_fail, " × ", "\x1b[31m", "fail", "\x1b[m",
          ", ",
          n_skipped, " × ", "\x1b[33m", "skip", "\x1b[m",
          sep="")
    print()
    for dir, j, errcode in failed_tests:
        os.chdir(orig_cwd)
        os.chdir(j)
        print("\x1b[1;31m" + " × " + "\x1b[m",
              dir + "/\x1b[1m" + os.path.basename(j) + "\x1b[m")
        print("   ", "\x1b[2m" + messages[errcode] + "\x1b[m")

        if not args.compact:
            if file_readable("pld-execute.txt"):
                segfaulted = False
                for line in open("pld-execute.txt").readlines():
                    if line.startswith('Segmentation fault'):
                        segfaulted = True
                        break
                if segfaulted:
                    print("   ", "   ", "\x1b[31m! " +
                          'Segmentation fault' + "\x1b[m")

            if file_readable("pld-compile.txt"):
                ifcc_segfaulted = False
                for line in open("pld-compile.txt").readlines():
                    if line.startswith('Segmentation fault'):
                        ifcc_segfaulted = True
                        break
                if ifcc_segfaulted:
                    f = os.path.join('tests', dir, os.path.basename(j)) + '.c'
                    print("   ", "   ", "\x1b[31;1m! " +
                          'Segmentation fault in compiler' + "\x1b[m")
                    print("   ", "   ", "\x1b[31;2m# " +
                          'gdb -q --args ./compiler/ifcc ' + f + "\x1b[m")

            if errcode == 'rejects_valid' and file_readable("pld-compile.txt"):
                for line in open("pld-compile.txt").readlines()[:-2]:
                    print("   ", "\x1b[2m| " + line.rstrip() + "\x1b[m")

            if errcode == 'incorrect_asm' and file_readable("pld-link.txt"):
                for line in open("pld-link.txt").readlines()[:-2]:
                    print("   ", "   ", "\x1b[2m| " + line.rstrip() + "\x1b[m")

            if errcode == 'different_results':
                if file_readable("gcc-execute.txt") and file_readable("pld-execute.txt"):
                    gccstatus = open("gcc-execute.txt").readlines()[-1]
                    gccstatus = gccstatus.lstrip('return code:').strip()

                    pldstatus = open("pld-execute.txt").readlines()[-1]
                    pldstatus = pldstatus.lstrip('return code:').strip()

                    print("   ", "   ", "\x1b[2m| " +
                          "Got return code: " + pldstatus + "\x1b[m")
                    print("   ", "   ", "\x1b[2m| " +
                          "but expected:    " + gccstatus + "\x1b[m")

            if errcode == 'gcc_linker_fail' and file_readable("gcc-link.txt"):
                for line in open("gcc-link.txt").readlines()[:-2]:
                    print("   ", "   ", "\x1b[2m| " + line.rstrip() + "\x1b[m")

            print()

    print()
    exit(1)
