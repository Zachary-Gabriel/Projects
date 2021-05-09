import java.io.*;
import java.net.*; 

// the content server
public class ContentServer 
{
    public static void main(String[] args) 
    {
        // initialising file parsing variables
        FileReader file_in = null; // file pointer
        String input = ""; // the string elements of the file
        int ascii = -1; // int value of the input character

        // initialising xml parsing variables
        XMLParser XML; // used to convert the xml from 'input'
        String xml = ""; // the converted xml from 'input'

        // initialising socket programming variables
        Socket socket; // socket used to connect to aggregate server
        DataOutputStream output; // output stream - the stream that is used to send data to the server

        try 
        {
            // reading input file
            file_in = new FileReader(args[0]);
            
            // starting file read
            ascii = file_in.read();

            // if the input character is valid, append to string
            while (ascii != -1) 
            {
                input += (char) ascii;
                ascii = file_in.read();
            }
            // finished reading file
            file_in.close();

            // push the input file through the XML parser
            XML = new XMLParser (input);
            // making HTTP PUT header
            xml = XML.HTTP_PUT_header (XML.get_XML ().length ());
            // appending the parsed xml
            xml += XML.get_XML ();

            // sending the xml to aggregation server
            // establishing connection (1-way)
            socket = new Socket ("localhost", 8080);
            output  = new DataOutputStream (socket.getOutputStream ()); 
            System.out.println ("Connected");
            
            // clearing pipeline
            output.flush ();
            // sending data 
            output.writeUTF(xml);
            // terminating connection
            output.close ();

            // send the XML data to the aggregate server using a PUT request (for clarity)
            System.out.println (xml);
        }

        // if can't connect to server or read file
        catch (Exception e)
        {
            System.out.println (e);
        }
    }
}
