#define row1 A3
#define row2 2
#define row3 3
#define row4 4
#define row5 5
#define row6 6
#define row7 7
#define row8 8

#define col1 9
#define col2 10
#define col3 11
#define col4 12
#define col5 13
#define col6 A0
#define col7 A1
#define col8 A2

#define vrX A4
#define vrY A5

bool eaten = false;
int food[2] = {4,4};
enum directions {NORTH, EAST, WEST, SOUTH};

unsigned long lastTime;
byte pointX = 0;
byte pointY = 0;

const byte rows[] = {
    row1, row2, row3, row4, row5, row6, row7, row8
};
const byte cols[] = {
  col1,col2, col3, col4, col5, col6, col7, col8
};

byte snake_length = 2;
enum directions dir;
int snake[64][2]={{0,1},{0,0}};
byte screen[8][8] = {{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};

void setup() {
  Serial.begin(9600);
  // setting all pins to output mode
  for (byte i = 1; i <= 13; i++)
        pinMode(i, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);

  pinMode(A4, INPUT);
  pinMode(A5, INPUT);

  // Setting all colums to high by default (turned off)
  for(byte col: cols)
    digitalWrite(col, HIGH);

  // initial screen
  // screen[pointX][pointY] = 1;

  // snake setup
  dir = EAST;
  screen[snake[0][0]][snake[0][1]] = 1;
  screen[snake[1][0]][snake[1][1]] = 1;
  screen[food[0]][food[1]] = 1;
}

void loop() {
  int corX = map(analogRead(vrX), 0, 1023, -1, 1);
  int corY = map(analogRead(vrY), 0, 1023, -1, 1);
//  if(millis() - lastTime > 200){
//    Serial.print("X: "); Serial.print(corX); Serial.print("Y: "); Serial.println(corY);
//  }
  if(millis() - lastTime > 400){
    updateScreen(corX, corY);
    lastTime = millis();
  }
  drawFrame();
}

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void updateScreen(int x, int y){ // add boundary conditions
  if(!eaten){
    screen[snake[snake_length-1][0]][snake[snake_length-1][1]] = 0; //deactivate current snake end before updating values
  }
  updateSnake(x, y);
  screen[snake[0][0]][snake[0][1]] = 1; //activate snake head at new pos


//  screen[pointX][pointY] = 0;
//  pointY = (pointY+1)%8;
//  if(pointY == 0) 
//    pointX = (pointX+1)%8;
//  screen[pointX][pointY] = 1;
}

void updateSnake(int x, int y){
  //update tail
  if(eaten){
    snake[snake_length][0] = snake[snake_length - 1][0];
    snake[snake_length][1] = snake[snake_length - 1][1];
  }
  for(int i = snake_length; i>0; i--){
    snake[i][0] = snake[i-1][0];
    snake[i][1] = snake[i-1][1];
  }

  if(eaten){
    snake_length++;
    eaten = false;
    
  }
  // update head
  if(dir == NORTH || dir == SOUTH){
    if(x == 1){ // go EAST
      snake[0][1] = (snake[0][1]+1)%8;
      dir = EAST;
    }
    else if(x == -1){ // go WEST
      snake[0][1] = ((snake[0][1]-1) == -1 ? 7 : (snake[0][1]-1));
      dir = WEST;
    }
    else { // continue course
      if(dir == NORTH) snake[0][0] = ((snake[0][0]-1) == -1 ? 7 : (snake[0][0]-1));
      if(dir == SOUTH) snake[0][0] = (snake[0][0]+1)%8;
    }
  }
  else if(dir == EAST || dir == WEST){
    if(y == 1){ // go SOUTH
      snake[0][0] = (snake[0][0]+1)%8;
      dir = SOUTH;
    }
    else if(y == -1){ // go NORTH
      snake[0][0] = ((snake[0][0]-1) == -1 ? 7 : (snake[0][0]-1));
      dir = NORTH;
    }
    else { // continue course
      if(dir == EAST) snake[0][1] = (snake[0][1]+1)%8;
      if(dir == WEST) snake[0][1] = ((snake[0][1]-1) == -1 ? 7 : (snake[0][1]-1));
    }
  }

  if(snake[0][0]==food[0] && snake[0][1]==food[1]){
    Serial.println("Fod eaten");
    eaten = true;
    while(true){
      int a = random(0,8);
      int b = random(0,8);
      if(!screen[a][b]){
        food[0] = a; food[1] = b;
        screen[a][b] = 1;
        break;
      }
    }
  }

  for(int i = 1; i < snake_length; i++)
  if(snake[0][0]== snake[i][0] && snake[0][1]==snake[i][1]){
    resetFunc();
  }
  Serial.print("\n Snake x: "); Serial.print(snake[0][0]);
  Serial.print(", y: "); Serial.print(snake[0][1]); Serial.print(" Direction: "); Serial.print(dir);
  Serial.print("Food? "); Serial.print(eaten);
}

void drawFrame(){
  for(byte i=0; i < 8; i++){
    for(byte j = 0; j < 8; j++){
      if(screen[i][j])
        activatePixel(i, j);
      else
        deactivatePixel(i, j);
    }
  }
}

void activatePixel(byte x, byte y){
  digitalWrite(rows[x], LOW);  digitalWrite(cols[y], HIGH);
  digitalWrite(rows[x], HIGH);  digitalWrite(cols[y], LOW);   
}

void deactivatePixel(byte x, byte y){
  digitalWrite(rows[x], HIGH);  digitalWrite(cols[y], LOW );
}
