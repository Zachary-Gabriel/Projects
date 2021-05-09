// responsible for dealing with the XML document
public class XMLParser 
{
    private String XML = "";

    // Creates a http PUT header
    public static String HTTP_PUT_header (int content_length, int time)
    {
        String header = "";
        header += "PUT /atom.xml HTTP/1.1\n";
        header += "User-Agent: ATOMClient/1/0\n";
        header += "Content-Type: application/xml\n";
        header += "Content-Length: " + content_length + "\n";
        header += "lamport-clock: " + time + "\n\r";
        return header;
    }
    
    // constructor - takes in the input file from ContentServer
    public XMLParser (String input, boolean verbose)
    {
        string_to_XML (input, verbose);
    }
    
    // returns the XML webpage in the form of a String
    public String get_XML ()
    {
        return this.XML;
    }
    
    // if end of new tag
    private int[] new_tag (int i, int j, int k, String input, String[] tags, boolean verbose)
    {
        // the entry tag contains multiple subtags, and is treated differently
        if (j == 6)
        {
            if (input.length() > i+4)
            {
                if(input.charAt(i+4) != 'f')
                    this.XML += "\t<entry>\n";
            }
            ++j;
            k=0;
            int[] indices = {i,j,k};
            return indices;
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
            ++j;
            
            // at the end of each entry, put a closing tag
            // notably this isn't executed on the last iteration
            if (j == 12)
            {
                this.XML += "\t</entry>\n"; 
                j = 6;
            }
            
            k=0;
            int[] indices = {i,j,k};
            return indices;
        }
    }
    
    //
    private int[] tag_parser (int i, int j, int k, String input, String[] tags, boolean verbose)
    {
        // comparing tags
        if ((k < tags[j].length()) && (input.charAt(i) == tags[j].charAt(k)))
        {
            k++;
            int[] indices = {i,j,k,1};
            return indices;
        }
        // skip if tag==entry
        if (j == 6)
        {
            int[] indices = {i,j,k,0};
            return indices;
        }
        // end of tag parsing
        else if ((k == tags[j].length()) && (input.charAt(i) == ':'))
        {
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
            
            int[] indices = {i,j,k,0};
            return indices;
        }
        
        // bad input
        System.out.println ("bad XML at the " + i + "'th index, input character: " + input.charAt(i));
        System.exit(1);
        int[] indices = {i,j,k,-1};
        return indices;
    }
    
    // parses the input file (from ContentServer) into a valid XML
    private void string_to_XML (String input, boolean verbose)
    {
        // initialising variables
        String[] tags = {"title", "subtitle", "link", "updated", "author", "id", "entry", "title", "link", "id", "updated","summary"};
        boolean new_line = true;
        int i = 0,
        j = 0,
        k = 0;
        
        // adding the first 2 lines of the XML document to the file
        this.XML += "<?xml version='1.0' encoding='iso-8859-1' ?>\n<feed xml:lang='en-US' xmlns='http://www.w3.org/2005/Atom'>\n";        
        
        // looping through the input file, converting the file into a valid XML document
        for (i = 0; i < input.length(); ++i)
        {
            // if new line + cariage return => new tag
            if (input.charAt(i) == '\r')
            {
                int[] indices = new_tag (i, j, k, input, tags, verbose);
                i = indices[0];
                j = indices[1];
                k = indices[2];
                new_line = true;
                continue;
            }
            
            // assuming some form of \n\r shenanigans
            if (new_line && input.charAt(i) == '\n')
            {
                continue;
            }
            
            // if comparing tags
            if (new_line)
            {
                int[] indices = tag_parser (i, j, k, input, tags, verbose);
                i = indices[0];
                j = indices[1];
                k = indices[2];
                new_line = (indices[3] == 1) ? true : false;
                continue ;
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
        this.XML += "</feed>\n";
        
        return ;
    }
    
    private static int[] new_tag (int i, int j, String input, boolean is_end, String tag, String[] tags, boolean verbose)
    {
        // checking if tag is valid
        if (!tag.equals (tags[j]))
        {
            // for debugging
            if (verbose)
                System.out.println (tag + " not equal to " + tags[j] );
            
            int[] ret = {i, j, 1};
            return ret;
        }
        
        // if author or entry, where the end tag is delayed
        if (j == 4 || j == 8)
        {
            ++j;
        }
        
        if (j == 14)
        {
            if (input.charAt(i+4) == 'f')
            {
                //nothing
            }
            else
            {
                j = 7;
            }
        }
        
        if (is_end)
        {
            j++;
        }        
        
        int[] ret = {i, j, 0};
        return ret;
    }
    
    // checks if the input is a valid XML document
    public static boolean is_valid_XML (String input, boolean verbose)
    {
        // initialising variables
        String[] tags = {"title", "subtitle", "link", "updated", "author", "name", "author", "id", "entry", "title", "link", "id", "updated", "summary", "entry", "feed"};
        String first_lines = "<?xml version='1.0' encoding='iso-8859-1' ?>\n<feed xml:lang='en-US' xmlns='http://www.w3.org/2005/Atom'>\n";
        int j = 0;
        boolean is_tag = false;
        boolean is_end = false;
        String tag = "";
        
        for (int i = 0; i < input.length (); ++i)
        {
            // if first 2 lines, for syntax
            if (i < first_lines.length ())
            {
                if (input.charAt(i) != first_lines.charAt(i))
                {
                    return false;
                }
            }
            
            // for all lines after the first 2
            else
            {
                // if starting tag
                if (input.charAt(i) == '<')
                {
                    tag = "";
                    is_tag = true;
                    continue;
                }
                
                // if starting closing tag
                else if (input.charAt(i) == '/' && input.charAt(i-1) == '<')
                {
                    is_end = true;
                    continue;
                }
                
                // end of tag
                else if (input.charAt(i) == '>')
                {
                    int[] vals = new_tag (i, j, input, is_end, tag, tags, verbose);
                    
                    boolean bad = (vals[2] == 1) ? true : false;
                    if (bad) 
                        return false;

                    i = vals[0];
                    j = vals[1];
                    
                    is_tag = false;
                    is_end = false;
                }
                
                // if between < (/) and >
                else if (is_tag)
                {
                    tag += input.charAt(i);
                }
                
                // if not tag
                else
                {
                    continue;
                }
            }
        }
        
        if (tag.equals ("feed"))
            return true;
        
        return false;
    }
}
