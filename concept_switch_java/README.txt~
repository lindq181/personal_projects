Author: Jonathan Lindquist
ID: 3526760

compile with "javac switchRouter.java" and run with "java Router".

The code will open a socket on port 5001, gather the input from the Test Program, and parse this input into Tokens using the split function. It will then call 2 functions. The first, ifInTable(), will check if the source MAC is in the switch routing table and if it has the given interface. It will update the interface with the new one if it is different, otherwise it will add a new entry in the table and update the index. 

The second function destInTable() will check if the destination MAC address is in the routing table. It will set the message to the interface # if it is different, set it to "IGNORE" if the interface is the same, or set it to "BROADCAST" if the entry does not exist. 

It will return a string stored in the variable 'result'. This is then sent to the test program.
