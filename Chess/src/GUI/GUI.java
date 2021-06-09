package GUI;

import processing.core.PApplet;
import processing.core.PImage;
import Game.*;
import Pieces.*;

// The 'graphical user interface' so the user caan play the game
public class GUI extends PApplet
{
    Board board;
    // the main function that runs the settings(), then setup() then draw() functions.
    public static void main (String[] args) { PApplet.main(new String[]{"GUI.GUI"}); }
    
    // initial settings
    public void settings() 
    {
        size(800, 600);
    }
    
    // setup to the draw function
    public void setup() 
    {
        // makes the new board
        board = new Board();
        board.initial_position ();
        
        // shows the board in the terminal
        Terminal_GUI tgui = new Terminal_GUI (board);
        tgui.terminal_board ();
        
        // draws the board
        draw_board ();
    }
    
    // Executes this code once per frame
    public void draw() 
    {
        
    }
    
    void draw_board ()
    {
        // columns (ranks)
        for (int j = 0; j < 8; ++j)
        {
            // rows (files)
            for (int i = 0; i < 8; ++i)
            {
                // tiles
                if ((i+j) % 2 == 1)
                    fill (92, 64, 51); // brown
                else
                    fill ( 102, 51, 0); // grey
                rect (i* (width/8), j* (height/8), (i+1)* (width/8), (j+1)* (height/8));

                // blank space representing no piece
                PImage img;
                String name = "null";
                
                //checking if a piece exists on said square
                if (board.get_board()[j][i] != null) 
                {
                    // converting the piece into a letter
                    switch (board.get_board()[j][i].get_piece())
                    {
                        case BISHOP:
                            name = "Bishop";
                            if (board.get_board()[j][i].get_side() == Side.BLACK)
                                name += "_b";
                            else
                                name += "_w";
                        break;
                        
                        case KING:
                            name = "King";
                            if (board.get_board()[j][i].get_side() == Side.BLACK)
                                name += "_b";
                            else
                                name += "_w";
                        break;

                        case KNIGHT:
                            name = "Knight";
                            if (board.get_board()[j][i].get_side() == Side.BLACK)
                                name += "_b";
                            else
                                name += "_w";
                        break;

                        case PAWN:
                            name = "Pawn";
                            if (board.get_board()[j][i].get_side() == Side.BLACK)
                                name += "_b";
                            else
                                name += "_w";
                        break;

                        case QUEEN:
                            name = "Queen";
                            if (board.get_board()[j][i].get_side() == Side.BLACK)
                                name += "_b";
                            else
                                name += "_w";
                        break;

                        case ROOK:
                            name = "Rook";
                            if (board.get_board()[j][i].get_side() == Side.BLACK)
                                name += "_b";
                            else
                                name += "_w";
                        break;

                        default:
                        name = "null";
                    }
                }
                System.out.println(name);
                img = loadImage(name);
                image (img, i* (width/8), j* (height/8), (width/8) , (height/8));
            } // end for i
        } // end for j
    } // end function
}
