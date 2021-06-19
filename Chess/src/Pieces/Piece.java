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

    public boolean first_move = true; // too many pieces need this
    
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
    
    // finds the coords of the king given a board
    private int[] find_king_coords (Piece[][] dup_board)
    {   
        for (int j = 0; j < 8; ++j)
        {
            for (int k = 0; k < 8; ++k)
            {
                if (dup_board[j][k] != null)
                {
                    if (dup_board[j][k].get_piece () == Piece_type.KING && dup_board[j][k].get_side () == this.side)
                    {
                        int[] indices = new int[2];
                        indices[0] = j;
                        indices[1] = k;
                        return indices;
                    }
                }
            }
        }
        
        // halt due to problem
        System.err.println ("CANT FIND KING");
        int[] indices = new int[2];
        indices[0] = -1;
        indices[1] = -1;
        return indices;
    }
    
    // removes moves that cause an allied king capture
    // checks if a move causes checkmate and prevents it
    // boolean checks if theres any piece that attacks the square (used for castle check)
    public boolean checkmate_check (Vector<Vector<Integer>> moves)
    {
        Piece[][] brd = board.get_board();
        boolean ret_val = true;
        boolean exit_early = false;
        boolean exit_early1 = false;

        for (int i = 0; i < moves.size(); ++i)
        {
            exit_early = false;
            exit_early1 = false;

            Piece[][] dup_board = new Piece[8][8];
            for (int j = 0; j < 8; ++j) 
            {
                for (int k = 0; k < 8; ++k) 
                {
                    dup_board[j][k] = brd[j][k];
                }
            }

            dup_board[this.x][this.y] = null;
            dup_board[moves.get(i).get(0)][moves.get(i).get(1)] = this;
            
            int[] indices = find_king_coords (dup_board);
            int kingx = indices[0];
            int kingy = indices[1];

            for (int j = 0; j < 8; ++j) // board rows
            {
                for (int k = 0; k < 8; ++k) // board cols
                {
                    if (dup_board[j][k] != null) // check if valid piece
                    {
                        if (dup_board[j][k].get_side () != this.side) //  check if attacking side
                        {
                            Vector<Vector<Integer>> temp_moves = dup_board[j][k].available_move(dup_board); // get the available attacking moves
                            for (int l = 0; l < temp_moves.size(); ++l)
                            {   
                                // allied move causes attacker to take the king, remove the move from the allied move set
                                if ((temp_moves.get(l).get(0) == kingx) && (temp_moves.get(l).get(1) == kingy)) 
                                {
                                    moves.remove(i);
                                    i--;
                                    exit_early = true;
                                    exit_early1 = true;
                                    ret_val = false;
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
        }
        return ret_val; // if a move was removed, return false otherwise true
    }

    // each piece has their own set of moves
    public abstract void move (int x, int y);
    public abstract Vector<Vector<Integer>> available_move ();
    public abstract Vector<Vector<Integer>> available_move (Piece[][] brd);
}
