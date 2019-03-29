#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <cstdio>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <queue>
#include <utility>

using namespace std;

#define blockgap 16

int startingx = 50, startingy = 50;

class timer
{
	private:
		unsigned long begTime;
	public:
		void start()
		{
			begTime = clock();
		}

		unsigned long elapsedTime()
		{
			return ((unsigned long) clock() - begTime) / CLOCKS_PER_SEC;
		}

		bool isTimeout(unsigned long seconds)
		{
			return seconds >= elapsedTime();
		}

};

class mazeSolver
{
    int rowsize, colsize;
    int startx, starty, endx, endy;
    int xcircle, ycircle;
    int no_of_steps;
    int min_steps;
    int timelimit;
    timer t;
    int **maze;
    int **visited;
    void background();
    void displaySolved(string );
    void displayWin();
    void displayLost();
    void BFS();
    int calcScore();
    bool solvable(int , int , string );
    bool isSafe(int , int );
public:
    void initialize()
    {
        min_steps = 0;
        no_of_steps = 0;
        xcircle = startingx + blockgap*(starty-1) + blockgap/2;
        ycircle = startingy + blockgap*(startx-1) + blockgap/2;
    }
    void initializeEasyMaze();
    void initializeMediumMaze();
    void initializeHardMaze();
    void play();

};

void mazeSolver :: initializeEasyMaze()
{

    startingx = 200;
    startingy = 200;

                    // Opening the file pointer according to the difficulty level
    ifstream fin("easy.txt");
                    // Reading the size of the matrix from the file
    fin>>rowsize>>colsize;
    fin>>startx>>starty>>endx>>endy;
                    // Declaring the matrix of specified size
    maze = new int*[rowsize];
    visited = new int*[rowsize];
    for(int i = 0; i<rowsize;i ++)
    {
        maze[i] = new int[colsize];
        visited[i] = new int[colsize];
    }
                    // Reading the maze from the file
    for(int i = 0; i<rowsize; i++)
    {
        for(int j = 0; j<colsize; j++)
        {
            fin>>maze[i][j];
        }

    }
                    // Closing the file pointer
    fin.close();

    timelimit = 30;
}

void mazeSolver :: initializeMediumMaze()
{
    startingx = 100;
    startingy = 100;
                    // Opening the file pointer according to the difficulty level
    ifstream fin("medium.txt");
                    // Reading the size of the matrix from the file
    fin>>rowsize>>colsize;
    fin>>startx>>starty>>endx>>endy;
                    // Declaring the matrix of specified size
    maze = new int*[rowsize];
    visited = new int*[rowsize];
    for(int i = 0; i<rowsize;i ++)
    {
        maze[i] = new int[colsize];
        visited[i] = new int[colsize];
    }
                    // Reading the maze from the file
    for(int i = 0; i<rowsize; i++)
    {
        for(int j = 0; j<colsize; j++)
        {
            fin>>maze[i][j];
        }

    }

                    // Closing the file pointer
    fin.close();

    timelimit = 60;
}

void mazeSolver :: initializeHardMaze()
{
    startingx = 50;
    startingy = 50;

                    // Opening the file pointer according to the difficulty level
    ifstream fin("hard.txt");
                    // Reading the size of the matrix from the file
    fin>>rowsize>>colsize;
    fin>>startx>>starty>>endx>>endy;
                    // Declaring the matrix of specified size
    maze = new int*[rowsize];
    visited = new int*[rowsize];
    for(int i = 0; i<rowsize;i ++)
    {
        maze[i] = new int[colsize];
        visited[i] = new int[colsize];
    }
                    // Reading the maze from the file
    for(int i = 0; i<rowsize; i++)
    {
        for(int j = 0; j<colsize; j++)
        {
            fin>>maze[i][j];
        }

    }

                    // Closing the file pointer
    fin.close();

    timelimit = 90;
}

void mazeSolver :: background()
{

    setfillstyle(SOLID_FILL,LIGHTBLUE);
    floodfill(1,1,WHITE);
    setfillstyle(SOLID_FILL,WHITE);

    int growsize = startingx + blockgap*(rowsize);
    int gcolsize = startingy + blockgap*(colsize);

    rectangle(startingx - blockgap,startingy - blockgap,gcolsize + blockgap,growsize + blockgap);
    setfillstyle(SOLID_FILL,BLACK);
    floodfill(startingx - blockgap/2,startingy - blockgap/2,WHITE);
        // Set Default Again
    setfillstyle(SOLID_FILL,WHITE);

    for(int i=startingx;i<=gcolsize;i+=blockgap)
    {
            line(i,startingy,i,growsize);
    }
    for(int i=startingy;i<=growsize;i+=blockgap)
    {
            line(startingx,i,gcolsize,i);
    }

    for(int i = 0; i<rowsize; i++)
    {
        for(int j = 0; j<colsize; j++)
        {
            if(maze[i][j] == 1)
            {
                floodfill(startingx + blockgap/2 + blockgap*j,startingy + blockgap/2 + blockgap*i,WHITE);
            }

        }
    }

}

