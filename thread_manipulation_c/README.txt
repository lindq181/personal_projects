Jonathan Lindquist
ID: 3526760


compile and run:
There are 3 necessary files. fifoClient.c, fifoServer.c, and fifoHeader.h

In a terminal, run "gcc -o server fifoServer.c -lpthread", and then "./server"

In a seperate terminal, run "gcc -o client fifoClient.c", and then "./client"



Code Design:

I used the code from the Robbins text on pages 197 and 198 as a skeleton for my design. I also included a few functions like "r_write", "r_close", and "readline" from the restart library in the appendix of the book. 

in server:

The server opens the well-known fifo, then creates 3 threads at the function space threadListener. The design for threadListener follows what was included on the handout. It will lock, read from the wellKnownFifo, and then unlock. It will then declare/define variables the work with, open the client specific fifo (from the included clientID as a string) and respond with a name that is associated with the given phone number. 

The server prints out which thread it is currently in, and as seen in the script, all 3 do work if there are enough clients executing. The server never exits, it must be canceled. 

in client:

The client opens the wellKnownFifo. It then sends a randomly chosen phone number (from 3) along with its client ID. It will close the well-Known, open its client specific FIFO as a RDONLY, and print out the name that the server associates with the number. The client will exit after one run.

