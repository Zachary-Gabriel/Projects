package GUI;

import Game.*;
import Pieces.*;
import Tests.*;

public class Terminal_GUI 
{
    Board board;

    // default constructor. Likely won't be used
    public Terminal_GUI ()
    {
        this.board = new Board ();
    }

    // default constructor. The idea is that the game can be played on terminal making it easier implement a stockfish.
    public Terminal_GUI (Board board)
    {
        this.board = board;
    }

    // draws the board in ther terminal
    public void terminal_board ()
    {
        // columns (ranks)
        for (int j = 0; j < 8; ++j)
        {
            // rows (files)
            for (int i = 0; i < 8; ++i)
            {
                // blank space representing no piece
                char res = ' ';
                
                //checking if a piece exists on said square
                if (board.get_board()[j][i] != null) 
                {
                    // converting the piece into a letter
                    switch (board.get_board()[j][i].get_piece())
                    {
                        case BISHOP:
                        res = 'B';
                        break;
                        case KING:
                        res = 'K';
                        break;
                        case KNIGHT:
                        res = 'N';
                        break;
                        case PAWN:
                        res = 'P';
                        break;
                        case QUEEN:
                        res = 'Q';
                        break;
                        case ROOK:
                        res = 'R';
                    }
                }
                System.out.print (res);
            }
            System.out.println();
        }
    }
}