void mazeSolver :: displaySolved(string direc)
{


            // Changing the color of the previous circle white
    floodfill(xcircle,ycircle,BLACK);


    if(direc == "UP")
    {
        ycircle -= blockgap;
    }
    else if(direc == "DOWN")
    {
        ycircle += blockgap;
    }
    else if(direc == "LEFT")
    {
        xcircle -= blockgap;
    }
    else if(direc == "RIGHT")
    {
        xcircle += blockgap;
    }

    setcolor(BLACK);
    circle(xcircle,ycircle,blockgap/4);
    setfillstyle(SOLID_FILL,BLUE);
    floodfill(xcircle,ycircle,BLACK);
    setfillstyle(SOLID_FILL,BLUE);
    setcolor(WHITE);

            // For displaying time elapsed.

    rectangle(1000,100,1250,250);
    setfillstyle(SOLID_FILL,BLACK);
    floodfill(1001,101,WHITE);
    setfillstyle(SOLID_FILL,WHITE);
    stringstream ss, ss1;
    ss<<t.elapsedTime();
    ss1<<timelimit;
    int elaptime, totaltime;
    string remainingtime;
    ss>>elaptime;
    ss1>>totaltime;
    int temp = totaltime - elaptime;
    if(temp < 0)
        temp = 0;
    stringstream ss2;
    ss2<<temp;
    ss2>>remainingtime;
    remainingtime += "  Seconds";
    char *printremainingtime;
    printremainingtime = (char *)remainingtime.c_str();
    setcolor(WHITE);
    settextstyle(5,0,3);
    outtextxy(1030,130,"Remaining Time : ");
    settextstyle(3,0,1);
    outtextxy(1060,180,printremainingtime);
    settextstyle(3,0,1);
}

void mazeSolver :: displayWin()
{
    cleardevice();
    background();

    PlaySound("C:\\Users\\Dhruv Mittal\\Desktop\\DS Project\\win.wav",NULL,SND_ASYNC);

            // Display part
    setbkcolor(LIGHTGREEN);

    int left = 950, right = 1250, top = 150, bottom = 510;
    for(int i = 0; i<3; i++, left--, right++, top--, bottom++)
        rectangle(left,top,right,bottom);
    setfillstyle(SOLID_FILL,LIGHTGREEN);
    floodfill(951,151,WHITE);
    setfillstyle(SOLID_FILL,WHITE);

    int colo = 3;

    for(int i = 0; i<3; i++)
    {
        setcolor(colo);
        settextstyle(0,0,8);            // text style, direction, size
        outtextxy(1000,220,"YOU");
        outtextxy(1000,280,"WIN");
        if(colo == 3)
        {
            colo = 9;
        }
        else
        {
            colo = 3;
        }

        delay(500);
    }

    BFS();

    setbkcolor(LIGHTGREEN);

    stringstream ss;
    ss<<calcScore();
    string score;
    ss>>score;
    char *printscore;
    printscore = (char *)score.c_str();

    setcolor(3);
    settextstyle(0,0,3);
    outtextxy(970,400,"Score: ");
    outtextxy(1130,400,printscore);
    settextstyle(3,0,1);

    setcolor(WHITE);
    setbkcolor(BLACK);
    delay(5000);
}

void mazeSolver :: displayLost()
{
    cleardevice();
    background();

    PlaySound("C:\\Users\\Dhruv Mittal\\Desktop\\DS Project\\lose.wav",NULL,SND_ASYNC);

            // Display part
    setbkcolor(LIGHTGREEN);

    int left=950,right=1250,top=150,bottom=510;
    for(int i=0;i<3;i++,left--,right++,top--,bottom++)
        rectangle(left,top,right,bottom);
    setfillstyle(SOLID_FILL,LIGHTGREEN);
    floodfill(970,210,WHITE);
    setfillstyle(SOLID_FILL,WHITE);

    int colo = 3;
    for(int i = 0; i<3; i++)
    {
        setcolor(colo);
        settextstyle(0,0,8);            // text style, direction, size
        outtextxy(970,220,"GAME");
        outtextxy(970,280,"OVER");
        settextstyle(0,0,3);
        outtextxy(1000,390,"Score: 0");
        settextstyle(0,0,2);
        if(t.elapsedTime() > timelimit)
        {
            outtextxy(1020,450,"Time Out !!!");
        }
        settextstyle(3,0,1);
        if(colo == 3)
        {
            colo = 9;           //lightblue
        }
        else
        {
            colo = 3;           //cyan
        }

        delay(500);
    }

    BFS();

    setcolor(WHITE);
    setbkcolor(BLACK);
    delay(5000);
}


