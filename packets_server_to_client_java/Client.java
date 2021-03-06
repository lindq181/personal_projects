import java.io.*;
import java.net.*;
import java.nio.file.*;
import java.security.MessageDigest;

class Client {
	public static void main(String[] args) throws Exception {
		String host = "localhost"; // Remote hostname. It can be changed to anything you desire.
		int port = 5001; // Port number.
		String sentence; // Store user input.
		String data; // Store the server's feedback.

		Socket cSock = null;
		DataOutputStream sendOut = null;
		BufferedReader readFrom = null;

		try{
			cSock = new Socket(host, port); // Initialize the socket.
			sendOut = new DataOutputStream(cSock.getOutputStream());// The output stream to server.
			readFrom = new BufferedReader(new InputStreamReader(cSock.getInputStream())); // The input stream from server.
		} catch (Exception e) {
			System.out.println("Error: cannot open socket");
			System.exit(1); // Handle exceptions.
		}

		//turn included file into String;
		String fileAsString = new String(Files.readAllBytes(Paths.get("trial.txt")));

		int index = 0;
		int packetNumber = 0;
		String sending;


		while(true){
			sending = makePacket(fileAsString, packetNumber, index);
			System.out.println("Sending packet # " + packetNumber);
			sendOut.writeBytes(sending + "\n");
			
			//if it was the last packet of string, exit
		

			cSock.setSoTimeout(5000); // Set the time out in milliseconds.
			try{
				//if last packet, close
				if (Integer.parseInt(sending.substring(44, 45)) == 1) {
						sendOut.flush();
						sendOut.close();
						break;
					}
				data = readFrom.readLine();
		
				//print ACK number
				System.out.println("ACK " + data);
				//check if its the correct ACK message
				if (Integer.parseInt("" + data.charAt(0)) == (packetNumber + 1)) {
					//interate through string if correct
					packetNumber += 1;
					index += sending.substring(45, sending.length()).length();
				} else {
					//handle if ACK is not correct by doing nothing and timing out
					System.out.println("Error with packet #! Retransmitting...");
				}
				
			} catch (SocketTimeoutException e) {
				System.out.println("Timeout! Retransmitting...");
			}
		}
	}

	public static String makePacket(String input, int sequence, int fileIndex) {

		String wordPacket = "";
		String packet = "";
		int stringIndex = fileIndex;
		int sizeOfPacket = 0;
		int isLastPacket = 0;
		int fileLength = input.length();
		int howMuchFileLeft = input.substring(stringIndex, fileLength).length();

		if (howMuchFileLeft >= 467) {
			for (int i = 0; i<467; i++) {
				wordPacket += String.valueOf(input.charAt(stringIndex));
				stringIndex += 1;
			}
			sizeOfPacket = 467;
		} else {
			for (int k = 0; k<howMuchFileLeft; k++) {
				wordPacket += String.valueOf(input.charAt(stringIndex));
				stringIndex += 1;
			}
			for (int j = howMuchFileLeft; j<467; j++) {
				wordPacket += "0";
			}
			sizeOfPacket = howMuchFileLeft;
			isLastPacket = 1;
		}
				
		packet = SHA1(wordPacket);
		packet += sequence;
		packet += sizeOfPacket;
		packet += isLastPacket;
		packet += wordPacket;

		return packet;
	}

	public static String SHA1(String inputText) {
		byte arr[] = null;

		try{
			MessageDigest m = MessageDigest.getInstance("SHA-1");  
			m.update(inputText.getBytes("UTF8"));  
			arr = m.digest();
		} catch (Exception e){
		}
		
		StringBuffer sb = new StringBuffer();  
		for (int i = 0; i < arr.length; ++i) {  
			sb.append(Integer.toHexString((arr[i] & 0xFF) | 0x100).substring(1,3));  
		}  
		return sb.toString();  
	}
}
