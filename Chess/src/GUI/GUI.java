package GUI;

import processing.core.PApplet;
import processing.core.PImage;
import Game.*;
import Pieces.*;

// The 'graphical user interface' so the user caan play the game
public class GUI extends PApplet
{
    // private variables
    Board board;
    PImage[] images = new PImage[13]; // saves the images for faster performance

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
        // preloading images whilst runniong other instructions
        preload_images();

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
    public void draw () 
    {
        draw_board ();
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
                    fill (92, 64, 51); // dark tiles
                else
                    fill (102, 51, 0); // light tiles

                // if hovering
                if (i* (width /8) < mouseX && mouseX < (i+1)* (width /8) && 
                    j* (height /8) < mouseY && mouseY < (j+1)* (height /8))
                    fill (0); // black

                // draws the rectangle
                rect (i* (width /8), j* (height /8), (i+1)* (width /8), (j+1)* (height /8));
                
                // if there's an image on the square, draw it
                draw_image (i, j);
            }
        }
    }
    
    void draw_image (int i, int j)
    {
        // the image name. 'null' represents nothing
        String img_name = "null";

        // checking if a piece exists on said square
        if (board.get_board()[j][i] != null) 
        {
            // converting the piece into a letter
            switch (board.get_board()[j][i].get_piece())
            {
                case BISHOP:
                img_name = "Bishop";
                if (board.get_board()[j][i].get_side() == Side.BLACK)
                img_name += "_b";
                else
                img_name += "_w";
                break;
                
                case KING:
                img_name = "King";
                if (board.get_board()[j][i].get_side() == Side.BLACK)
                img_name += "_b";
                else
                img_name += "_w";
                break;
                
                case KNIGHT:
                img_name = "Knight";
                if (board.get_board()[j][i].get_side() == Side.BLACK)
                img_name += "_b";
                else
                img_name += "_w";
                break;
                
                case PAWN:
                img_name = "Pawn";
                if (board.get_board()[j][i].get_side() == Side.BLACK)
                img_name += "_b";
                else
                img_name += "_w";
                break;
                
                case QUEEN:
                img_name = "Queen";
                if (board.get_board()[j][i].get_side() == Side.BLACK)
                img_name += "_b";
                else
                img_name += "_w";
                break;
                
                case ROOK:
                img_name = "Rook";
                if (board.get_board()[j][i].get_side() == Side.BLACK)
                img_name += "_b";
                else
                img_name += "_w";
            }
            
            int index = name_to_index (img_name);
            if (index != -1)
            {
                image (images [index], i* (width/8), j* (height/8), (width/8) , (height/8));
            }
        }
        return ;
    }
    
    // Returns the index of the 'images array' where the image name is stored.
    int name_to_index (String img_name)
    {
        switch (img_name)
        {
            case "null": 
            return 0;
            case "Bishop_b":
            return 1;
            case "Bishop_w": 
            return 2;
            case "King_w": 
            return 3;
            case "King_b": 
            return 4;
            case "Knight_w": 
            return 5;
            case "Knight_b": 
            return 6;
            case "Pawn_w": 
            return 7;
            case "Pawn_b": 
            return 8;
            case "Queen_w": 
            return 9;
            case "Queen_b": 
            return 10;
            case "Rook_w": 
            return 11;
            case "Rook_b": 
            return 12;
        }
        return -1;
    }

    // Stores the images in an array so that theyre only loaded once.
    void preload_images ()
    {
        String img_name = "null.png";
        PImage img = loadImage (img_name);
        images[0] = img;
        
        img_name = "Bishop_b.png";
        img = loadImage (img_name);
        images[1] = img;

        img_name = "Bishop_w.png";
        img = loadImage (img_name);
        images[2] = img;
        
        img_name = "King_w.png";
        img = loadImage (img_name);
        images[3] = img;

        img_name = "King_b.png";
        img = loadImage (img_name);
        images[4] = img;
        
        img_name = "Knight_w.png";
        img = loadImage (img_name);
        images[5] = img;

        img_name = "Knight_b.png";
        img = loadImage (img_name);
        images[6] = img;
        
        img_name = "Pawn_w.png";
        img = loadImage (img_name);
        images[7] = img;

        img_name = "Pawn_b.png";
        img = loadImage (img_name);
        images[8] = img;
        
        img_name = "Queen_w.png";
        img = loadImage (img_name);
        images[9] = img;

        img_name = "Queen_b.png";
        img = loadImage (img_name);
        images[10] = img;
        
        img_name = "Rook_w.png";
        img = loadImage (img_name);
        images[11] = img;

        img_name = "Rook_b.png";
        img = loadImage (img_name);
        images[12] = img;
        
        return ;
    }
}
