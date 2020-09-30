#! /usr/bin/python

import os, sys, glob, time, subprocess, signal
import popen2
import time

dict_train= {"testA.txt":"trainA.txt", "testB.txt":"trainB.txt", "testC.txt":"trainC.txt", "testD.txt":"trainD.txt", "testE.txt":"trainE.txt", "testF.txt":"trainF.txt", "testG.txt":"trainG.txt", "testH.txt":"trainH.txt", "testI.txt":"trainI.txt"}

weight = {"testA.txt":5.0, "testB.txt":10.0, "testC.txt":5.0, "testD.txt":5.0, "testE.txt":5.0, "testF.txt":5.0, "testG.txt":5.0, "testH.txt":5.0, "testI.txt":5.0}


list_tests= ["testA.txt", "testB.txt", "testC.txt", "testD.txt", "testE.txt", "testF.txt", "testG.txt", "testH.txt", "testI.txt"];

dict_results= {"testA.txt":"resultA.txt", "testB.txt":"resultB.txt", "testC.txt":"resultC.txt", "testD.txt":"resultD.txt", "testE.txt":"resultE.txt", "testF.txt":"resultF.txt", "testG.txt":"resultG.txt", "testH.txt":"resultH.txt", "testI.txt":"resultI.txt"}

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

def run_command(command_string, input_string="", max_lines=0, timeout=120, verbose=False, echo=False, throw_exception=True, return_valgrind_output=False, user_program=False):
    #print timeout
#    print "executing:", command_string
    if echo:
        print "executing:", command_string
    obj = popen2.Popen4(command_string)
    output = ""
    valgrind_output = ""
#    print "1"
    obj.tochild.write(input_string)
    obj.tochild.close()
    valgrind_prefix = "==%d==" % obj.pid
    maxSleep = timeout
#    print "2"
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
#    print "3"
    line = obj.fromchild.readline()
    while (line):
        if verbose == 1:
            print line,
        if line.startswith(valgrind_prefix):
            valgrind_output += line
        output += line
        line = obj.fromchild.readline()
    exit_status = obj.wait()
 #   print "4"
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
                val1 = int(test_list[i].strip());
                val2 = int(line.strip());
                if ((val1 + 1 == val2) or (val1 - 1 == val2)):
                    i+=1;
                    continue;
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
    if(ref==test.lower()):
        print "%s and %s are in different case. Please print your output in correct case."%(ref, test)
    return False


def make_executable(dirname):
    if os.path.isfile('Makefile') or os.path.isfile('makefile'):
        run_command("make clean", verbose=False)
        run_command("make", verbose=True)
    else:
        print "No Makefile found in", dirname
        print "Please submit a Makefile to receive full grade."
        run_command("gcc -o %s *.c *.h"%(dirname), verbose=False)


def file_grade(dirname):
#    print "Grading", dirname
    score = 0;
    try:
        make_executable(dirname)
    except:
        print "An exception occured trying to execute %s"%(dirname)
	os.chdir(prevdir)
	print "Score is ", score
	print ""
	print ""
        return score


    
    if not os.path.isfile("ml"):
        print "Executable %s missing. Please check the compilation output."%("ml")
	os.chdir(prevdir)
	print "Score is ", score
	print ""
	print ""
        return score

    test_dir = test_cases_directory
    #print test_dir
    #print sorted(os.listdir(test_dir))

    for test in list_tests:        

        train_file = test_dir + dict_train[test];
        result_file = test_dir + dict_results[test];
        
        print "Grading test file %s."%(test)
        if not result_file:
            print "Found a test file %s. but no result file."%(test)
            continue

        if not train_file:
            print "Found a test file %s. but no train file."%(test)
            continue

        ret = "";
        try:
            command_string = "./ml " + train_file + " " + test_dir + test
            ret = run_command(command_string, verbose=False);
        except:
            print "An exception occurred while executing ./ml %s %s"%(train_file, test_dir+test)
            print ret;
            print "Score is ", score
            continue;
	#time.sleep(10)
        
        if compare_string_file(result_file, ret, show_difference=True):
            score = score + weight[test];
    
        else:
            print "The output is not correct for input file %s."%(test)

    print "Score is ", score
    return score

            
if __name__ == '__main__':
    basepath = "hw2"
    tarmode = False #by default check the directory
    total_score = 0;

    test_cases_directory = os.getcwd() + "/testcases/"
    if len(sys.argv)>1:
        if sys.argv[1].strip().endswith("tar"):
            tarmode=True

    if tarmode==False:
        if not os.path.isdir(basepath):
            print "hw2 is not present in this directory."
            sys.exit(1)
        else:
	    prevdir = os.getcwd()
#	    print "dir is ", prevdir
	    if os.path.exists("obj_temp"):
#            	print "Deleting the directory obj_temp."
            	run_command("rm -rf obj_temp", verbose=False)
            run_command("mkdir obj_temp", verbose=False)
            os.chdir("obj_temp")
            run_command("cp -r ../hw2 .")
            if os.path.isdir("hw2"):
	    	os.chdir("hw2")
   	        print "Grading the content of hw2."
            total_score = file_grade("hw2")
            #os.chdir(basepath)
            #global_grade(basepath)

    else:
        total_score = 0;
        prevdir = os.getcwd()
	print "dir is ", prevdir
        if not os.path.exists("hw2.tar"):
            print "Expecting hw2.tar in current directory. Current directory is %s"%(prevdir)
            print "Please make sure you created hw2.tar in the right directory"
            sys.exit(1)
        if os.path.exists("obj_temp"):
            print "Deleting the directory obj_temp."
            run_command("rm -rf obj_temp", verbose=False)
        run_command("mkdir obj_temp", verbose=False)
        os.chdir("obj_temp")
        run_command("tar -xvf ../hw2.tar")
        if os.path.isdir("hw2"):
            os.chdir("hw2")
            total_score = file_grade("hw2")
        else:
            print "There is not directory named hw2 in hw2.tar."
            print "Please check your tar file."
        os.chdir(prevdir)
    print "Your TOTAL SCORE = ", str(total_score) + "/50";
