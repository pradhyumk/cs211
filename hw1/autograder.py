#!/usr/bin/python

import os, sys, glob, time, subprocess, signal
import popen2

hwName = 'hw1'
programs = ['sortArray', 'linkedList', 'hashTable', 'stringOps', 'bst']

# if a program has single liner input and output, we put all test cases in single file. Otherwise, we have a file for test and associated file with results
formats = {'sortArray':'file',
           'linkedList':'file',
           'hashTable':'file',
           'stringOps':'line',
           'bst':'file'}

weight = {'sortArray':1, 'linkedList':1, 'hashTable':1, 'stringOps':1, 'bst':1}

scores = {'sortArray':0, 'linkedList':0, 'hashTable':0, 'stringOps':0, 'bst':0}

test_cases_directory = ""

class ExperimentError(Exception):
    def __init__(self, command, output):
        self.command = command
        limit = 10000
        if(len(output) > limit):
          self.output = output[:limit/2] + "\n\n...TRUNCATED...\n\n" + output[-limit/2:]
        else:
          self.output = output
    def __str__(self):
        return "ExperimentError:" + `self.command`

def run_command(command_string, input_string="", max_lines=0, verbose=False, echo=False, throw_exception=True, return_valgrind_output=False, user_program=False):

    if echo:
        print "executing:", command_string
    obj = popen2.Popen4(command_string)
    output = ""
    valgrind_output = ""
    obj.tochild.write(input_string)
    obj.tochild.close()
    valgrind_prefix = "==%d==" % obj.pid
    maxSleep = 20
    if user_program: #program may have an infinite loop
        while maxSleep>0:
            time.sleep(0.25)
            maxSleep-=1
            if obj.poll()!=-1:
                break
        if maxSleep==0 and obj.poll()==-1:
            os.kill(obj.pid, signal.SIGKILL)
            print command_string, " taking longer than expected. Killed."
            return ""
    line = obj.fromchild.readline()
    while (line):
        if verbose == 1:
            print line,
        if line.startswith(valgrind_prefix):
            valgrind_output += line
        output += line
        line = obj.fromchild.readline()
    exit_status = obj.wait()
    if(max_lines != 0):
        lines = output.split("\n");
        output = string.join(lines[-max_lines:], "\n")

    if throw_exception and exit_status != 0:
        raise ExperimentError(command_string, output)

    if return_valgrind_output:
        return valgrind_output
    else:
        return output

def compare_string_file(ref_file, test_string, show_difference=False):
    test_list=test_string.split("\n")
    fd = open(ref_file)
    i=0
    flag=True

    for line in fd:
        if i<len(test_list):
            if line.strip()!=test_list[i].strip():
                print "Observed:", test_list[i].strip();
                print "Expected:", line.strip();
                flag=False
        elif len(line.strip())>0:
            print "Output missing: ", line
            flag=False
        i+=1
    
    fd.close()
    while(i<len(test_list)):
        if len(test_list[i].strip())==0:
            i+=1
            continue
        print "Extra output: ", test_list[i]
        i+=1
        flag=False

    return flag

def compare_string(ref, test):
    ref = ref.strip()
    test = test.strip()

    if(ref==test):
        return True
    if(ref.lower()==test.lower()):
        print "%s and %s are in different case. Please print your output in correct case."%(ref, test)
    return False

def make_executable(dirname):
    if os.path.isfile('Makefile') or os.path.isfile('makefile'):
        run_command("make clean", verbose=False)
        run_command("make", verbose=False)
    else:
        raise ExperimentError("No Makefile found in %s" % dirname, "")