void mazeSolver :: BFS(){


    queue< pair<int, int> > q;

    pair<int , int > previous[rowsize][colsize];

        // Initializing the visited matrix, and the previous matrix
    for(int i = 0; i<rowsize; i++)
    {
        for(int j = 0; j<colsize; j++)
        {
            visited[i][j] = 0;
            previous[i][j].first = -1;
            previous[i][j].second = -1;
        }

    }

                                //      BFS Starts

        //  Pushing the start point into the queue
    q.push(make_pair(startx-1, starty-1));
    visited[startx - 1][starty - 1] = 1;

    pair<int , int > curpos;

    while(!q.empty()){

            //  Popping the first element of the queue
        curpos = q.front();
        q.pop();

        int x = curpos.first, y = curpos.second;


            //  Down
        if( isSafe(x + 1, y) && !visited[x + 1][y] ){
            visited[x + 1][y] = 1;
            q.push(make_pair(x + 1, y));

            previous[x + 1][y].first = x;
            previous[x + 1][y].second = y;
        }

            //  Up
        if( isSafe(x - 1, y) && !visited[x - 1][y] ){
            visited[x - 1][y] = 1;
            q.push(make_pair(x - 1, y));

            previous[x - 1][y].first = x;
            previous[x - 1][y].second = y;
        }

            //  Left
        if( isSafe(x, y - 1) && !visited[x][y - 1] ){
            visited[x][y - 1] = 1;
            q.push(make_pair(x, y - 1));

            previous[x][y - 1].first = x;
            previous[x][y - 1].second = y;
        }

            //  Right
        if( isSafe(x, y + 1) && !visited[x][y + 1] ){
            visited[x][y + 1] = 1;
            q.push(make_pair(x, y + 1));

            previous[x][y + 1].first = x;
            previous[x][y + 1].second = y;
        }


        if(visited[endx-1][endy-1])
            break;
    }

    vector< pair<int , int > > path;

    if(visited[endx - 1][endy - 1])
    {
        pair<int , int > current;
        current.first=endx - 1;
        current.second=endy - 1;

        while( (previous[current.first][current.second].first != startx - 1) || (previous[current.first][current.second].second != starty - 1) )
        {
            path.push_back(current);
            int x = current.first;
            int y = current.second;
            current.first = previous[x][y].first;
            current.second = previous[x][y].second;

        }

        path.push_back(current);
        path.push_back(make_pair(startx - 1, starty - 1));
    }

        //  BFS Ends
    setbkcolor(LIGHTBLUE);
    setcolor(YELLOW);
    settextstyle(0,0,3);
    outtextxy(startingx - 20, 20, "BFS is calculating the shortest path....");
    settextstyle(3,0,1);
    setcolor(WHITE);

        //  Displaying the path
    if(visited[endx - 1][endy - 1])
    {
        setcolor(BLACK);
        min_steps = path.size() - 1;
        for(int i = path.size() - 1; i >= 0; i--)
        {
            xcircle = startingx + blockgap*(path[i].second) + blockgap/2;
            ycircle = startingy + blockgap*(path[i].first) + blockgap/2;

            setcolor(BLACK);
            setfillstyle(SOLID_FILL,BLUE);
            circle(xcircle, ycircle, blockgap/4);
            floodfill(xcircle, ycircle, BLACK);
            setfillstyle(SOLID_FILL,WHITE);

            if(i != 0){
                delay(100);
                floodfill(xcircle, ycircle, BLACK);
            }

        }
        setcolor(WHITE);

    }

    stringstream ss, ss1;
    ss<<min_steps;
    ss1<<no_of_steps;
    string mini, finalmini, orig, finalorig;
    ss>>mini;
    ss1>>orig;
    finalmini = "Minimum Steps : " + mini;
    finalorig = "Your Steps : " + orig;
    char *minimum = (char *)finalmini.c_str();
    char *your = (char *)finalorig.c_str();


    setcolor(YELLOW);
    settextstyle(0,0,3);
    outtextxy(startingx - 20, 630, minimum);
    outtextxy(startingx - 20, 660, your);
    settextstyle(3,0,1);
    setcolor(WHITE);
    setbkcolor(BLACK);

}

int mazeSolver :: calcScore()
{
    int max_score = min_steps * 100;
    max_score -= (no_of_steps - min_steps)*30;
    return max_score;
}

void mazeSolver :: play()
{

    t.start();          // Clock Started.

    background();

    if(solvable(startx-1,starty-1,""))
    {
        displayWin();
    }
    else
    {
        displayLost();
    }

}

