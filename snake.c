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


void fillBounds(char bounds[][LEN],int logicbounds[][LEN], int logicDirec[][LEN]);


int main(){
	
	char bounds[LEN][LEN];
	int logicbounds[LEN][LEN]={0};
	//tracks  where snakes direction is changed. no change:0 ,up:1, down:2, left:3, right:4
	int logicDirec[LEN][LEN];

	// activate the keypad to read from this terminal
  	keypad(initscr(),TRUE);
  	// don't show cursor
 	curs_set(0);
 	cbreak();         // don't stop for getch
 	timeout(250);     // wait 500ms for key press then move on
 	noecho();//typed letters do  not appear of screen

 	// initialise the x and y positions of the cursor
 	int xPos=2;
  	int yPos=2;
  	char c='\0'; 

  	int xspace=2;//initialising position for trailing space
  	int yspace=2;
  	int firstround=-1;//will indicate the first few loops
  	
  	int xapple,yapple,applespace;

 	int direction, spacedirec;
 	int isapple=0;
 	int body=1;
 	int pntcheck=0;

 	srand(time(NULL)); // initialise the random number generator

  	fillBounds(bounds, logicbounds, logicDirec);

  	mvaddch(yPos,xPos,'o');
  	
  	while(c!='q'){
		//randomly generate 'food' for snake to each. isapple makes sure only one 
  		//exists at a time and pntcheck ensures there is nothing at the genereated spot
  		if (isapple==0){
  			while(pntcheck==0){
  				yapple=rand()%(LEN-2)+1;
  				xapple=rand()%(LEN-2)+1;

  				if(logicbounds[yapple][xapple]==0){
  					mvaddch(yapple,xapple,'@');
  					logicbounds[yapple][xapple]=2;//2 will represent 'food' in logicbounds 
  					pntcheck=1;//point check success
  				}
  			}
  			pntcheck=0;	//reset pntcheck
  			isapple=1;
  		}


		c=getch();
		//changes direction of snake
		switch(c){
			case UP:
			if(direction!=2){			
				logicDirec[yPos][xPos]=1;
				yPos--;
				direction=1;
				body=0;	
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
		}//continues in specifed direction
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
		//checking if the new position is already occupied or contains food
		if((logicbounds[yPos][xPos]==1) && (firstround==0)){
			//go back one place and turn 'head' into an x
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
			mvaddch(yPos,xPos,'x');
			break;//exit loop
		}
		if(logicbounds[yPos][xPos]==2){
			applespace=1;
			isapple=0;
		}

		//print snake 
		mvaddch(yPos,xPos,'o');
		logicbounds[yPos][xPos]=1;
		
		//this section keeps the snake at a certain length by adding a space at the end of the  snake
		if (firstround==1){
			spacedirec=direction;
			mvaddch(yspace,xspace,' ');

			logicDirec[yspace][xspace]=0;
			logicbounds[yspace][xspace]=0;

			firstround=0;
		}
		if(applespace==1){
			applespace=0;
		}else{
		
			if (firstround==0){//move space coordiantes based on direction
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

		//check whether the space is where the snake turns and changing space direction accordingly
		if (logicDirec[yspace][xspace]!=0){
			spacedirec=logicDirec[yspace][xspace];
			logicDirec[yspace][xspace]=0;
		}
		
		if ((firstround==-1) && (body==0)){//to start runnin when key pressed
			firstround=1;
		}
		body=1;
	}
	
	while(c!='q'){//display message when game over
		int xdisp=LEN/2;
		int ydisp=LEN/2;
		mvprintw(ydisp,xdisp-5,"Game Over!");
		mvprintw(ydisp+1,xdisp-7,"press q to quit");
		c=getch();
	}

	endwin();
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