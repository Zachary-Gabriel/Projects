
// represents the coordinate pairing. Will probably remove this as it complicates things
public class Coords 
{
    // represents the file and rank of the piece
    int x, y;

    // Constructor that sets the x and y coordinates
    public Coords (int x, int y)
    {
        this.x = x;
        this.y = y;
    }

    // updating both x and y coords
    public void set_all (int x, int y)
    {
        this.x = x;
        this.y = y;
    }

    // update only the x coord
    public void set_x (int x)
    {
        this.x = x;
    }

    // update only the y coord
    public void set_y (int y)
    {
        this.y = y;
    }
    
    // fetches the x coord
    public int get_x ()
    {
        return this.x;
    }

    // fetches the y coord
    public int get_y ()
    {
        return this.y;
    }
}
