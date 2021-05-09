
/* 
    Draft comment: 
    I forgot the semantics of http headers. 
    I also don't know if I'm missing '\r' in the output xml.
 */

// responsible for dealing with the XML document
public class XMLParser 
{
    private String XML = "";
    
    // Creates a http header
    public String HTTP_PUT_header (int content_length)
    {
        String header = "";
        header += "PUT /atom.xml HTTP/1.1\n";
        header += "User-Agent: ATOMClient/1/0\n";
        header += "Content-Type: application/xml\n";
        header += "Content-Length: " + content_length + "\n\n\r";
        return header;
    }

    // constructor - takes in the input file from ContentServer
    public XMLParser (String input)
    {
        string_to_XML (input);
    }
    
    // returns the XML webpage in the form of a String
    public String get_XML ()
    {
        return this.XML;
    }
    
    // parses the input file (from ContentServer) into a valid XML
    private void string_to_XML (String input)
    {
        // initialising variables
        String[] tags = {"title", "subtitle", "link", "updated", "author", "id", "entry", "title", "link", "id", "updated","summary"};
        boolean new_line = true;
        int i = 0,
        j = 0,
        k = 0,
        tabs = 1;
        
        // adding the first 2 lines of the XML document to the file
        this.XML += "<?xml version='1.0' encoding='iso-8859-1' ?>\n<feed xml:lang='en-US' xmlns='http://www.w3.org/2005/Atom'>\n";        
        
        // looping through the input file, converting the file into a valid XML document
        for (i =0; i < input.length(); ++i)
        {
            // if new line + cariage return => new tag
            if (input.charAt(i) == '\r')
            {
                // the entry tag contains multiple subtags, and is treated differently
                if (j == 6)
                {
                    this.XML += "\t<entry>\n";
                    new_line = true;
                    ++j;
                    k=0;
                    continue;
                }
                // normal tags are treated similarly to each other
                else
                {
                    // the author tag has a sub-tag of name
                    if (j == 4)
                    {
                        this.XML += "</name>\n\t"; 
                    }

                    this.XML += "</" + tags[j] + ">\n"; 
                    new_line = true;
                    ++j;
                    // at the end of each entry, put a closing tag
                    // notably this isn't executed on the last iteration
                    if (j == 12)
                    {
                        this.XML += "\t</entry>\n"; 
                        j = 6;
                    }
                    k=0;
                    continue;
                }
            }
            
            // assuming some form of \n\r shenanigans
            if (new_line && input.charAt(i) == '\n')
            {
                continue;
            }
            
            // if comparing tags
            if (new_line)
            {
                // comparing tags
                if ((k < tags[j].length()) && (input.charAt(i) == tags[j].charAt(k)))
                {
                    k++;
                    continue;
                }
                // skip if tag==entry
                if (j == 6)
                {
                    continue;
                }
                // end of tag parsing
                else if ((k == tags[j].length()) && (input.charAt(i) == ':'))
                {
                    // the rest of the data is appendend after the tag
                    new_line = false;
                    // for cute indenting
                    if (j> 6)
                        this.XML += "\t";
                    
                    // adding the tag to the start of the line
                    this.XML += "\t<" + tags[j] + ">";
                    
                    // if tag == author, make a sub-tag "name"
                    if (j == 4)
                    {
                        this.XML += "\n\t\t<name>";
                    }
                    
                    continue;
                }
                
                // bad input
                else
                {
                    System.out.println ("bad XML at the " + i + "'th index, input character: " + input.charAt(i));
                    System.exit(1);
                }
            }

            // if the input isn't a tag, just add it to the line
            else
            {
                this.XML += input.charAt(i);
            }
        }

        // to deal with the last tag
        if (j == 11)
        {
            this.XML += "</summary>\n\t<entry>\n";
        }

        // to end the feed
        this.XML += "</feed>";
        
        return ;
    }


    // checks if the input is a valid XML document
    boolean is_valid_XML (String input)
    {
        return true;
    }
}