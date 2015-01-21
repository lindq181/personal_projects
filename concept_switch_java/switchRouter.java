//Author: Jonathan Lindquist 
//ID #: 3526760

import java.io.*;
import java.net.*;
import java.util.*;
import java.lang.*;

class Router {

	public static String[][] routingTable = new String[100][2];
	public static int index = 0;

	public static void main(String[] args) throws Exception {
		String host = "localhost";
		int port = 5001;

		Socket cSock = null;
		DataOutputStream sendOut = null;
		BufferedReader readFrom = null;

		/*-----------------  First Phase, Initialization ------------- */

		try{
			cSock = new Socket(host, port);
			sendOut = new DataOutputStream(cSock.getOutputStream());
			readFrom = new BufferedReader(new InputStreamReader(cSock.getInputStream()));
		} catch (Exception e) {
			System.out.println("Error: cannot open socket");
			System.exit(1); // Handle exceptions.
		}

		while(true) {
			try {
				String result;
				String input = readFrom.readLine();

				if (input.equals("END")) {
					System.out.println("Ending Session...");
					System.exit(1);
				}
			
				//gather input information via Tokenizing
				String delims = "[ ]";
				String[] inputBreakdown = input.split(delims);

				String interfaceInput = inputBreakdown[0];
				String srcMAC = inputBreakdown[1];
				String destMAC = inputBreakdown[2];

				//check table entries
				ifInTable(interfaceInput, srcMAC);
				//set result String to be returned to test program 
				result = destInTable(interfaceInput, destMAC);

				sendOut.writeBytes(result + "\n");
				sendOut.flush();

			} catch (Exception e) {
				System.out.println("Error: Problem handling input");
				System.exit(1); // Handle exceptions.
			}		
		}
	}

	public static void ifInTable(String inter, String src) {
		boolean check = false;

		for (int i=0; i<index; i++) {
			//update interface if it has changed
			if (routingTable[i][1].equals(src) && !(routingTable[i][0].equals(inter))) {
				routingTable[i][0] = inter;
				check = true;
			} else if (routingTable[i][1].equals(src)) {
				check = true;
			}
		}

		//if not in table, put it in
		if (!check) {
			routingTable[index][0] = inter;
			routingTable[index][1] = src;
			index++;
		}
	}

	public static String destInTable(String inter, String dest) {
		boolean found = false;
		String action = "";

		for (int i=0; i<index; i++) {
			if (routingTable[i][1].equals(dest) && !(routingTable[i][0].equals(inter))) {
				action = routingTable[i][0];
				found = true;
			} else if (routingTable[i][1].equals(dest)) {
				action = "IGNORE";
				found = true;
			}
		}

		if (!found) {
			action = "BROADCAST";
		}
	
		return action;
	}
}