def file_grade(dirname):
    print "Grading", dirname
    prevdir = os.getcwd()
    score = 0

    try:
        make_executable(dirname)
    except Exception as e:
        print "An exception occured trying to build %s"%(dirname)
        print(e)
	print "Score is ", score
        return
    
    if not os.path.isfile(dirname):
        print "Executable %s missing. Please check the compilation output."%(dirname)
	print "Score is ", score
        return

    test_dir = test_cases_directory + dirname  + "/"

    for testfile in sorted(os.listdir(test_dir)):
        if os.path.isdir(testfile) or not testfile.startswith("test"):
            continue
        resultfile = test_dir + "result"+testfile[4:len(testfile)]
        if not os.path.isfile(resultfile):
            print "Found a test file %s. but no associated result file."%(testfile)
            continue

        try:
            ret = run_command('./%s "%s"'%(dirname, test_dir + testfile), user_program=True, verbose=False);
        except Exception as e:
            print "An exception occurred while executing %s %s"%(dirname, testfile)
            print(e)
            scores[dirname] = score
            print "Score is ", score
            return

        if compare_string_file(resultfile, ret, show_difference=True):
	    score = score + weight[dirname]
        else:
            print "The output is not correct for input file %s."%(testfile)
    print "Score is ", score
    scores[dirname] = score
    
def single_grade(dirname):
    print "Grading", dirname
    prevdir = os.getcwd()
    score = 0

    try:
        make_executable(dirname)
    except:
        print "An exception occured trying to build %s"%(dirname)
	print "Score is ", score
        return

    if not os.path.isfile(dirname):
        print "Executable %s missing. Please check the compilation output."%(dirname)
        print "Score is ", score
        return

    test_file = test_cases_directory + dirname + "/test.txt"

    if not os.path.isfile(test_file):
        print "Expecting the test cases in test.txt. Not found."
	print "Score is ", score
	return
    else:
        print ""

    fd = open(test_file)
    state = 0
    for line in fd:
        if state==0:
            inputline = line
            state = 1
        else:
            outputline = line
            state = 0

            try:
	        command_str = './%s "%s"'%(dirname, inputline.strip())

                ret = run_command(command_str, user_program=True)
                if compare_string(outputline, ret):
    		    score = score + weight[dirname]
            except Exception as e:
                print(e)
                print "An exception occured trying to execute %s"%(command_str)
        	print "Score is ", score
    fd.close()
    print "Score is ", score
    scores[dirname] = score
    
def global_grade(dirname):
    for prog in programs:
        if prog in formats and formats[prog]=='line':
            single_grade(prog)
        elif prog in formats and formats[prog]=='file':
            file_grade(prog)
            
if __name__ == '__main__':
    tarmode = False #by default check the directory
    test_cases_directory = os.getcwd() + "/testcases/"
    if len(sys.argv)>1:
        if sys.argv[1].strip().endswith("tar"):
            tarmode=True

    if tarmode:
        prevdir = os.getcwd()
	print "dir is ", prevdir
        if not os.path.exists(hwName + ".tar"):
            print "Expecting %s.tar in current directory. Current directory is %s"%(hwName, prevdir)
            print "Please make sure you created %s.tar in the right directory" % hwName
            sys.exit(1)
        if os.path.exists("obj_temp"):
            print "Deleting the directory obj_temp."
            run_command("rm -rf obj_temp", verbose=False)
        run_command("mkdir obj_temp", verbose=False)
        os.chdir("obj_temp")
        run_command("tar -xvf ../%s.tar" % hwName)
        if os.path.isdir(hwName):
            os.chdir(hwName)
            global_grade(hwName)
        else:
            print "There is no directory named %s in %s.tar." % (hwName, hwName)
            print "Please check your tar file."
    else:
        if not os.path.isdir(hwName):
            print "%s is not present in this directory." % hwName
            sys.exit(1)
        else:
            print "Grading the content of %s." % hwName
            os.chdir(hwName)
            global_grade(hwName)

    total_score = 0.0
    print 'You scored '
    for key, value in scores.iteritems():
        total_score += value
        print str(value), ' in ', key

    print "Your total score = ", total_score, " / 10"
    print "Your assignment will be graded with additional unreleased test cases"
