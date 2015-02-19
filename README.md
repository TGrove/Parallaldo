# Parallaldo
Searches through binary files for patterns and pattern rotations.

Written for a Parallel Computing class I took.
The assignment was to search through binary files filled with lines of 0s and 1s for a specified grid pattern and or the rotation of the provided pattern using X number of threads. 
The Program first reads all target files into a queue and then creates a pre-rotated copy of parallado.
For each file the program attempts to assign X Pthreads an equal section of the target file to scan through for parallados.

*Takes command line arguments number of threads to use and for a folders containing the target files and pattern files containing "parallados" to look for (Use the debugger properties to adjust).
