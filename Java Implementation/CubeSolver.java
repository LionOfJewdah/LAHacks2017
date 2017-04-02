package cubesolver;
import java.util.Scanner;
public class CubeSolver {

    public static void main(String[] args) 
    {
        // TODO code application logic here
                //VARABLES
                /*
                 * The first and second array number is the location
                 * and the value is the color
                 * 1 = GREEN
                 * 2 = WHITE
                 * 3 = BLUE
                 * 4 = YELLOW
                 * 5 = ORANGE
                 * 6 = RED
                 */
        int Cube_colors[][] = new int[6][9];
        //get the colors that is on the cube right now
        Cube_colors = getColors();
        System.out.println("Checkpoint");
        
        //solve the white side
        Cube_colors = whiteSide(Cube_colors);
         
    }
    
    
    
    //<editor-fold defaultstate="collapsed" desc="The entire white side">
    public static int[][] whiteSide(int Cube_colors[][])
    {
        //make the cross for the white side
        Cube_colors = whiteCross(Cube_colors);
        //solve the edge pieces for the cube
        Cube_colors = whiteCorner(Cube_colors);
        return Cube_colors;
    }
    
    //<editor-fold defaultstate="collapsed" desc="The white corners">
    public static int[][] whiteCorner(int Cube_colors[][])
    {
        //get the green, orange, and white piece into place
        Cube_colors = whiteFirstCorner(Cube_colors);
        System.out.println("FIRST DONE");
        //get the blue, orange, and white piece into place
        Cube_colors = whiteSecondCorner(Cube_colors);
        System.out.println("SECOND DONE");
        //get the red, blue, and white piecie into place
        Cube_colors = whiteThirdCorner(Cube_colors);
        System.out.println("THIRD DONE");
        //get the green, red, and white piece into place
        Cube_colors = whiteFourthCorner(Cube_colors);
        System.out.println("FOURTH DONE");
        
        return Cube_colors;
    }
    public static int[][] whiteFirstCorner(int Cube_colors[][])
    {
        if(Cube_colors[0][0] == 2 && Cube_colors[4][0] == 5 && Cube_colors[3][2] == 1)
        {
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[0][2] == 2 && Cube_colors[1][0] == 5 && Cube_colors[4][6] == 1)
        {
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[0][6] == 2 && Cube_colors[3][8] == 5 && Cube_colors[5][6] == 1)
        {
            Cube_colors = WhiteCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[0][8] == 2 && Cube_colors[5][0] == 5 && Cube_colors[1][6] == 1)
        {
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[1][2] == 2 && Cube_colors[2][0] == 5 && Cube_colors[4][8] == 1)
        {
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[1][6] == 2 && Cube_colors[0][8] == 5 && Cube_colors[5][0] == 1)
        {
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[1][8] == 2 && Cube_colors[5][2] == 5 && Cube_colors[2][6] == 1)
        {
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[1][8] == 2 && Cube_colors[5][2] == 5 && Cube_colors[2][6] == 1)
        {
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[2][2] == 2 && Cube_colors[3][0] == 5 && Cube_colors[4][2] == 1)
        {
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[2][6] == 2 && Cube_colors[1][8] == 5 && Cube_colors[5][2] == 1)
        {
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);      
        }
        else if(Cube_colors[2][8] == 2 && Cube_colors[5][8] == 5 && Cube_colors[3][6] == 1)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[3][0] == 2 && Cube_colors[4][2] == 5 && Cube_colors[2][2] == 1)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[3][2] == 2 && Cube_colors[0][0] == 5 && Cube_colors[4][0] == 1)
        {
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[3][6] == 2 && Cube_colors[2][8] == 5 && Cube_colors[5][8] == 1)
        {
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);   
        }
        else if(Cube_colors[3][8] == 2 && Cube_colors[5][6] == 5 && Cube_colors[0][6] == 1)
        {
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[4][0] == 2 && Cube_colors[3][2] == 5 && Cube_colors[0][0] == 1)
        {
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[4][2] == 2 && Cube_colors[2][2] == 5 && Cube_colors[3][0] == 1)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[4][6] == 2 && Cube_colors[0][2] == 5 && Cube_colors[1][0] == 1)
        {
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[4][8] == 2 && Cube_colors[1][2] == 5 && Cube_colors[2][0] == 1)
        {
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[5][0] == 2 && Cube_colors[1][6] == 5 && Cube_colors[0][8] == 1)
        {
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[5][2] == 2 && Cube_colors[2][6] == 5 && Cube_colors[1][8] == 1)
        {
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[5][6] == 2 && Cube_colors[0][6] == 5 && Cube_colors[3][8] == 1)
        {
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[5][8] == 2 && Cube_colors[3][6] == 5 && Cube_colors[2][8] == 1)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }       
        return Cube_colors;
    }
    public static int[][] whiteSecondCorner(int Cube_colors[][])
    {
        if(Cube_colors[0][0] == 2 && Cube_colors[4][0] == 1 && Cube_colors[3][2] == 6)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
        }
        // 02 is where the white orange green is
        else if(Cube_colors[0][6] == 2 && Cube_colors[3][8] == 1 && Cube_colors[5][6] == 6)
        {
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
        }
        else if(Cube_colors[0][8] == 2 && Cube_colors[5][0] == 1 && Cube_colors[1][6] == 6)
        {
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
        }
        else if(Cube_colors[1][2] == 2 && Cube_colors[2][0] == 1 && Cube_colors[4][8] == 6)
        {
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
        }
        else if(Cube_colors[1][6] == 2 && Cube_colors[0][8] == 1 && Cube_colors[5][0] == 6)
        {
            //this is the place it should be in so its fine
        }
        else if(Cube_colors[1][8] == 2 && Cube_colors[5][2] == 1 && Cube_colors[2][6] == 6)
        {
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
        }
        else if(Cube_colors[2][0] == 2 && Cube_colors[4][8] == 1 && Cube_colors[1][2] == 6)
        {
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
        }
        else if(Cube_colors[2][2] == 2 && Cube_colors[3][0] == 1 && Cube_colors[4][2] == 6)
        {
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
        }
        else if(Cube_colors[2][6] == 2 && Cube_colors[1][8] == 1 && Cube_colors[5][2] == 6)
        {
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
        }
        else if(Cube_colors[3][0] == 2 && Cube_colors[4][2] == 1 && Cube_colors[2][2] == 6)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
        }
        else if(Cube_colors[3][2] == 2 && Cube_colors[0][0] == 1 && Cube_colors[4][0] == 6)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
        }
        else if(Cube_colors[3][6] == 2 && Cube_colors[2][8] == 1 && Cube_colors[5][8] == 6)
        {
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
        }
        else if(Cube_colors[3][8] == 2 && Cube_colors[5][6] == 1 && Cube_colors[0][6] == 6)
        {
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
        }
        //for 46 there is already something there
        else if(Cube_colors[4][0] == 2 && Cube_colors[3][2] == 1 && Cube_colors[0][0] == 6)
        {
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
        }
        else if(Cube_colors[4][2] == 2 && Cube_colors[2][2] == 1 && Cube_colors[3][0] == 6)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
        }
        else if(Cube_colors[4][8] == 2 && Cube_colors[1][2] == 1 && Cube_colors[2][0] == 6)
        {
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
        }
        else if(Cube_colors[5][0] == 2 && Cube_colors[1][6] == 1 && Cube_colors[0][8] == 6)
        {
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
        }
        else if(Cube_colors[5][2] == 2 && Cube_colors[2][6] == 1 && Cube_colors[1][8] == 6)
        {
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
        }
        else if(Cube_colors[5][6] == 2 && Cube_colors[0][6] == 1 && Cube_colors[3][8] == 6)
        {
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
        }
        else if(Cube_colors[5][8] == 2 && Cube_colors[3][6] == 1 && Cube_colors[2][8] == 6)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
        }
        return Cube_colors;
    }
    public static int[][] whiteThirdCorner(int Cube_colors[][])
    {
        if(Cube_colors[0][0] == 2 && Cube_colors[4][0] == 5 && Cube_colors[3][2] == 3)
        {
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        //wont be here because it is already taken
        else if(Cube_colors[0][6] == 2 && Cube_colors[5][6] == 5 && Cube_colors[3][8] == 3)
        {
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        //wont be here because 08 is already take
        // wint be here because 10 is already taken
        //if it is here then it is in the right place
        //wont be here because 16 is already taken
        else if(Cube_colors[1][8] == 2 && Cube_colors[2][6] == 5 && Cube_colors[5][2] == 3)
        {
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        else if(Cube_colors[2][0] == 2 && Cube_colors[1][2] == 5 && Cube_colors[4][8] == 3)
        {
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        else if(Cube_colors[2][2] == 2 && Cube_colors[4][2] == 5 && Cube_colors[3][0] == 3)
        {
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        else if(Cube_colors[2][6] == 2 && Cube_colors[5][2] == 5 && Cube_colors[1][8] == 3)
        {
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = OrangeCCW(Cube_colors);
        }
        else if(Cube_colors[2][8] == 2 && Cube_colors[3][6] == 5 && Cube_colors[5][8] == 3)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        else if(Cube_colors[3][0] == 2 && Cube_colors[2][2] == 5 && Cube_colors[4][2] == 3)
        {
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        else if(Cube_colors[3][2] == 2 && Cube_colors[4][0] == 5 && Cube_colors[0][0] == 3)
        {
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        else if(Cube_colors[3][6] == 2 && Cube_colors[5][8] == 5 && Cube_colors[2][8] == 3)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        else if(Cube_colors[3][8] == 2 && Cube_colors[0][6] == 5 && Cube_colors[5][6] == 3)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        else if(Cube_colors[4][0] == 2 && Cube_colors[0][0] == 5 && Cube_colors[3][2] == 3)
        {
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        else if(Cube_colors[4][2] == 2 && Cube_colors[3][0] == 5 && Cube_colors[2][2] == 3)
        {
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        else if(Cube_colors[5][2] == 2 && Cube_colors[1][8] == 5 && Cube_colors[2][6] == 3)
        {
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        else if(Cube_colors[5][6] == 2 && Cube_colors[3][8] == 5 && Cube_colors[0][6] == 3)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        else if(Cube_colors[5][8] == 2 && Cube_colors[2][8] == 5 && Cube_colors[3][6] == 3)
        {
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        return Cube_colors;
    }
    public static int[][] whiteFourthCorner(int Cube_colors[][])
    {
        if(Cube_colors[0][0] == 2 && Cube_colors[3][2] == 3 && Cube_colors[4][0] == 6)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
        }
        //it wont be at 02 because there is already a picec there
        else if(Cube_colors[0][6] == 2 && Cube_colors[5][6] == 3 && Cube_colors[3][8] == 6)
        {
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
        }
        //it wouldnt be at 08 becuase there is already a piece there
        //10 piece already there
        //12 piece alrady there
        //16 piece already there
        //18 if it is there it is in the right place
        //20 cant becuase there is a piece already there
        else if(Cube_colors[2][2] == 2 && Cube_colors[4][2] == 3 && Cube_colors[3][0] == 6)
        {
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
        }
        else if(Cube_colors[2][6] == 2 && Cube_colors[5][2] == 3 && Cube_colors[1][8] == 6)
        {
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
        }
        else if(Cube_colors[2][8] == 2 && Cube_colors[3][6] == 3 && Cube_colors[5][8] == 6)
        {
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
        }
        else if(Cube_colors[3][0] == 2 && Cube_colors[2][2] == 3 && Cube_colors[4][2] == 6)
        {
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
        }
        else if(Cube_colors[3][2] == 2 && Cube_colors[4][0] == 3 && Cube_colors[0][0] == 6)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
        }
        else if(Cube_colors[3][6] == 2 && Cube_colors[5][8] == 3 && Cube_colors[2][8] == 6)
        {
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
        }
        else if(Cube_colors[3][8] == 2 && Cube_colors[0][6] == 3 && Cube_colors[5][6] == 6)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
        }
        else if(Cube_colors[3][8] == 2 && Cube_colors[0][6] == 3 && Cube_colors[5][6] == 6)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
        }
        else if(Cube_colors[4][0] == 2 && Cube_colors[0][0] == 3 && Cube_colors[3][2] == 6)
        {
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
        }
        else if(Cube_colors[4][2] == 2 && Cube_colors[2][2] == 3 && Cube_colors[3][0] == 6)
        {
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
        }
        //50 is already taken by another poiece 
        else if(Cube_colors[5][2] == 2 && Cube_colors[1][8] == 3 && Cube_colors[2][6] == 6)
        {
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
        }
        else if(Cube_colors[5][6] == 2 && Cube_colors[3][8] == 3 && Cube_colors[0][6] == 6)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
        }
        else if(Cube_colors[5][8] == 2 && Cube_colors[2][8] == 3 && Cube_colors[3][6] == 6)
        {
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
        }
        return Cube_colors;
    }
//</editor-fold>
    
    //<editor-fold defaultstate="collapsed" desc="The white cross">
    public static int[][] whiteCross(int Cube_colors[][])
    {
        //get the green and shite piece into place
        Cube_colors = whiteFirstEdge(Cube_colors);
        //get the orange and white piece into place
        Cube_colors = whiteSecondEdge(Cube_colors);
        //get the bloue and white piece into place
        Cube_colors = whiteThirdEdge(Cube_colors);
        //get the red and white piece into place
        Cube_colors = whiteFourthEdge(Cube_colors);
        
        return Cube_colors;
    }
    public static int[][] whiteFirstEdge(int Cube_colors[][])
    {
        /*
        in this method we are going to take
        the white/green corner piece and put
        it in the right place
        */
        if(Cube_colors[0][1] == 2 && Cube_colors[4][3] == 1)
        {
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
        }
        else if(Cube_colors[0][3] == 2 && Cube_colors[3][5] == 1)
        {
            Cube_colors = GreenCW(Cube_colors);
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
        }
        else if(Cube_colors[0][5] == 2 && Cube_colors[1][3] == 1)
        {
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
        }
        else if(Cube_colors[0][7] == 2 && Cube_colors[5][3] == 1)
        {
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[1][1] ==  2 && Cube_colors[4][7] == 1)
        {
            Cube_colors = WhiteCCW(Cube_colors);
        }
        else if(Cube_colors[1][3] == 2 && Cube_colors[0][5] == 1)
        {
            //this is in the right place so it doesnt need to be moved
        }
        else if(Cube_colors[1][5] == 2 && Cube_colors[2][3] == 1)
        {
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[1][7] == 2 && Cube_colors[5][1] == 1)
        {
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[2][1] == 2 && Cube_colors[4][5] == 1)
        {
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
        }
        else if(Cube_colors[2][3] == 2 && Cube_colors[1][5] == 1)
        {
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
        }
        else if(Cube_colors[2][5] == 2 && Cube_colors[3][3] == 1)
        {
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
        }
        else if(Cube_colors[2][7] == 2 && Cube_colors[5][5] == 1)
        {
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
        }
        else if(Cube_colors[3][1] == 2 && Cube_colors[4][1] == 1)
        {
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
        }
        else if(Cube_colors[3][3] == 2 && Cube_colors[2][5] == 1)
        {
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[3][5] == 2 && Cube_colors[0][3] == 1)
        {
            Cube_colors = GreenCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[3][7] == 2 && Cube_colors[5][7] == 1)
        {
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[4][1] == 2 && Cube_colors[3][1] == 1)
        {
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[4][3] == 2 && Cube_colors[0][1] == 1)
        {
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[4][5] == 2 && Cube_colors[2][1] == 1)
        {
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[4][7] == 2 && Cube_colors[1][1] == 1)
        {
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[5][1] == 2 && Cube_colors[1][7] == 1)
        {
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
        }
        else if(Cube_colors[5][3] == 2 && Cube_colors[0][7] == 1)
        {
            Cube_colors = GreenCCW(Cube_colors);
        }
        else if(Cube_colors[5][5] == 2 && Cube_colors[2][7] == 1)
        {
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
        }
        else if(Cube_colors[5][7] == 2 && Cube_colors[3][7] == 1)
        {
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
        }
        return Cube_colors;
    }
    public static int[][] whiteSecondEdge(int Cube_colors[][])
    {
        if(Cube_colors[0][1] == 2 && Cube_colors[4][3] == 5)
        {
            Cube_colors = OrangeCCW(Cube_colors);
        }
        else if(Cube_colors[0][3] == 2 && Cube_colors[3][5] == 5)
        {
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[0][5] == 2 && Cube_colors[1][3] == 5)
        {
            //it wouldnt be here because the pice is aready in place
        }
        else if(Cube_colors[0][7] == 2 && Cube_colors[5][3] == 5)
        {
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[1][1] == 2 && Cube_colors[4][7] == 5)
        {
            //this piece is in the right place
        }
        else if(Cube_colors[1][3] == 2 && Cube_colors[0][5] == 5)
        {
            //it wouldn't be here because the other picee is already in this place
        }
        else if(Cube_colors[1][5] == 2 && Cube_colors[2][3] == 5)
        {
            Cube_colors = GreenCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            
        }
        else if(Cube_colors[1][7] == 2 && Cube_colors[5][1] == 5)
        {
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
        }
        else if(Cube_colors[2][1] == 2 && Cube_colors[4][5] == 5)
        {
            Cube_colors = OrangeCW(Cube_colors);
        }
        else if(Cube_colors[2][3] == 2 && Cube_colors[1][5] == 5)
        {
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        else if(Cube_colors[2][5] == 2 && Cube_colors[3][3] == 5)
        {
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        else if(Cube_colors[2][7] == 2 && Cube_colors[5][5] == 5)
        {
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        else if(Cube_colors[3][1] == 2 && Cube_colors[4][1] == 5)
        {
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        else if(Cube_colors[3][3] == 2 && Cube_colors[2][5] == 5)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        else if(Cube_colors[3][5] == 2 && Cube_colors[0][3] == 5)
        {
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        else if(Cube_colors[3][7] == 2 && Cube_colors[5][7] == 5)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        else if(Cube_colors[4][1] == 2 && Cube_colors[3][1] == 5)
        {
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        else if(Cube_colors[4][3] == 2 && Cube_colors[0][1] == 5)
        {
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        else if(Cube_colors[4][5] == 2 && Cube_colors[2][1] == 5)
        {
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
        }
        else if(Cube_colors[4][7] == 2 && Cube_colors[1][1] == 5)
        {
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
        }
        else if(Cube_colors[5][1] == 2 && Cube_colors[1][7] == 5)
        {
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
        }
        else if(Cube_colors[5][3] == 2 && Cube_colors[0][7] == 5)
        {
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
        }
        else if(Cube_colors[5][5] == 2 && Cube_colors[2][7] == 5)
        {
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
        }
        else if(Cube_colors[5][7] == 2 && Cube_colors[3][7] == 5)
        {
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
        }
        return Cube_colors;
    }
    public static int[][] whiteThirdEdge(int Cube_colors[][])
    {
        if(Cube_colors[0][1] == 2 && Cube_colors[4][3] == 3)
        {
            Cube_colors = WhiteCCW(Cube_colors);
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[0][3] == 2 && Cube_colors[3][5] == 3)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
        }
        else if(Cube_colors[0][5] == 2 && Cube_colors[1][3] == 3)
        {
            /*
            it cant be in this place
            because there is another piece in this place
            */
        }
        else if(Cube_colors[0][7] == 2 && Cube_colors[5][3] == 3)
        {
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
        }
        else if(Cube_colors[1][1] == 2 && Cube_colors[4][7] == 3)
        {
            /*
            It cant be here because
            there is another piece here
            */
        }
        else if(Cube_colors[1][3] == 2 && Cube_colors[0][5] == 3)
        {
            /*
            It cant be here because
            there is another piece here
            */
        }
        else if(Cube_colors[1][5] == 2 && Cube_colors[2][3] == 3)
        {
            /*
            if it is here then it is in the right place
            */
        }
        else if(Cube_colors[1][7] == 2 && Cube_colors[5][1] == 3)
        {
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
        }
        else if(Cube_colors[2][1] == 2 && Cube_colors[4][5] == 3)
        {
            Cube_colors = WhiteCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[2][3] == 2 && Cube_colors[1][5] == 3)
        {
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[2][5] == 2 && Cube_colors[3][3] == 3)
        {
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[2][7] == 2 && Cube_colors[5][5] == 3)
        {
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[3][1] == 2 && Cube_colors[4][1] == 3)
        {
            Cube_colors = WhiteCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[3][3] == 2 && Cube_colors[2][5] == 3)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[3][5] == 2 && Cube_colors[0][3] == 3)
        {
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[3][7] == 2 && Cube_colors[5][7] == 3)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[4][1] == 2 && Cube_colors[3][1] == 3)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
        }
        else if(Cube_colors[4][3] == 2 && Cube_colors[0][1] == 3)
        {
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[4][5] == 2 && Cube_colors[2][1] == 3)
        {
            Cube_colors = BlueCCW(Cube_colors);
        }
        else if(Cube_colors[4][7] == 2 && Cube_colors[1][1] == 3)
        {
            /*
            cant be here becaue there 
            is already another piece here
            */
        }
        else if(Cube_colors[5][1] == 2 && Cube_colors[1][7] == 3)
        {
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
        }
        else if(Cube_colors[5][3] == 2 && Cube_colors[0][7] == 3)
        {
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
        }
        else if(Cube_colors[5][5] == 2 && Cube_colors[2][7] == 3)
        {
            Cube_colors = RedCW(Cube_colors);
        }
        else if(Cube_colors[5][7] == 2 && Cube_colors[3][7] == 3)
        {
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
        }
        return Cube_colors;
    }
    public static int[][] whiteFourthEdge(int Cube_colors[][])
    {
        if(Cube_colors[0][1] == 2 && Cube_colors[4][3] == 6)
        {
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[0][3] == 2 && Cube_colors[3][5] == 6)
        {
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = OrangeCCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[0][5] == 2 && Cube_colors[1][3] == 6)
        {
            //it wouldnt be here because there is the other piece there
        }
        else if(Cube_colors[0][7] == 2 && Cube_colors[5][3] == 6)
        {
            Cube_colors = RedCW(Cube_colors);
        }
        else if(Cube_colors[1][1] == 2 && Cube_colors[4][7] == 6)
        {
            //it wouldnt be here becasue the other piece is there
        }
        else if(Cube_colors[1][3] == 2 && Cube_colors[0][5] == 6)
        {
            //it woulndt be here becasue the other piece is there
        }
        else if(Cube_colors[1][5] == 2 && Cube_colors[2][3] == 6)
        {
            //it woulnt be here becasue the other piece is there
        }
        else if(Cube_colors[1][7] == 2 && Cube_colors[5][1] == 6)
        {
            //if it is here then it is in ther ight place
        }
        else if(Cube_colors[2][1] == 2 && Cube_colors[4][5] == 6)
        {
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[2][3] == 2 && Cube_colors[1][5] == 6)
        {
            //it wouldnt be here because there is already a piece there
        }
        else if(Cube_colors[2][5] == 2 && Cube_colors[3][3] == 6)
        {
            Cube_colors = WhiteCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = RedCCW(Cube_colors);
        }
        else if(Cube_colors[2][7] == 2 && Cube_colors[5][5] == 6)
        {
            Cube_colors = RedCCW(Cube_colors);
        }
        else if(Cube_colors[3][1] == 2 && Cube_colors[4][1] == 6)
        {
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[3][3] == 2 && Cube_colors[2][5] == 6)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);            
        }
        else if(Cube_colors[3][5] == 2 && Cube_colors[0][3] == 6)
        {
            Cube_colors = YellowCCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[3][7] == 2 && Cube_colors[5][7] == 6)
        {
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = YellowCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[4][1] == 2 && Cube_colors[3][1] == 6)
        {
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = OrangeCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[4][3] == 2 && Cube_colors[0][1] == 6)
        {
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = GreenCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
        }
        else if(Cube_colors[4][5] == 2 && Cube_colors[2][1] == 6)
        {
            Cube_colors = WhiteCCW(Cube_colors);
            Cube_colors = BlueCCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[4][7] == 2 && Cube_colors[1][1] == 6)
        {
            //it wouldnt be here becaue there is already a piece here
        }
        else if(Cube_colors[5][1] == 2 && Cube_colors[1][7] == 6)
        {
            Cube_colors = RedCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[5][3] == 2 && Cube_colors[0][7] == 6)
        {
            Cube_colors = WhiteCW(Cube_colors);
            Cube_colors = GreenCCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
        }
        else if(Cube_colors[5][5] == 2 && Cube_colors[2][7] == 6)
        {
            Cube_colors = WhiteCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        else if(Cube_colors[5][7] == 2 && Cube_colors[3][7] == 6)
        {
            Cube_colors = RedCCW(Cube_colors);
            Cube_colors = WhiteCCW(Cube_colors);
            Cube_colors = BlueCW(Cube_colors);
            Cube_colors = WhiteCW(Cube_colors);
        }
        return Cube_colors;
    }
//</editor-fold>
//</editor-fold>

    //<editor-fold defaultstate="collapsed" desc="Turns and color collection">
    public static int[][] GreenCW(int colors[][])
    {
        //First you have to change the face parts
        int[] primaryFace = new int[62];
        primaryFace[0] = colors[0][0];
        primaryFace[1] = colors[0][1];
        primaryFace[2] = colors[0][2];
        primaryFace[3] = colors[0][3];
        primaryFace[4] = colors[0][4];
        primaryFace[5] = colors[0][5];
        primaryFace[6] = colors[0][6];
        primaryFace[7] = colors[0][7];
        primaryFace[8] = colors[0][8];
        
        colors[0][2] = primaryFace[0];
        colors[0][5] = primaryFace[1];
        colors[0][8] = primaryFace[2];
        colors[0][1] = primaryFace[3];
        colors[0][4] = primaryFace[4];
        colors[0][7] = primaryFace[5];
        colors[0][0] = primaryFace[6];
        colors[0][3] = primaryFace[7];
        colors[0][6] = primaryFace[8];
        
        //Then you have to change the outer parts
        int[] primaryOuter = new int[62];
        primaryOuter[40] = colors[4][0];
        primaryOuter[43] = colors[4][3];
        primaryOuter[46] = colors[4][6];
        primaryOuter[10] = colors[1][0];
        primaryOuter[13] = colors[1][3];
        primaryOuter[16] = colors[1][6];
        primaryOuter[50] = colors[5][0];
        primaryOuter[53] = colors[5][3];
        primaryOuter[56] = colors[5][6];
        primaryOuter[38] = colors[3][8];
        primaryOuter[35] = colors[3][5];
        primaryOuter[32] = colors[3][2];
        
        colors[1][0] = primaryOuter[40];
        colors[1][3] = primaryOuter[43];
        colors[1][6] = primaryOuter[46];
        colors[5][0] = primaryOuter[10];
        colors[5][3] = primaryOuter[13];
        colors[5][6] = primaryOuter[16];
        colors[3][8] = primaryOuter[50];
        colors[3][5] = primaryOuter[53];
        colors[3][2] = primaryOuter[56];
        colors[4][0] = primaryOuter[38];
        colors[4][3] = primaryOuter[35];
        colors[4][5] = primaryOuter[32];
        
        //now you have to new values ater turning the green clockwise once
        System.out.println("GreenClockWise");
        return colors;
    }
    public static int[][] GreenCCW(int colors[][])
    {
        //First you have to change the face parts
        int[] primaryFace = new int[62];
        primaryFace[0] = colors[0][0];
        primaryFace[1] = colors[0][1];
        primaryFace[2] = colors[0][2];
        primaryFace[3] = colors[0][3];
        primaryFace[4] = colors[0][4];
        primaryFace[5] = colors[0][5];
        primaryFace[6] = colors[0][6];
        primaryFace[7] = colors[0][7];
        primaryFace[8] = colors[0][8];
        
        colors[0][6] = primaryFace[0];
        colors[0][3] = primaryFace[1];
        colors[0][0] = primaryFace[2];
        colors[0][7] = primaryFace[3];
        colors[0][4] = primaryFace[4];
        colors[0][1] = primaryFace[5];
        colors[0][8] = primaryFace[6];
        colors[0][5] = primaryFace[7];
        colors[0][2] = primaryFace[8];
        
        //Then you have to change the outer parts
        int[] primaryOuter = new int[62];
        primaryOuter[40] = colors[4][0];
        primaryOuter[43] = colors[4][3];
        primaryOuter[46] = colors[4][6];
        primaryOuter[10] = colors[1][0];
        primaryOuter[13] = colors[1][3];
        primaryOuter[16] = colors[1][6];
        primaryOuter[50] = colors[5][0];
        primaryOuter[53] = colors[5][3];
        primaryOuter[56] = colors[5][6];
        primaryOuter[38] = colors[3][8];
        primaryOuter[35] = colors[3][5];
        primaryOuter[32] = colors[3][2];
        
        colors[3][8] = primaryOuter[40];
        colors[3][5] = primaryOuter[43];
        colors[3][2] = primaryOuter[46];
        colors[4][0] = primaryOuter[10];
        colors[4][3] = primaryOuter[13];
        colors[4][6] = primaryOuter[16];
        colors[1][0] = primaryOuter[50];
        colors[1][3] = primaryOuter[53];
        colors[1][6] = primaryOuter[56];
        colors[5][0] = primaryOuter[38];
        colors[5][3] = primaryOuter[35];
        colors[5][6] = primaryOuter[32];
        
        //now you have to new values ater turning the green clockwise once
        System.out.println("GreenCounterClockWise");
        return colors;
    }
    public static int[][] WhiteCW(int colors[][])
    {
        //First you have to change the face parts
        int[] primaryFace = new int[62];
        primaryFace[0] = colors[1][0];
        primaryFace[1] = colors[1][1];
        primaryFace[2] = colors[1][2];
        primaryFace[3] = colors[1][3];
        primaryFace[4] = colors[1][4];
        primaryFace[5] = colors[1][5];
        primaryFace[6] = colors[1][6];
        primaryFace[7] = colors[1][7];
        primaryFace[8] = colors[1][8];
        
        colors[1][2] = primaryFace[0];
        colors[1][5] = primaryFace[1];
        colors[1][8] = primaryFace[2];
        colors[1][1] = primaryFace[3];
        colors[1][4] = primaryFace[4];
        colors[1][7] = primaryFace[5];
        colors[1][0] = primaryFace[6];
        colors[1][3] = primaryFace[7];
        colors[1][6] = primaryFace[8];
        
        //Then you have to change the outer parts
        int[] primaryOuter = new int[62];
        primaryOuter[46] = colors[4][6];
        primaryOuter[47] = colors[4][7];
        primaryOuter[48] = colors[4][8];
        primaryOuter[20] = colors[2][0];
        primaryOuter[23] = colors[2][3];
        primaryOuter[26] = colors[2][6];
        primaryOuter[52] = colors[0][8];
        primaryOuter[51] = colors[0][5];
        primaryOuter[50] = colors[0][2];
        primaryOuter[8] = colors[0][8];
        primaryOuter[5] = colors[0][5];
        primaryOuter[2] = colors[0][2];
        
        colors[2][0] = primaryOuter[46];
        colors[2][3] = primaryOuter[47];
        colors[2][6] = primaryOuter[48];
        colors[5][2] = primaryOuter[20];
        colors[5][1] = primaryOuter[23];
        colors[5][0] = primaryOuter[26];
        colors[0][8] = primaryOuter[52];
        colors[0][5] = primaryOuter[51];
        colors[0][2] = primaryOuter[50];
        colors[4][6] = primaryOuter[8];
        colors[4][7] = primaryOuter[5];
        colors[4][8] = primaryOuter[2];
        
        //now you have to new values ater turning the White clockwise once
        System.out.println("WhiteClockWise");
        return colors;
    }
    public static int[][] WhiteCCW(int colors[][])
    {
        //First you have to change the face parts
        int[] primaryFace = new int[62];
        primaryFace[0] = colors[1][0];
        primaryFace[1] = colors[1][1];
        primaryFace[2] = colors[1][2];
        primaryFace[3] = colors[1][3];
        primaryFace[4] = colors[1][4];
        primaryFace[5] = colors[1][5];
        primaryFace[6] = colors[1][6];
        primaryFace[7] = colors[1][7];
        primaryFace[8] = colors[1][8];
        
        colors[1][6] = primaryFace[0];
        colors[1][3] = primaryFace[1];
        colors[1][0] = primaryFace[2];
        colors[1][7] = primaryFace[3];
        colors[1][4] = primaryFace[4];
        colors[1][1] = primaryFace[5];
        colors[1][8] = primaryFace[6];
        colors[1][5] = primaryFace[7];
        colors[1][2] = primaryFace[8];
        
        //Then you have to change the outer parts
        int[] primaryOuter = new int[62];
        primaryOuter[46] = colors[4][6];
        primaryOuter[47] = colors[4][7];
        primaryOuter[48] = colors[4][8];
        primaryOuter[20] = colors[2][0];
        primaryOuter[23] = colors[2][3];
        primaryOuter[26] = colors[2][6];
        primaryOuter[52] = colors[0][8];
        primaryOuter[51] = colors[0][5];
        primaryOuter[50] = colors[0][2];
        primaryOuter[8] = colors[0][8];
        primaryOuter[5] = colors[0][5];
        primaryOuter[2] = colors[0][2];
        
        colors[0][8] = primaryOuter[46];
        colors[0][5] = primaryOuter[47];
        colors[0][2] = primaryOuter[48];
        colors[4][6] = primaryOuter[20];
        colors[4][7] = primaryOuter[23];
        colors[4][8] = primaryOuter[26];
        colors[2][0] = primaryOuter[52];
        colors[2][3] = primaryOuter[51];
        colors[2][6] = primaryOuter[50];
        colors[5][2] = primaryOuter[8];
        colors[5][1] = primaryOuter[5];
        colors[5][0] = primaryOuter[2];
        
        //now you have to new values ater turning the White CounterClokwise once
        System.out.println("WhiteCounterClockWise");
        return colors;
    }
    public static int[][] BlueCW(int colors[][])
    {
        //First you have to change the face parts
        int[] primaryFace = new int[62];
        primaryFace[0] = colors[2][0];
        primaryFace[1] = colors[2][1];
        primaryFace[2] = colors[2][2];
        primaryFace[3] = colors[2][3];
        primaryFace[4] = colors[2][4];
        primaryFace[5] = colors[2][5];
        primaryFace[6] = colors[2][6];
        primaryFace[7] = colors[2][7];
        primaryFace[8] = colors[2][8];
        
        colors[2][2] = primaryFace[0];
        colors[2][5] = primaryFace[1];
        colors[2][8] = primaryFace[2];
        colors[2][1] = primaryFace[3];
        colors[2][4] = primaryFace[4];
        colors[2][7] = primaryFace[5];
        colors[2][0] = primaryFace[6];
        colors[2][3] = primaryFace[7];
        colors[2][6] = primaryFace[8];
        
        //Then you have to change the outer parts
        int[] primaryOuter = new int[62];
        primaryOuter[48] = colors[4][8];
        primaryOuter[45] = colors[4][5];
        primaryOuter[42] = colors[4][2];
        primaryOuter[30] = colors[3][0];
        primaryOuter[33] = colors[3][3];
        primaryOuter[36] = colors[3][6];
        primaryOuter[58] = colors[5][8];
        primaryOuter[55] = colors[5][5];
        primaryOuter[52] = colors[5][2];
        primaryOuter[18] = colors[1][8];
        primaryOuter[15] = colors[1][5];
        primaryOuter[12] = colors[1][2];
        
        colors[3][0] = primaryOuter[48];
        colors[3][3] = primaryOuter[45];
        colors[3][6] = primaryOuter[42];
        colors[5][8] = primaryOuter[30];
        colors[5][5] = primaryOuter[33];
        colors[5][2] = primaryOuter[36];
        colors[1][8] = primaryOuter[58];
        colors[1][5] = primaryOuter[55];
        colors[1][2] = primaryOuter[52];
        colors[4][8] = primaryOuter[18];
        colors[4][5] = primaryOuter[15];
        colors[4][2] = primaryOuter[12];
        
        //now you have to new values ater turning the White CounterClokwise once
        System.out.println("BlueClockWise");
        return colors;
    }
    public static int[][] BlueCCW(int colors[][])
    {
        //First you have to change the face parts
        int[] primaryFace = new int[62];
        primaryFace[0] = colors[2][0];
        primaryFace[1] = colors[2][1];
        primaryFace[2] = colors[2][2];
        primaryFace[3] = colors[2][3];
        primaryFace[4] = colors[2][4];
        primaryFace[5] = colors[2][5];
        primaryFace[6] = colors[2][6];
        primaryFace[7] = colors[2][7];
        primaryFace[8] = colors[2][8];
        
        colors[2][6] = primaryFace[0];
        colors[2][3] = primaryFace[1];
        colors[2][0] = primaryFace[2];
        colors[2][7] = primaryFace[3];
        colors[2][4] = primaryFace[4];
        colors[2][1] = primaryFace[5];
        colors[2][8] = primaryFace[6];
        colors[2][5] = primaryFace[7];
        colors[2][2] = primaryFace[8];
        
        //Then you have to change the outer parts
        int[] primaryOuter = new int[62];
        primaryOuter[48] = colors[4][8];
        primaryOuter[45] = colors[4][5];
        primaryOuter[42] = colors[4][2];
        primaryOuter[30] = colors[3][0];
        primaryOuter[33] = colors[3][3];
        primaryOuter[36] = colors[3][6];
        primaryOuter[58] = colors[5][8];
        primaryOuter[55] = colors[5][5];
        primaryOuter[52] = colors[5][2];
        primaryOuter[18] = colors[1][8];
        primaryOuter[15] = colors[1][5];
        primaryOuter[12] = colors[1][2];
        
        colors[1][8] = primaryOuter[48];
        colors[1][5] = primaryOuter[45];
        colors[1][2] = primaryOuter[42];
        colors[4][8] = primaryOuter[30];
        colors[4][5] = primaryOuter[33];
        colors[4][2] = primaryOuter[36];
        colors[3][0] = primaryOuter[58];
        colors[3][3] = primaryOuter[55];
        colors[3][6] = primaryOuter[52];
        colors[5][8] = primaryOuter[18];
        colors[5][5] = primaryOuter[15];
        colors[5][2] = primaryOuter[12];
        
        //now you have to new values ater turning the White CounterClokwise once
        System.out.println("BlueCounterClockWise");
        return colors;
    }
    public static int[][] YellowCW(int colors[][])
    {
        //First you have to change the face parts
        int[] primaryFace = new int[62];
        primaryFace[0] = colors[3][0];
        primaryFace[1] = colors[3][1];
        primaryFace[2] = colors[3][2];
        primaryFace[3] = colors[3][3];
        primaryFace[4] = colors[3][4];
        primaryFace[5] = colors[3][5];
        primaryFace[6] = colors[3][6];
        primaryFace[7] = colors[3][7];
        primaryFace[8] = colors[3][8];
        
        colors[3][2] = primaryFace[0];
        colors[3][5] = primaryFace[1];
        colors[3][8] = primaryFace[2];
        colors[3][1] = primaryFace[3];
        colors[3][4] = primaryFace[4];
        colors[3][7] = primaryFace[5];
        colors[3][0] = primaryFace[6];
        colors[3][3] = primaryFace[7];
        colors[3][6] = primaryFace[8];
        
        //Then you have to change the outer parts
        int[] primaryOuter = new int[62];
        primaryOuter[42] = colors[4][2];
        primaryOuter[41] = colors[4][1];
        primaryOuter[40] = colors[4][0];
        primaryOuter[00] = colors[0][0];
        primaryOuter[03] = colors[0][3];
        primaryOuter[06] = colors[0][6];
        primaryOuter[56] = colors[5][6];
        primaryOuter[57] = colors[5][7];
        primaryOuter[58] = colors[5][8];
        primaryOuter[28] = colors[2][8];
        primaryOuter[25] = colors[2][5];
        primaryOuter[22] = colors[2][2];
        
        colors[0][0] = primaryOuter[42];
        colors[0][3] = primaryOuter[41];
        colors[0][6] = primaryOuter[40];
        colors[5][6] = primaryOuter[00];
        colors[5][7] = primaryOuter[03];
        colors[5][8] = primaryOuter[06];
        colors[2][8] = primaryOuter[56];
        colors[2][5] = primaryOuter[57];
        colors[2][2] = primaryOuter[58];
        colors[4][2] = primaryOuter[28];
        colors[4][1] = primaryOuter[25];
        colors[4][0] = primaryOuter[22];
        
        //now you have to new values ater turning the White CounterClokwise once
        System.out.println("YellowClockWise");
        return colors;
    }
    public static int[][] YellowCCW(int colors[][])
    {
        //First you have to change the face parts
        int[] primaryFace = new int[62];
        primaryFace[0] = colors[3][0];
        primaryFace[1] = colors[3][1];
        primaryFace[2] = colors[3][2];
        primaryFace[3] = colors[3][3];
        primaryFace[4] = colors[3][4];
        primaryFace[5] = colors[3][5];
        primaryFace[6] = colors[3][6];
        primaryFace[7] = colors[3][7];
        primaryFace[8] = colors[3][8];
        
        colors[3][6] = primaryFace[0];
        colors[3][3] = primaryFace[1];
        colors[3][0] = primaryFace[2];
        colors[3][7] = primaryFace[3];
        colors[3][4] = primaryFace[4];
        colors[3][1] = primaryFace[5];
        colors[3][8] = primaryFace[6];
        colors[3][5] = primaryFace[7];
        colors[3][2] = primaryFace[8];
        
        //Then you have to change the outer parts
        int[] primaryOuter = new int[62];
        primaryOuter[42] = colors[4][2];
        primaryOuter[41] = colors[4][1];
        primaryOuter[40] = colors[4][0];
        primaryOuter[00] = colors[0][0];
        primaryOuter[03] = colors[0][3];
        primaryOuter[06] = colors[0][6];
        primaryOuter[56] = colors[5][6];
        primaryOuter[57] = colors[5][7];
        primaryOuter[58] = colors[5][8];
        primaryOuter[28] = colors[2][8];
        primaryOuter[25] = colors[2][5];
        primaryOuter[22] = colors[2][2];
        
        colors[2][8] = primaryOuter[42];
        colors[2][5] = primaryOuter[41];
        colors[2][2] = primaryOuter[40];
        colors[4][2] = primaryOuter[00];
        colors[4][1] = primaryOuter[03];
        colors[4][0] = primaryOuter[06];
        colors[0][0] = primaryOuter[56];
        colors[0][3] = primaryOuter[57];
        colors[0][6] = primaryOuter[58];
        colors[5][6] = primaryOuter[28];
        colors[5][7] = primaryOuter[25];
        colors[5][8] = primaryOuter[22];
        
        //now you have to new values ater turning the White CounterClokwise once
        System.out.println("YellowCounterClockWise");
        return colors;
    }
    public static int[][] OrangeCW(int colors[][])
    {
        //First you have to change the face parts
        int[] primaryFace = new int[62];
        primaryFace[0] = colors[4][0];
        primaryFace[1] = colors[4][1];
        primaryFace[2] = colors[4][2];
        primaryFace[3] = colors[4][3];
        primaryFace[4] = colors[4][4];
        primaryFace[5] = colors[4][5];
        primaryFace[6] = colors[4][6];
        primaryFace[7] = colors[4][7];
        primaryFace[8] = colors[4][8];
        
        colors[4][2] = primaryFace[0];
        colors[4][5] = primaryFace[1];
        colors[4][8] = primaryFace[2];
        colors[4][1] = primaryFace[3];
        colors[4][4] = primaryFace[4];
        colors[4][7] = primaryFace[5];
        colors[4][0] = primaryFace[6];
        colors[4][3] = primaryFace[7];
        colors[4][6] = primaryFace[8];
        
        //Then you have to change the outer parts
        int[] primaryOuter = new int[62];
        primaryOuter[32] = colors[3][2];
        primaryOuter[31] = colors[3][1];
        primaryOuter[30] = colors[3][0];
        primaryOuter[22] = colors[2][2];
        primaryOuter[21] = colors[2][1];
        primaryOuter[20] = colors[2][0];
        primaryOuter[12] = colors[1][2];
        primaryOuter[11] = colors[1][1];
        primaryOuter[10] = colors[1][0];
        primaryOuter[02] = colors[0][2];
        primaryOuter[01] = colors[0][1];
        primaryOuter[00] = colors[0][0];
        
        colors[2][2] = primaryOuter[32];
        colors[2][1] = primaryOuter[31];
        colors[2][0] = primaryOuter[30];
        colors[1][2] = primaryOuter[22];
        colors[1][1] = primaryOuter[21];
        colors[1][0] = primaryOuter[20];
        colors[0][2] = primaryOuter[12];
        colors[0][1] = primaryOuter[11];
        colors[0][0] = primaryOuter[10];
        colors[3][2] = primaryOuter[02];
        colors[3][1] = primaryOuter[01];
        colors[3][0] = primaryOuter[00];
        
        //now you have to new values ater turning the White CounterClokwise once
        System.out.println("OrangeClockWise");
        return colors;
    }
    public static int[][] OrangeCCW(int colors[][])
    {
        //First you have to change the face parts
        int[] primaryFace = new int[62];
        primaryFace[0] = colors[4][0];
        primaryFace[1] = colors[4][1];
        primaryFace[2] = colors[4][2];
        primaryFace[3] = colors[4][3];
        primaryFace[4] = colors[4][4];
        primaryFace[5] = colors[4][5];
        primaryFace[6] = colors[4][6];
        primaryFace[7] = colors[4][7];
        primaryFace[8] = colors[4][8];
        
        colors[4][6] = primaryFace[0];
        colors[4][3] = primaryFace[1];
        colors[4][0] = primaryFace[2];
        colors[4][7] = primaryFace[3];
        colors[4][4] = primaryFace[4];
        colors[4][1] = primaryFace[5];
        colors[4][8] = primaryFace[6];
        colors[4][5] = primaryFace[7];
        colors[4][2] = primaryFace[8];
        
        //Then you have to change the outer parts
        int[] primaryOuter = new int[62];
        primaryOuter[32] = colors[3][2];
        primaryOuter[31] = colors[3][1];
        primaryOuter[30] = colors[3][0];
        primaryOuter[22] = colors[2][2];
        primaryOuter[21] = colors[2][1];
        primaryOuter[20] = colors[2][0];
        primaryOuter[12] = colors[1][2];
        primaryOuter[11] = colors[1][1];
        primaryOuter[10] = colors[1][0];
        primaryOuter[02] = colors[0][2];
        primaryOuter[01] = colors[0][1];
        primaryOuter[00] = colors[0][0];
        
        colors[0][2] = primaryOuter[32];
        colors[0][1] = primaryOuter[31];
        colors[0][0] = primaryOuter[30];
        colors[3][2] = primaryOuter[22];
        colors[3][1] = primaryOuter[21];
        colors[3][0] = primaryOuter[20];
        colors[2][2] = primaryOuter[12];
        colors[2][1] = primaryOuter[11];
        colors[2][0] = primaryOuter[10];
        colors[1][2] = primaryOuter[02];
        colors[1][1] = primaryOuter[01];
        colors[1][0] = primaryOuter[00];
        
        //now you have to new values ater turning the White CounterClokwise once
        System.out.println("OrangeCounerClockWise");
        return colors;
    }
    public static int[][] RedCW(int colors[][])
    {
        //First you have to change the face parts
        int[] primaryFace = new int[62];
        primaryFace[0] = colors[5][0];
        primaryFace[1] = colors[5][1];
        primaryFace[2] = colors[5][2];
        primaryFace[3] = colors[5][3];
        primaryFace[4] = colors[5][4];
        primaryFace[5] = colors[5][5];
        primaryFace[6] = colors[5][6];
        primaryFace[7] = colors[5][7];
        primaryFace[8] = colors[5][8];
        
        colors[5][2] = primaryFace[0];
        colors[5][5] = primaryFace[1];
        colors[5][8] = primaryFace[2];
        colors[5][1] = primaryFace[3];
        colors[5][4] = primaryFace[4];
        colors[5][7] = primaryFace[5];
        colors[5][0] = primaryFace[6];
        colors[5][3] = primaryFace[7];
        colors[5][6] = primaryFace[8];
        
        //Then you have to change the outer parts
        int[] primaryOuter = new int[62];
        primaryOuter[16] = colors[1][6];
        primaryOuter[17] = colors[1][7];
        primaryOuter[18] = colors[1][8];
        primaryOuter[26] = colors[2][6];
        primaryOuter[27] = colors[2][7];
        primaryOuter[28] = colors[2][8];
        primaryOuter[36] = colors[3][6];
        primaryOuter[37] = colors[3][7];
        primaryOuter[38] = colors[3][8];
        primaryOuter[6] = colors[0][6];
        primaryOuter[7] = colors[0][7];
        primaryOuter[8] = colors[0][8];
        
        colors[2][6] = primaryOuter[16];
        colors[2][7] = primaryOuter[17];
        colors[2][8] = primaryOuter[18];
        colors[3][6] = primaryOuter[26];
        colors[3][7] = primaryOuter[27];
        colors[3][8] = primaryOuter[28];
        colors[0][6] = primaryOuter[36];
        colors[0][7] = primaryOuter[37];
        colors[0][8] = primaryOuter[38];
        colors[1][6] = primaryOuter[6];
        colors[1][7] = primaryOuter[7];
        colors[1][8] = primaryOuter[8];
        
        //now you have to new values ater turning the White CounterClokwise once
        System.out.println("RedClockWise");
        return colors;
    }
    public static int[][] RedCCW(int colors[][])
    {
        //First you have to change the face parts
        int[] primaryFace = new int[62];
        primaryFace[0] = colors[5][0];
        primaryFace[1] = colors[5][1];
        primaryFace[2] = colors[5][2];
        primaryFace[3] = colors[5][3];
        primaryFace[4] = colors[5][4];
        primaryFace[5] = colors[5][5];
        primaryFace[6] = colors[5][6];
        primaryFace[7] = colors[5][7];
        primaryFace[8] = colors[5][8];
        
        colors[5][6] = primaryFace[0];
        colors[5][3] = primaryFace[1];
        colors[5][0] = primaryFace[2];
        colors[5][7] = primaryFace[3];
        colors[5][4] = primaryFace[4];
        colors[5][1] = primaryFace[5];
        colors[5][8] = primaryFace[6];
        colors[5][5] = primaryFace[7];
        colors[5][2] = primaryFace[8];
        
        //Then you have to change the outer parts
        int[] primaryOuter = new int[62];
        primaryOuter[16] = colors[1][6];
        primaryOuter[17] = colors[1][7];
        primaryOuter[18] = colors[1][8];
        primaryOuter[26] = colors[2][6];
        primaryOuter[27] = colors[2][7];
        primaryOuter[28] = colors[2][8];
        primaryOuter[36] = colors[3][6];
        primaryOuter[37] = colors[3][7];
        primaryOuter[38] = colors[3][8];
        primaryOuter[6] = colors[0][6];
        primaryOuter[7] = colors[0][7];
        primaryOuter[8] = colors[0][8];
        
        colors[0][6] = primaryOuter[16];
        colors[0][7] = primaryOuter[17];
        colors[0][8] = primaryOuter[18];
        colors[1][6] = primaryOuter[26];
        colors[1][7] = primaryOuter[27];
        colors[1][8] = primaryOuter[28];
        colors[2][6] = primaryOuter[36];
        colors[2][7] = primaryOuter[37];
        colors[2][8] = primaryOuter[38];
        colors[3][6] = primaryOuter[6];
        colors[3][7] = primaryOuter[7];
        colors[3][8] = primaryOuter[8];
        
        //now you have to new values ater turning the White CounterClokwise once
        System.out.println("RedCounterClockWise");
        return colors;
    }
    public static int[][] getColors()//input is the colors that we get and it will be returned as so
    {
        int input[][] = new int[6][9];
        //initiate the counter
        int counter_0 = 0;
        int counter_1 = 0;
        //initiate the scanner for the imput
        Scanner in = new Scanner(System.in);
        //Collect the data
        
                //Get the data for the green side
                System.out.println("Enter the colors for the green side\n1 = GREEN\n2 = WHITE\n3 = BLUE\n4 = YELLOW\n5 = ORANGE\n6 = RED\n"
                        + "The orange side should be on top\n"
                        + "The red side is on the bottom\n"
                        + "The white side to the right\n"
                        + "And the yellow side to the left");
                while(counter_0 <= 8)
                { 
                        //System.out.println(counter_0);
                        counter_1 = 0;
                        input[0][counter_0] = Integer.parseInt(in.nextLine());
                        counter_0++;
                }
                //Enter the colors for the white side
                System.out.println("Enter the colors for the white side\n1 = GREEN\n2 = WHITE\n3 = BLUE\n4 = YELLOW\n5 = ORANGE\n6 = RED\n"
                        + "The orange side should be on top\n"
                        + "The red side should be on the bottom\n"
                        + "The blue side to the right\n"
                        + "And the green side to the left");
                //reset the counter
                counter_0 = 0;
                while(counter_0 <= 8)
                {
                        counter_1 = 0;
                        input[1][counter_0] = Integer.parseInt(in.nextLine());
                        counter_0++;
                }
                //Enter the colors for the blue side
                System.out.println("Enter the colors for the blue side\n1 = GREEN\n2 = WHITE\n3 = BLUE\n4 = YELLOW\n5 = ORANGE\n6 = RED\n"
                        + "Orange on top\n"
                        + "red on bottom\n"
                        + "yellow to the right\n"
                        + "And Red on bottom");
                counter_0 = 0;
                while(counter_0 <= 8)
                {
                        counter_1 = 0;
                        input[2][counter_0] = Integer.parseInt(in.nextLine());
                        counter_0++;
                }
              //Enter the colors for the yellow side
                System.out.println("Enter the colors for the yellow side\n1 = GREEN\n2 = WHITE\n3 = BLUE\n4 = YELLOW\n5 = ORANGE\n6 = RED\n"
                        + "Orange on top\n"
                        + "red on bottom\n"
                        + "green to right\n"
                        + "blue to left");
                counter_0 = 0;
                while(counter_0 <= 8)
                {
                        counter_1 = 0;
                        input[3][counter_0] = Integer.parseInt(in.nextLine());
                        counter_0++;
                }
              //Enter the colors for the orange side
                System.out.println("Enter the colors for the orange side\n1 = GREEN\n2 = WHITE\n3 = BLUE\n4 = YELLOW\n5 = ORANGE\n6 = RED");
                counter_0 = 0;
                while(counter_0 <= 8)
                {
                        counter_1 = 0;
                        input[4][counter_0] = Integer.parseInt(in.nextLine());
                        counter_0++;
                }
              //Enter the colors for the red side
                System.out.println("Enter the colors for the red side\n1 = GREEN\n2 = WHITE\n3 = BLUE\n4 = YELLOW\n5 = ORANGE\n6 = RED");
                counter_0 = 0;
                while(counter_0 <= 8)
                {
                        counter_1 = 0;
                        input[5][counter_0] = Integer.parseInt(in.nextLine());
                        counter_0++;
                }
        return input;
    }
    
//</editor-fold>
}