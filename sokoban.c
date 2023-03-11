#include <stdio.h>
#include <string.h>

#define MAP_WIDTH 14
#define MAP_HEIGHT 10
#define PLAYER_POSITION pos_y * MAP_WIDTH + pos_x

char map[] = {
    "##############\n"
    "#      # xB  #\n"
    "#  x   # xB  #\n"
    "#   B  # xB  #\n"
    "#      ####  #\n"
    "#     @   #  #\n"
    "#         #  #\n"
    "#    B    #  #\n"
    "#     x      #\n"
    "##############\n"
};

int dest_squares[10];                                              // collecting cell indexes for 'x' cells [that remain to won]


int GetDestSquares()                                               // init 'x' cells indexes [that we will be collecting]
{
    int count, cell;                                               // 'x' cell number, current cell index

    for(int row = 0; row < MAP_HEIGHT; row++)                      // loop over map rows
    {
        for(int col = 0; col < MAP_WIDTH; col++)                   // loop over map columns
        {
            cell = row * MAP_WIDTH + col;                          // current cell index
        
            if(map[cell] == 'x' || map[cell] == 'O')               // if 'x' cell is not occupied or with box on it
                dest_squares[count++] = cell;                      // colect it in  dest_squares[] array
        }
    }

    return count - 1;                                              // return number of 'x' cells [to collect]

}


void GetPosition(int *pos_x, int *pos_y)                           // function that gets our current pos
{
    int cell;                                                      // current cell index

    for(int row = 0; row < MAP_HEIGHT; row++)                      // loop over map rows
    {
        for(int col = 0; col < MAP_WIDTH; col++)                   // loop over map columns
        {
            cell = row * MAP_WIDTH + col;                          // init current cell index
        
            if(map[cell] == '@')                                   // if current cell == player
            {
                *pos_x = col;                                      // store our's x coordinate as cell 
                *pos_y = row;                                      // store our's y coordinate as cell
            }
        }
    }
}


void MoveCharacter(int pos_x, int pos_y, int offset)               // moving our player with x and y coords
{
    if(map[PLAYER_POSITION + offset] != '#')                       // if player doesn't hit the wall (#)
    {
        if(((map[PLAYER_POSITION + offset] == 'B') ||              // if player hits the box         (B)
            (map[PLAYER_POSITION + offset] == 'O')) &&             // or the box on 'x' cell         (0)
            (map[PLAYER_POSITION + offset * 2] != '#' ||                // and box doesn't hit a wall(B -> #)
             map[PLAYER_POSITION + offset * 2] != 'B' ||                // or another box            (B -> B)
             map[PLAYER_POSITION + offset * 2] != 'O'))                 // or box on 'x' cell        (@ -> 0)
        {
            map[PLAYER_POSITION] = ' ';                                 // clear previous player's position
            pos_x += offset;                                            // update player's coordinate

            if(map[PLAYER_POSITION + offset] == ' ')                    // if the square next to the box is empty
                map[PLAYER_POSITION + offset] = 'B';                    // push the box (@ -> B)

            else if(map[PLAYER_POSITION + offset] == 'x')               // if the square next to the box is 'x'  (B -> X)
                map[PLAYER_POSITION + offset] = 'O';                    // mark the box is on it's place (X -> 0)

            else
            {
                map[PLAYER_POSITION - offset] = '@';                    // if box hits the wall or another box
                return;                                                 // don't push it any further
            }

            map[PLAYER_POSITION] = '@';                                 // draw the player in the new position
        }

        else                                                            // if the square next to the player is empty                                                        
        {
            map[PLAYER_POSITION] = ' ';                                 // clear previous player position
            pos_x += offset;                                            // update player's coordinate
            map[PLAYER_POSITION] = '@';                                 // draw the player in the new position
        }
    }   
}

int main()
{


char text[] =
{
"  #####   #####   #  #   #####   #####     #     ##   #\n"
" ##       #   #   # #    #   #   #    #   # #    # #  #\n"
"   ##     #   #   ##     #   #   #####   #####   # ## #\n"
"    ##    #   #   # #    #   #   #    #  #   #   #  # #\n"
"#####     #####   #  #   #####   #####   #   #   #   ##\n"   
"\n"                 
};

printf("%s", text);

    int key;                                                       // input key [leave/moving]
    int pos_x, pos_y;                                              //  player's coordinates
    int dest_count;                                                //  'x' cells counter

    int dest_num = GetDestSquares();                               // get number of 'x' cells, which we want to "collect" by
                                                                   // putting B boxes on them

    printf("%s\n", map);                                           // print map

    while(key != 27)                                               // game loop
    {  
        GetPosition(&pos_x, &pos_y);                               // get player's coordinates
        
        scanf(" %c", &key);                                         // get user input [ESC or WSAD]

        switch(key)                                           
        {
            // move character up
            case 'w':
                MoveCharacter(pos_x, pos_y, - MAP_WIDTH - 1);
                break;

            // move character down
            case 's':
                MoveCharacter(pos_x, pos_y, MAP_WIDTH + 1);
                break; 

            // move character left
            case 'a':
                MoveCharacter(pos_x, pos_y, -1);
                break; 

            // move character right
            case 'd':
                MoveCharacter(pos_x, pos_y, 1);
                break; 

        }

        dest_count = 0;                                            // reset 'x' cells counter

        // for every collected X square

        for(int i = 0; i < 10; i++)                                // for all destination squares
        {            
            if(map[dest_squares[i]] == 'O') dest_count++;          // increase 'x' cells counter if box is on 'x' cell
        
            if(map[dest_squares[i]] == ' ')                        // if 'x' cell has been erased
                map[dest_squares[i]] = 'x';                        // restore it
        }


        // print map after every move
        printf("%s\n", map);                                       


        // if all boxes are on X places, automaticly press key="27" (ESC -> breaks the loop)
        if(dest_num == dest_count)
        {
            printf("!!!!! You won !!!!!\n");
            key = 27;
        }        
    }
        
    return 0;

}