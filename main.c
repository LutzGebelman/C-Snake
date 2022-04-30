#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Segment stuff
//  Defining key structors

enum Dir {
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT
} typedef DIRECTION;

struct Seg{
	Vector2 ball_pos;
	Vector2 last_pos;
    DIRECTION dir;
    struct Seg * next;
	unsigned int speed;
	Vector2 screen;
	double movement_start;
} typedef Segment;

struct AppleStruct{
	bool exists;
	Vector2 pos;
	Vector2 screen;
} typedef Apple;

//  Segment Logic
Segment * _Get_Last(Segment * first){
    if ( !first->next ) return first;
    Segment * segment = first;
	while(segment->next){
		segment = segment->next;
	}
    return segment;
}

// Generates random position on the screen
Vector2 _Get_Random_Pos( unsigned int Width, unsigned int Height ){
	srand(time(NULL));
	int X = rand() % (Width + 1);
	int Y = rand() % (Height + 1);
	Vector2 ans = {X, Y};
	return ans;
}

// Adds new segment to the snake behind the last segment
void Add_To_Last(Segment * first){
    Segment * last = _Get_Last(first);
    Segment * new_ptr = (Segment*)malloc(sizeof(Segment));
	new_ptr->ball_pos = last->ball_pos;
	new_ptr->dir = last->dir;
	new_ptr->next = NULL;
	new_ptr->speed = last->speed;
	new_ptr->screen = last->screen;
	last->next = new_ptr;
}

double Get_Distance(Vector2 * first, Vector2 * second){
	return sqrt( pow((double)(first->x - second->x), 2.0) + pow((double)(first->y - second->y), 2.0) );
}


//  Segments Movement logic
double Move_Segment(Segment * segment){ // returns time of movement
	segment->last_pos = segment->ball_pos;
	switch (segment->dir)
	{
		case NONE:
			break;
		case UP:
			segment->ball_pos.y = segment->ball_pos.y - segment->speed;
			break;
		case DOWN:
			segment->ball_pos.y = segment->ball_pos.y + segment->speed;
			break;
		case LEFT:
			segment->ball_pos.x = segment->ball_pos.x - segment->speed;
			break;
		case RIGHT:
			segment->ball_pos.x = segment->ball_pos.x + segment->speed;
			break;
		default:
			break;
	}

	// Teleport the head if it reaches the boarder
	if (segment->ball_pos.x > segment->screen.x) segment->ball_pos.x = 0;
	if (segment->ball_pos.x < 0) segment->ball_pos.x = segment->screen.x;
	if (segment->ball_pos.y > segment->screen.y) segment->ball_pos.y = 0;
	if (segment->ball_pos.y < 0) segment->ball_pos.y = segment->screen.y;

	// segment->last_pos = segment->ball_pos;
	// segment->ball_pos = new_pos;
}

//	Move all segments by specifide delay
void Move_Segs(Segment * first, unsigned int delay){
    Segment * segment = first;
	Move_Segment(segment);
    while (segment->next)
	{
		segment->last_pos = segment->ball_pos;
		segment->next->ball_pos = segment->last_pos;
		segment = segment->next;
		// double Calc_Distance = Get_Distance( &segment->ball_pos, &segment->next->ball_pos );
		// Wright_Value_On_Screen(*delay, 10, 0);
		// Wright_Value_On_Screen(Calc_Distance, 10, 40);
		// if ( Calc_Distance > *delay ) {
		// 	Move_Segment(segment->next, segment->last_pos);
		// }
		// segment = segment->next;
	}
}

bool Check_For_collisions(Segment * head){
	if (!head->next) return false;
	Segment * segment = head->next;
	while (segment->next)
	{
		if (CheckCollisionCircles(head->ball_pos, 2, segment->ball_pos, 2)) return true;
		segment = segment->next;
	}
	return false;
}

// End of Segment segment

