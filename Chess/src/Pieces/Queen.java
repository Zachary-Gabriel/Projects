package Pieces;
import Game.*;
import java.util.*;

public class Queen extends Piece 
{
    // constructor
    public Queen (int x, int y, Side side)
    {
        super (x, y, side, Piece_type.QUEEN);
    }
    
    // implementing the move
    public void move (int x, int y)
    {
        Piece[][] brd = board.get_board();
        
        brd[this.x][this.y] = null;
        brd[x][y] = this;
        this.x = x;
        this.y = y;

        return;
    }

    private void steal_moves (Vector<Vector<Integer>> moves, Piece[][] brd)
    {
        Bishop b = new Bishop(this.x, this.y, side);
        brd[this.x][this.y] = b;

        Rook r = new Rook(this.x, this.y, side);
        brd[this.x][this.y] = r;

        moves.addAll(r.available_move(brd));
        moves.addAll(b.available_move(brd));
        
        brd[this.x][this.y] = this;
    }

    // the set of all unique moves of the specific piece
    public Vector<Vector<Integer>> available_move ()
    {
        Vector<Vector<Integer>> moves = new Vector<Vector<Integer>>();
        Piece[][] brd = board.get_board();
        steal_moves (moves, brd);
        
        return moves;
    }
    
    // the set of all unique moves of the specific piece using an input board
    public Vector<Vector<Integer>> available_move (Piece[][] brd)
    {
        Vector<Vector<Integer>> moves = new Vector<Vector<Integer>>();
        steal_moves (moves, brd);
        
        return moves;
    }
}
