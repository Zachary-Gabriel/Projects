package Pieces;
import Game.*;
import java.util.*;

public class Rook extends Piece 
{    

    // constructor
    public Rook (int x, int y, Side side)
    {
        super (x, y, side, Piece_type.ROOK);
    }
    
    void update_first_move (boolean toggle)
    {
        this.first_move = toggle;
        return;
    }
    
    // implementing the move
    public void move (int x, int y)
    {
        Piece[][] brd = board.get_board();
        
        brd[this.x][this.y] = null;
        brd[x][y] = this;
        this.x = x;
        this.y = y;
        first_move = false;

        return;
    }

    // finds the moves for one of the four directions the rook can move
    private void one_direction (Vector<Vector<Integer>> moves, Piece[][] brd, boolean incr_x, boolean incr_y)
    {
        int i = this.x;
        int j = this.y;
        
        while (0 <= i && 0 <= j && i < 8 && j < 8) // bounds of the board
        {
            if (!(this.x == i && this.y == j))
            {
                if (brd[i][j] == null) // nothing in the tile
                {
                    push_to_moves (moves, i, j);
                }
                else if (brd[i][j].side != this.side) // taking an enemy piece
                {
                    push_to_moves (moves, i, j);
                    break;
                }
                else // cant push through the allied piece
                {
                    break;
                }
            }
            
            if (incr_y)
            j++;
            else
            j--;
        }

        i = this.x;
        j = this.y;
        while (0 <= i && 0 <= j && i < 8 && j < 8) // bounds of the board
        {
            if (!(this.x == i && this.y == j))
            {
                if (brd[i][j] == null) // nothing in the tile
                {
                    push_to_moves (moves, i, j);
                }
                else if (brd[i][j].side != this.side) // taking an enemy piece
                {
                    push_to_moves (moves, i, j);
                    break;
                }
                else // cant push through the allied piece
                {
                    break;
                }
            }
            
            if (incr_x)
            i++;
            else
            i--;
        }
    }

    // the set of all unique moves of the specific piece
    public Vector<Vector<Integer>> available_move ()
    {
        Vector<Vector<Integer>> moves = new Vector<Vector<Integer>>(); 
        Piece[][] brd = board.get_board();

        one_direction (moves, brd, true, true);
        one_direction (moves, brd, false, false);

        return moves;
    }

    // the set of all unique moves of the specific piece using an input board
    public Vector<Vector<Integer>> available_move (Piece[][] brd)
    {
        Vector<Vector<Integer>> moves = new Vector<Vector<Integer>>();
        one_direction (moves, brd, true, true);
        one_direction (moves, brd, false, false);
        
        return moves;
    }
}
