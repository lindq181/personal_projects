/* Fall 2014 CSci4211: Introduction to Computer Networks
** This program serves as the Router.
** Written in Java.
** Creation: 11/05/2014	Modify: 11/05/2014 */

import java.io.*;
import java.net.*;
import java.util.*;
import java.lang.*;

class Router {
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

		String N = ""; // N is the number of total routers (it is a string).
		N = readFrom.readLine();

		sendOut.writeBytes("ACK" + "\n"); // An ACK must be sent.
		System.out.println("Received the total number of routers.");

		String costMatrix = ""; // costMatrix is the corresponding cost matrix (it is one dimensional, and it is a string).
		costMatrix = readFrom.readLine();
		sendOut.writeBytes("ACK" + "\n");
		System.out.println("Received the cost matrix.");

		String ipRange = ""; // ipRange is the IP range for which each route servers. It is a string, please refer to its format in the project description.

		ipRange = readFrom.readLine();
		sendOut.writeBytes("ACK" + "\n");
		System.out.println("Received the IP range.");




		// Number of routers cast to int
		int numRouters = Integer.parseInt(N);

		// put distance costs into 2d matrix as ints
		int intCostMatrix[][] = new int[numRouters][numRouters];

		setCostMatrix(intCostMatrix, numRouters, costMatrix);

		//build 2d matrix for IP ranges as long ints
		long ipRangeMatrix[][] = new long[numRouters][2];

		setIpMatrix(ipRangeMatrix, numRouters, ipRange);

		//compute Djikstra matrix, updates intCostMatrix with new values, and sets updated nextHopMatrix
		String nextHopArray[] = new String[numRouters];

		computeDjikstra(intCostMatrix, numRouters, nextHopArray);

		String inputIpAddress = "";

		while (true) {

			inputIpAddress = readFrom.readLine();
			if (inputIpAddress.equals("END")) {
				System.out.println("Recieved END, exiting...");
				System.exit(1);
			}

			System.out.println("Received " + inputIpAddress + " Forwarding...");

			long ipCheck = ipToValue(inputIpAddress);
			int destinationRouter = 0;

			//find destination router
			for (int f=0; f<numRouters; f++) {
				if (ipCheck >= ipRangeMatrix[f][0] && ipCheck <= ipRangeMatrix[f][1]) {
					destinationRouter = f;
				}
			}

			//set return value as the next-hop router
			String returnRouter = "" + nextHopArray[destinationRouter].substring(0,1);

			sendOut.writeBytes(returnRouter + "\n");
			sendOut.flush();
		}
	}


	public static void setCostMatrix(int intCostMatrix[][], int numRouters, String costMatrix) {
		String delims  = "[, ]+";
		String[]matrixTokens = costMatrix.split(delims);
		int count = 0;
		
		for (int i=0; i<numRouters; i++) {
			for (int j=0; j<numRouters; j++) {
				intCostMatrix[i][j] = Integer.parseInt(matrixTokens[count]);
				count++;
			}
		}
	}

	public static void setIpMatrix(long ipRangeMatrix[][], int numRouters, String ipRange){
		String delims = "[ ]+";
		String[]ipTokens = ipRange.split(delims);
		int i;

		delims = "[\\-]";

		//This will turn the IP ranges into long integer values, stored in ipRangeMatrix
		for (i=0; i<ipTokens.length; i++) {

			String real = ipTokens[i].substring(1, ipTokens[i].length()-1);
			String[]smallSplit = real.split(delims);

			String intDelims = "[\\.]";
			for (int m=0; m<2; m++) {
				String[]singleIpSplit = smallSplit[m].split(intDelims);
				String stringValue = "";
				for (int k=0; k<4; k++) {
					int stringLength = singleIpSplit[k].length();
					if (stringLength == 1) {
						stringValue += "0" + "0" + singleIpSplit[k];
					} else if (stringLength == 2) {
						stringValue += "0" + singleIpSplit[k];
					} else {
						stringValue += singleIpSplit[k];
					}
				}
				long intValue = Long.parseLong(stringValue);
				ipRangeMatrix[i][m] = intValue;
			}
		}			
	}

	public static void computeDjikstra(int theCostMatrix[][], int numRouters, String hopMatrix[]) {
		
		//set initial next hop array
		for (int k=0; k<numRouters; k++) {
			if (theCostMatrix[0][k] >=0) {	
				hopMatrix[k] = "" + k;
			} else {
				hopMatrix[k] = "";
			}
		}

		int minimumNumber = Integer.MAX_VALUE;
		int minimumNode = 0;
		int nodesVisited[] = new int[numRouters];

		//if a node is visited, set that index to -1 in the visted array
		nodesVisited[0] = -1;
		for (int g=1; g<numRouters; g++) {
			nodesVisited[g]  = 0;
		}

		for (int j=1; j<numRouters; j++) {
			//get minimum node
			int i;
			for (i=1; i<numRouters; i++) {
				if (theCostMatrix[0][i] > 0 && theCostMatrix[0][i] < minimumNumber && nodesVisited[i] == 0) {
					minimumNumber = theCostMatrix[0][i];
					minimumNode = i;
				}
			}

			//compute values and update cost matrix and router 0's next hop matrix
			for (i=1; i<numRouters; i++) {
				if (theCostMatrix[minimumNode][i] > 0) {
					int distance = minimumNumber + theCostMatrix[minimumNode][i];
					if (theCostMatrix[0][i] < 0 || distance < theCostMatrix[0][i]) {
						theCostMatrix[0][i] = distance;
						hopMatrix[i] = hopMatrix[minimumNode] + i;
					}
				}
			}		

			//reset minimum number, and mark off node as visited
			minimumNumber = Integer.MAX_VALUE;
			nodesVisited[minimumNode] = -1;
		}
	}

	public static long ipToValue(String input) {

		long ipCheckValue;

		//turn the ip address into a value and return		
		String intDelims = "[\\.]";
				String[]singleIpSplit = input.split(intDelims);
				String stringValue = "";
				for (int k=0; k<4; k++) {
					int stringLength = singleIpSplit[k].length();
					if (stringLength == 1) {
						stringValue += "0" + "0" + singleIpSplit[k];
					} else if (stringLength == 2) {
						stringValue += "0" + singleIpSplit[k];
					} else {
						stringValue += singleIpSplit[k];
					}
				}
				ipCheckValue = Long.parseLong(stringValue);

		return ipCheckValue;
	}
}
