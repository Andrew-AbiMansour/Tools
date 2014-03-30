#!/N/soft/rhel6/python/2.7.3/bin/python
# encoding: utf-8
 
import multiprocessing
import subprocess
 
def process_custom(*args, **cargs):
	"""Replace this with your own function
	that processes data, etc."""

	pass

def process(args):
	# Change this to the suitable binary to be called
	executable = ['grompp_mpi']

	# Command-line args go here as a *list*
	command_args = ['-h']

	# create process and pipe std in/out/err
	executable.extend(command_args)
	proc = subprocess.Popen(executable, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	
	# wait for the process to end / eof is reached
	result, err = proc.communicate()

	# include possible errors
	return result + err

def write_file(output):
	"""
	This function is called back once process terminates
	Its only purpose is writing output.
	"""

	fname = 'file{}.dat'.format(count)
        fp = open(fname,'w')

	output = '\n'.join(output)

        fp.write(output)
        fp.close()
	
	count += 1 # Very HACKISH method
  
if __name__ == '__main__':

	# Create a pool of worker processes using all available processors
	# you can specify the number of processors and an initializer function
	# by changing *processes* and *initializer*, respectively.

	pool = multiprocessing.Pool(processes=None, initializer=None)

	# Call *process* *numSims* times and then write output to *file{proc_num}.dat*
	numSims = 10
	count = 0

	for n in range(numSims):
		async_map = pool.map_async(process, (None,), callback=write_file)
		async_map.wait()

	pool.close()
