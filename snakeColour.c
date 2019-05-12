#include<curses.h>
#include<time.h>
#include<stdlib.h>
#define TRUE 1
#define FALSE 0
#define UP 3
#define DOWN 2
#define RIGHT 5
#define LEFT 4
#define LEN 24
#define SNAKE_PAIR 6
#define APPLE_PAIR 7
#define DEAD_PAIR 8
#define BLUE_PAIR 9


void fillBounds(char bounds[][LEN],int logicbounds[][LEN], int logicDirec[][LEN]);


int main(){
	
	char bounds[LEN][LEN];
	int logicbounds[LEN][LEN]={0};
	//tracks  where snakes direction is changed. no change:0 ,up:1, down:2, left:3, right:4
	int logicDirec[LEN][LEN];

	// activate the keypad to read from this terminal
  	keypad(initscr(),TRUE);
 	curs_set(0);// don't show cursor

 	cbreak();     // don't stop for getch
 	timeout(250);     // repeat whileloop on timer
 	noecho();//typed letters do  not appear of screen

 	// initialise the x and y positions of the cursor
 	int xPos=2;
  	int yPos=2;
  	char c='\0'; 

  	int xspace=2;//initialise x and y positions for trailing space
  	int yspace=2;
  	int firstround=-1;//will indicate the first few loops to get timing of trailing space right
  	
  	int xapple,yapple,applespace;

 	int direction, spacedirec;
 	int isapple=0;//logcal for if there is an apple currently printed
 	int body=1;//indicates whether continued body needs  to be  printed  for  a particular loop
 	int pntcheck=0;//creates condition for apple loop so apple is created in free space

 	int score=0;

 	srand(time(NULL)); // initialise the random number generator

 	start_color();//initialise colour pairs
 	init_pair(SNAKE_PAIR, COLOR_GREEN, '\0');
 	init_pair(APPLE_PAIR, COLOR_RED, '\0');
 	init_pair(DEAD_PAIR, COLOR_RED, COLOR_GREEN);
 	init_pair(BLUE_PAIR, COLOR_BLUE,'\0');
 	clear();

  	fillBounds(bounds, logicbounds, logicDirec);//draw boundrys and fill logic arrays

  	mvaddch(yPos,xPos,'o');
  	
  	//loop repeats on timer
  	while(c!='q'){
		//randomly generate 'food' for snake to each. isapple makes sure only one 
  		//exists at a time and pntcheck ensures there is nothing at the genereated spot
  		if (isapple==0){
  			while(pntcheck==0){
  				yapple=rand()%(LEN-2)+1;//generate random position
  				xapple=rand()%(LEN-2)+1;

  				if(logicbounds[yapple][xapple]==0){
  					attron(COLOR_PAIR(APPLE_PAIR));//enable colour
  					mvaddch(yapple,xapple,'@');//print apple
  					attroff(COLOR_PAIR(APPLE_PAIR));

  					logicbounds[yapple][xapple]=2;//2 will represent 'food' in logicbounds 
  					pntcheck=1;//point check success
  				}
  			}
  			pntcheck=0;	//reset pntcheck
  			isapple=1; //there is an apple
  		}


		c=getch();
		//changes direction of snake
		switch(c){
			case UP:
			if(direction!=2){	//if the snake isn't going the  oposite  direction		
				logicDirec[yPos][xPos]=1;//set turning point in direction array
				yPos--;
				direction=1;
				body=0;	//tells continuous movement switch statement to not do  anything
			}		
			break;

			case DOWN:	
			if(direction!=1){		
				logicDirec[yPos][xPos]=2;
				yPos++;
				direction=2;
				body=0;
			}			
			break;

			case LEFT:	
			if(direction!=4){		
				logicDirec[yPos][xPos]=3;
				xPos--;
				direction=3;
				body=0;	
			}		
			break;

			case RIGHT:	
			if(direction!=3){		
				logicDirec[yPos][xPos]=4;
				xPos++;
				direction=4;
				body=0;	
			}		
			break;
		}//continues in specifed direction if direction has not just been changed
		if(body==1){
			switch(direction){
				case 1:
					yPos--;
				break;
				case 2:
					yPos++;
				break;
				case 3:				
					xPos--;				
				break;
				case 4:				
					xPos++;				
				break;
			}
		}
		//checking if the new position is already occupied (snake or barrier)
		if((logicbounds[yPos][xPos]==1) && (firstround==0)){
			//go back one place and turn 'head' into an x to depict dead snake
			switch(direction){
				case 1:
					yPos++;
				break;
				case 2:
					yPos--;
				break;
				case 3:				
					xPos++;				
				break;
				case 4:				
					xPos--;				
				break;
			}
			attron(COLOR_PAIR(DEAD_PAIR));
			mvaddch(yPos,xPos,'x');
			attroff(COLOR_PAIR(DEAD_PAIR));
			break;//exit loop and end game
		}
		//checking if the new position contains an apple
		if(logicbounds[yPos][xPos]==2){
			applespace=1;//indicates an apple has been eaten
			isapple=0;//indicates no apples
			score=score+1;
		}

		//print snake 
		attron(COLOR_PAIR(SNAKE_PAIR));
		mvaddch(yPos,xPos,'o');
		attroff(COLOR_PAIR(SNAKE_PAIR));
		logicbounds[yPos][xPos]=1;//update logic array to contain new snake position
		
		//this section keeps the snake at a certain length by adding a space at the end of the  snake
		if (firstround==1){//if this is the  first round the snake moves by itself
			spacedirec=direction;//direction of space is direction of snake
			mvaddch(yspace,xspace,' ');

			logicDirec[yspace][xspace]=0;//set logic array positions to zero
			logicbounds[yspace][xspace]=0;

			firstround=0;//indicate first round  is over
		}
		if(applespace==1){
			applespace=0;//do nothing if an apple was eaten so the snake can grow by one

		}else{
			//move space coordiantes based on space direction
			if (firstround==0){
				switch(spacedirec){
					case 1:
					if (yspace-1!=0){
						yspace--;
					}
					break;
					case 2:
					if (yspace+1!=LEN-1){
						yspace++;
					}
					break;
					case 3:
					if (xspace-1!=0){
						xspace--;
					}
					break;
					case 4:
					if (xspace+1!=LEN-1){
						xspace++;
					}
					break;
				}
				mvaddch(yspace,xspace,' ');
				logicbounds[yspace][xspace]=0;
			}
		}

		//check whether the space is where the snake turns (this is recorded in logicDirec) and 
		//change space direction accordingly
		if (logicDirec[yspace][xspace]!=0){
			spacedirec=logicDirec[yspace][xspace];
			logicDirec[yspace][xspace]=0;
		}

		//print game message and score (apples eaten)
		mvprintw(3,LEN+1,"Snake");
		mvprintw(4,LEN+1,"-Press arrow keys to start moving");
		mvprintw(5,LEN+1,"-Eat apples to get longer");
		mvprintw(6,LEN+1,"-Don't bump into walls or youself or snek die :`(");
		mvprintw(7,LEN+1,"-Press q to quit");
		attron(COLOR_PAIR(BLUE_PAIR));
		mvprintw(9,LEN+1,"Score: %d",score);
		attroff(COLOR_PAIR(BLUE_PAIR));
		
		if ((firstround==-1) && (body==0)){//initiates 'first round' when arrow key is pressed to start movement
			firstround=1;
		}
		body=1;//reset body
	}
	
	while(c!='q'){//display message when game over
		int xdisp=LEN/2;
		int ydisp=LEN/2-2;
		attron(COLOR_PAIR(APPLE_PAIR));
		mvprintw(ydisp,xdisp-5,"Game Over!");
		mvprintw(ydisp+1,xdisp-7,"press q to quit");
		attroff(COLOR_PAIR(APPLE_PAIR));
		c=getch();
	}

	endwin();//reset terminal
	return 0;
}





void fillBounds(char bounds[][LEN],int logicbounds[][LEN], int logicDirec[][LEN]){
	//prints boundry and sets logical array logicbounds
  int row,col;
 
  //fills boundry array with boundry andlogical array with 1 for boundry symbol and 0 for space
  for (row=0;row<LEN;row++){
  	for (col=0;col<LEN;col++){
  		logicDirec[row][col]=0;//fill direction change array

  		if((row==0) ||  (row==LEN-1)){//fill barrior and barior indicator arrays
  			bounds[row][col]='-';
  			logicbounds[row][col]=1;

  		}else if((col==0) || (col==LEN-1)){
  			bounds[row][col]='|';
  			logicbounds[row][col]=1;
  		}else{
  			bounds[row][col]=' ';
  			logicbounds[row][col]=0;
  		}
  	}
  }

  for(row=0;row<LEN;row++){//prints boundry
    for(col=0;col<LEN;col++){
      mvaddch(row,col,bounds[row][col]);
    }
   }
}