// Apple Logic
void Make_Apple_Exist(Apple * apple){
	Vector2 pos = _Get_Random_Pos(apple->screen.x, apple->screen.y);
	apple->pos = pos;
	apple->exists = true;
}

void Handle_Apple(Apple * apple, Segment * head){
	if ( !apple->exists ){
		Make_Apple_Exist(apple);
	};

	if ( CheckCollisionCircles( apple->pos, 10, head->ball_pos, 10 ) ){
		
		if (apple->exists) Add_To_Last(head);
		apple->exists = false;
		Make_Apple_Exist(apple);
	}
}

// Drawing the thing
void Draw_Snake(Segment * first){
    Segment * segment = first;
	while (segment)
	{
    	DrawCircle(segment->ball_pos.x, segment->ball_pos.y, 10, PURPLE);
		if (segment->next) segment = segment->next;
		else break;
	}
}

void Draw_Apple(Apple apple){
	if (apple.exists){
		DrawCircle(apple.pos.x, apple.pos.y, 10, RED);
	}
}


// Debuging functions
void Wright_Value_On_Screen(double value, int position_X, int position_Y){
	char value_str[20];
	_itoa_s(value, value_str, 10, 10);
	DrawText(value_str, position_X, position_Y, 20, DARKGRAY);
}

unsigned int Count_Segments(Segment * head){
	Segment * segment = head;

	unsigned int count = 0;
	while (segment->next)
	{
		count++;
		segment = segment->next;
	}
	return count;
}

// Game logic

void Game_Over(){
	EndDrawing();
	CloseWindow();
}

void Game_Logic(Vector2 screen, Segment * head_ptr, Apple * apple, unsigned int * delay){
	// keyboard handling
	if (IsKeyPressed(KEY_EQUAL)) Add_To_Last(head_ptr);
	if (IsKeyPressed(KEY_MINUS)) *delay = *delay + 1;
	if (IsKeyPressed(KEY_LEFT)) head_ptr->dir = LEFT;
	if (IsKeyPressed(KEY_RIGHT)) head_ptr->dir = RIGHT;
	if (IsKeyPressed(KEY_UP)) head_ptr->dir = UP;
	if (IsKeyPressed(KEY_DOWN)) head_ptr->dir = DOWN;

	Move_Segs(head_ptr, *delay);
	// if(	Check_For_collisions(head_ptr) ) Game_Over();
	Handle_Apple(apple, head_ptr);
}


int main() {

	// A bunch of much needed variables
	const int Width = 800;
	const int Height = 480;
	
	Vector2 screen = {Width, Height}; 

	unsigned int speed = 2; // The number of pixels, that segment will move in one frame
	unsigned int delay = 1; // 

	// Initilise apple
	struct AppleStruct apple = {false, {0, 0}, screen};

	// Initializing the first segment
	Vector2 start_pos = { (float)screen.x / 2, (float)screen.y / 2 };
	Segment head_content = {start_pos, start_pos, NONE, NULL, delay};
	Segment * head_ptr = (Segment*)malloc(sizeof(Segment));
	head_ptr->ball_pos = start_pos;
	head_ptr->dir = NONE;
	head_ptr->next = NULL;
	head_ptr->speed = speed;
	head_ptr->screen = screen;

	// Some Raylib related stuff
	InitWindow(Width, Height, "Some shit that will display in the title of the window");
	SetTargetFPS(75);

	// The main loop
	while (!WindowShouldClose()) {

		Game_Logic(screen, head_ptr, &apple, &delay);

		BeginDrawing();

			ClearBackground(RAYWHITE);
			DrawText("Some stupid shit", Width / 2, Height / 2, 20, LIGHTGRAY);
			Wright_Value_On_Screen((double)Count_Segments(head_ptr), 0, 20);

			Draw_Apple(apple);
            Draw_Snake(head_ptr);

		EndDrawing();
		}
	CloseWindow();
	return 0;
}