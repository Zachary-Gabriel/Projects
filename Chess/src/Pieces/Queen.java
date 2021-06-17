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

    // the set of all unique moves of the specific piece
    public Vector<Vector<Integer>> available_move ()
    {
        Vector<Vector<Integer>> moves = new Vector<Vector<Integer>>();
        Board tmp_board = board;

        Bishop b = new Bishop(this.x, this.y, side);
        tmp_board.get_board()[this.x][this.y] = b;
        tmp_board.set_board (tmp_board.get_board());
        b.update_board(tmp_board);

        Rook r = new Rook(this.x, this.y, side);
        tmp_board.get_board()[this.x][this.y] = r;
        tmp_board.set_board (tmp_board.get_board());
        r.update_board(tmp_board);

        moves = b.available_move();
        for (int i = 0; i < r.available_move().size(); ++i)
        {
            moves.addElement(r.available_move().get(i));
        }
        
        tmp_board.get_board()[this.x][this.y] = this;
        tmp_board.set_board (tmp_board.get_board());
        this.update_board(board);
        
        return moves;
    }
}