bool mazeSolver :: solvable(int row, int col, string direc)
{
    if(row == endx-1 && col == endy-1)
    {
        displaySolved(direc);
        return true;
    }
    if(isSafe(row,col))
    {
        PlaySound("C:\\Users\\Dhruv Mittal\\Desktop\\DS Project\\step.wav",NULL,SND_ASYNC);
        no_of_steps++;

                // Delay for the circle to move forward slowly
        delay(150);

        displaySolved(direc);
        while(1)
        {

            if(t.elapsedTime() > timelimit)
            {
                return false;
            }

            if( GetAsyncKeyState(VK_UP) && solvable(row-1,col,"UP"))
            {
                return true;
            }

            if(t.elapsedTime() > timelimit)
            {
                return false;
            }

            if( GetAsyncKeyState(VK_LEFT) && solvable(row,col-1,"LEFT"))
            {
                 return true;
            }

            if(t.elapsedTime() > timelimit)
            {
                return false;
            }

            if( GetAsyncKeyState(VK_DOWN) && solvable(row+1,col,"DOWN"))
            {
                return true;
            }

            if(t.elapsedTime() > timelimit)
            {
                return false;
            }

            if( GetAsyncKeyState(VK_RIGHT) && solvable(row,col+1,"RIGHT"))
            {
                return true;
            }

            if(t.elapsedTime() > timelimit)
            {
                return false;
            }

            displaySolved("");

        }

    }

    return false;
}

bool mazeSolver :: isSafe(int row, int column)
{
    if(row>=0 && row<rowsize && column>=0 && column<colsize && maze[row][column] == 1)
    {
        return true;
    }

    return false;
}


int main()
{
    int gmode, gdriver = DETECT;    //requests auto detection.
    initgraph(&gdriver,&gmode,"");  //initializes graphic modes.//""-is pathtodriver.
    initwindow(1300,700);
    cleardevice();
    while(true)
    {
        mazeSolver MS;
        cleardevice();
        setbkcolor(LIGHTBLUE);
        setfillstyle(SOLID_FILL,BLUE);
        floodfill(1,1,WHITE);
        setcolor(LIGHTBLUE);
        arc(400,200,90,180,40);
        line(360,200,360,500);
        arc(400,500,180,270,40);
        line(400,540,900,540);
        arc(900,500,270,360,40);
        line(940,500,940,200);
        arc(900,200,0,90,40);
        line(900,160,400,160);
        setcolor(WHITE);
        setfillstyle(SOLID_FILL,LIGHTBLUE);
        floodfill(401,201,LIGHTBLUE);
        setfillstyle(SOLID_FILL,WHITE);
        settextstyle(0,0,4);            // text style, direction, size
        outtextxy(430,220,"THE MAZERUNNER");
        settextstyle(0,0,2);
        outtextxy(500,250,"-x-x-x-x-x-x-x-x-x-x-");
        outtextxy(430,300,"Press <RETURN> to choose level :");
        ellipse(500,370,0,360,100,30);
        outtextxy(450,360,"1. Easy ");
        ellipse(800,370,0,360,100,30);
        outtextxy(730,360,"2. Medium ");
        ellipse(650,450,0,360,100,30);
        outtextxy(600,440,"3. Hard ");

        line(0,670,1300,670);
        setfillstyle(SOLID_FILL,7);
        floodfill(0,671,WHITE);
        setfillstyle(SOLID_FILL,WHITE);
        setcolor(BLUE);
        setbkcolor(7);
        settextstyle(4,0,1);
        outtextxy(20,675,"TEAM MEMBERS :       Dhruv Mittal       Sarthak Rana        Deepak Mathuria     Harsh Gupta");
        setbkcolor(BLACK);
        setcolor(WHITE);

        int col = 7;
        while(true){

            if(GetAsyncKeyState(VK_ESCAPE))
                exit(0);
            else if(GetAsyncKeyState(VK_RETURN))
                break;
            else{
                setbkcolor(BLUE);
                setcolor(col);
                outtextxy(500,600,"press <ESC> to exit.");
                if(col == 7)
                    col = 1;
                else
                    col = 7;
            }
            delay(200);
        }
        setcolor(WHITE);
        int choice;
        cin>>choice;
        switch(choice)
        {
            case 1 :    MS.initializeEasyMaze();
                        break;

            case 2 :    MS.initializeMediumMaze();
                        break;

            case 3 :    MS.initializeHardMaze();
                        break;

            default :   exit(0);

        }

        cin.ignore();
        setcolor(WHITE);
        cleardevice();
        MS.initialize();
        setbkcolor(BLACK);
        MS.play();
    }

    closegraph();
    return 0;
}
