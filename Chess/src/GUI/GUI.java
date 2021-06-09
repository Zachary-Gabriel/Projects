package GUI;

import processing.core.PApplet;
import Game.*;
import Pieces.*;

// The 'graphical user interface' so the user caan play the game
public class GUI extends PApplet
{

    public static void main(String[] args) {
        PApplet.main(new String[]{"GUI.GUI"});
    }
    
    // initial settings
    public void settings() 
    {
        size(800, 600);
    }
    
    // setup to the draw function
    public void setup() 
    {
        // makes the new board
        Board board = new Board();
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
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if ((i+j) % 2 == 1)
                    fill (92, 64, 51); // brown
                else
                    fill ( 102, 51, 0); // grey
                rect (i* (width/8), j* (height/8), (i+1)* (width/8), (j+1)* (height/8));
            }
        }
    }
    
    void draw_board ()
    {

    }
}
