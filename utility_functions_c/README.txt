Author: Jonathan Lindquist
ID: 3526760

Run program:

To run lab 3, just type in "make lab3", this will compile and execute the neccessary files, and will give you the command line for the program. To exit the program just type "end".

ls -l:

This function is straight forward, it will list the files/directories in the directory that the program is in. It will not list the directories "." or "..".

find:

This function will recursively search through directories and output every path that is associated with the given file name. The input for starting directory must start with "/home" to work. I tried to grab to full path of a directory using readpath(), but i couldnt get that to work. Instead I used chdir() to enter the provided starting directory and recursively searched from there. 

Thus the starting directory (i.e. the third argument of the 'find' command) must be a full directory path. So if the provided directory is /home/lindq181/Documents it will recursively search from the Documents directory. If just /home is provided it will start from there. But if just /Documents is provided the function will not work. 

grep:

This function will search a provided article snippet from ESPN that is provided as trial.txt. So the command line must be 'grep (anything you want) trial.txt'. I would have made this work with the find function, but it wasn't specified in the lab outline so I just used a local file to 'grep'. It will output the line # and then the targeted line of 100 characters if that line contains the target phrase.
