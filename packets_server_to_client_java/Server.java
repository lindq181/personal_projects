import java.io.*;
import java.net.*;
import java.util.*;

class Server {
	public static void main(String[] args) throws Exception {
		int port = 5001;
		ServerSocket welcomeSock = null;

		try {
			welcomeSock = new ServerSocket(port);
		} catch (Exception e) {
			System.out.println("Error: cannot open socket");
			System.exit(1); // Handle exceptions.
		}

		System.out.println("Server is listening on port 5001...");

           	Socket sSock = welcomeSock.accept();
		try{
            	BufferedReader inFromClient = new BufferedReader(new InputStreamReader(sSock.getInputStream()));
            	PrintWriter sendOut = new PrintWriter(sSock.getOutputStream(), true);

		int packetNumber = 0; //server window
		String total = "";
		String temp = "";
		String data;
            	
                  while(true){
			//grab incoming packet
                        data = inFromClient.readLine();

			//If last packet dont mangle, close socket
			if (Integer.parseInt(data.substring(44, 45)) == 1) {
				temp = data.substring(45, data.length());
				total += temp;
				System.out.println("Received last packet");
				sendOut.flush();
				sendOut.close();
				break;
			}

			String originalCheck = data.substring(0, 40);

			//pass it into replace function
                        String mangledPacket = replace(data, 2, 20, 20);
			if (!mangledPacket.equals("")) {
				String mangledCheck = mangledPacket.substring(0, 40);
				//check if the mangled packet was actually mangled
				if (originalCheck.equals(mangledCheck)) {
					
					//check if incoming packet is in correct sequence with server window 
					if (Integer.parseInt(data.substring(40, 41)) == packetNumber) {
						temp = data.substring(45, data.length());
						//copy to String total
						total += temp;
						System.out.println("Received packet " + packetNumber);
						packetNumber += 1;
						sendOut.println(packetNumber);
					} else {
						//wrong packet number, resend old ACK
					     	System.out.println("Wrong packetNumebr");
						sendOut.println(packetNumber);
					}
					sendOut.flush();
				
				} else {
					System.out.println("packet dropped, re-requesting packet " + packetNumber);
					sendOut.println(packetNumber);
				}
			} else {
			System.out.println("packet dropped, re-requesting packet " + packetNumber);
			sendOut.println(packetNumber);
			}
                  }

            } catch(Exception e){
                  // Do nothing here.
            }

	}

	public static String replace(String packet, int delay, int drop, int mangle){
		try{
			Thread thread = Thread.currentThread();
  			thread.sleep(delay * 1000);
		} catch (Exception e) {

		}
	
		String result = "";

		if (packet.length() < 512) return result;

		Random rand = new Random();
        	int prob = rand.nextInt(100);

		if (prob < drop + mangle){
			if (prob < drop) {
				return result;
			} else {
				for (int i = 0; i < 512; i++){
					int charInt = rand.nextInt(94) + 32;
					char[] character = Character.toChars(charInt);
					String temp = Character.toString(character[0]);
					result = result + temp;
				}
				return result;
			}
		} else {
			return packet;
		}
	}
}
