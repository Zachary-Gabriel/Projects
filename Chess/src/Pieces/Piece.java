package Pieces;
import Game.*;

import java.util.*;

// represents the 'skeleton' of all the pieces
public abstract class Piece 
{
    int x, y; // coordinates for the pieces representing the rows
    Side side; // the side i.e. white / black
    Piece_type piece; // the piece i.e. bishop
    Board board;
    
    // constructor
    public Piece (int x, int y, Side side, Piece_type piece)
    {
        this.x = x;
        this.y = y;
        this.side = side;
        this.piece = piece;
    }
    
    // updates the coordinates of the piece
    public void set_x (int x)
    {
        this.x = x;
    }
    
    // returns the coordinates of the piece
    public int get_x ()
    {
        return this.x;
    }
    
    // updates the coordinates of the piece
    public void set_y (int y)
    {
        this.y = y;
    }
    
    // returns the coordinates of the piece
    public int get_y ()
    {
        return this.y;
    }
    
    // fetches the side that the piece is on
    public Side get_side ()
    {
        return this.side;
    }
    
    // fetches the coords of the piece
    public void set_piece (Piece_type piece)
    {
        this.piece = piece;
    }
    
    // fetches the name of the piece
    public Piece_type get_piece ()
    {
        return this.piece;
    }
    
    // updates the board
    public void update_board (Board board)
    {
        this.board = board;
    }
    
    // pushes the coords (x, y) of an elegable move to a moves vector
    protected void push_to_moves (Vector<Vector<Integer>> moves, int x, int y)
    {
        Vector<Integer> temp_vec = new Vector<Integer>();
        temp_vec.add (x);
        temp_vec.add (y);
        moves.add (temp_vec);
        return;
    }
    
    // checks if a move causes checkmate
    public void checkmate_check (Vector<Vector<Integer>> moves)
    {
        Piece[][] brd = board.get_board();
        
        Piece[][] dup_board = new Piece[8][8];
        for (int i = 0; i < 8; ++i) {for (int j = 0; j < 8; ++j) {dup_board[i][j] = brd[i][j];}}

        for (int i = 0; i < moves.size(); ++i)
        {
            dup_board[this.x][this.y] = null;
            dup_board[moves.get(i).get(0)][moves.get(i).get(1)] = this;
            
            int kingx = -1;
            int kingy = -1;
            boolean exit_early = false;
            boolean exit_early1 = false;

            for (int j = 0; j < 8; ++j)
            {
                for (int k = 0; k < 8; ++k)
                {
                    if (dup_board[j][k] != null)
                    {
                        if (dup_board[j][k].get_piece () == Piece_type.KING && dup_board[j][k].get_side () == this.side)
                        {
                            kingx = j;
                            kingy = k;
                            exit_early = true;
                            break ;
                        }
                    }
                }
                if (exit_early)
                {
                    break;
                }
            }
            
            exit_early = false;
            
            for (int j = 0; j < 8; ++j)
            {
                for (int k = 0; k < 8; ++k)
                {
                    if (dup_board[j][k] != null)
                    {
                        if (dup_board[j][k].get_side () != this.side)
                        {
                            Vector<Vector<Integer>> temp_moves = dup_board[j][k].available_move(dup_board);
                            for (int l = 0; l < temp_moves.size(); ++l)
                            {   
                                if ((temp_moves.get(l).get(0) == kingx) && (temp_moves.get(l).get(1) == kingy))
                                {
                                    moves.remove(i);
                                    i--;
                                    exit_early = true;
                                    exit_early1 = true;
                                    break;
                                }
                            }
                        }
                    }
                    if (exit_early1)
                    {
                        break;
                    }
                }
                if (exit_early)
                {
                    break;
                }
            }

            for (int j = 0; j < 8; ++j) 
            {
                for (int k = 0; k < 8; ++k) 
                {
                    dup_board[j][k] = brd[j][k];
                }
            }
        }
        return ;
    }
    
    // each piece has their own set of moves
    public abstract void move (int x, int y);
    public abstract Vector<Vector<Integer>> available_move ();
    public abstract Vector<Vector<Integer>> available_move (Piece[][] brd);
}
