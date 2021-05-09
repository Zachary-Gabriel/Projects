
// main driver for the game
public class Chess 
{
    public static void main (String[] args) 
    {
        // just testin
        Board board = new Board();
        board.initial_position ();

        GUI_Board gui = new GUI_Board(board);
    }
    
}
