#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)
float movement_force = 150000.f;
float friction_force = 5.f;
float player_pos = 0.f;

float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 7;
float ball_half_size = 1;

struct Ball {
	float pos_x = 0;
	float pos_y = 0;
	float dp_x = 100;
	float dp_y = 0;
};

struct Player {
	float pos_x = 0.f;
	float pos_y = 0.f;
	float dp = 0;
	int score = 0;
};

enum Gamemode {
	GM_MENU, 
	GM_GAMEPLAY,
};


Gamemode current_gamemode;
int hot_button = 0;
bool enemy_is_ai;

Player player_1;
Player player_2;
Ball ball;

//player animation
float player_1_timer = 0;
float player_2_timer = 0;
int player_1_frame = 0;
int player_2_frame = 0;

//returns true if the two objects are collliding based off aabb collision
static bool
aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y,
	float p2x, float p2y, float hs2x, float hs2y) {

	return(p1x + hs1x > p2x - hs2x &&
		p1x + hs1x < p2x + hs2x &&
		p1y + hs1x > p2y - hs2y &&
		p1y + hs1x < p2y + hs2y);
}

// Player Movement
static void
simulate_player(Player* player, float ddp, float delta_time) {
	// Friction
	ddp -= player->dp * friction_force;

	// Momenteum
	player->pos_y = player->pos_y + player->dp * delta_time + ddp * delta_time * delta_time;
	player->dp = player->dp + ddp * delta_time;

	// Player collision with arena
	if (player->pos_y + player_half_size_y > arena_half_size_y) {
		player->pos_y = arena_half_size_y - player_half_size_y;
		player->dp *= -.5;
	}
	else if (player->pos_y - player_half_size_y < -arena_half_size_y) {
		player->pos_y = -arena_half_size_y + player_half_size_y;
		player->dp *= -.5;
	}
}

// Ball Movement
static void
simulate_ball(Ball* ball, Player* player_1, Player* player_2, float delta_time){
	// Ball Movement
	ball->pos_x += ball->dp_x * delta_time;
	ball->pos_y += ball->dp_y * delta_time;

	float ball_starting_speed_y = ball->dp_y;

	// Ball Collision
	if (aabb_vs_aabb(ball->pos_x, ball->pos_y, ball_half_size, ball_half_size,
		80, player_1->pos_y, player_half_size_x, player_half_size_y)) {

		//ball bounces off paddle
		ball->pos_x = 80 - player_half_size_x - ball_half_size;
		ball->dp_x *= -1;

		// Balls velocity gets affected based off paddles velocity
		// Ball bounces direction off paddle based on how far away from center
		ball->dp_y = (ball->pos_y - player_1->pos_x) * 2 + player_1->dp * .75f;
		
		// start player 1 animation
		player_1_frame = 1;
		player_1_timer = .3f;
	}
	else if (aabb_vs_aabb(ball->pos_x, ball->pos_y, ball_half_size, ball_half_size,
		-80, player_2->pos_y, player_half_size_x, player_half_size_y)) {

		//ball bounces off paddle
		ball->pos_x = -80 + player_half_size_x + ball_half_size;
		ball->dp_x *= -1;

		// Balls velocity gets affected based off paddles velocity
		// Ball bounces direction off paddle based on how far away from center
		ball->dp_y = (ball->pos_y - player_2->pos_x) * 2 + player_2->dp * .75f;

		// start player 2 animation
		player_2_frame = 1;
		player_2_timer = .3f;
	}

	// Ball bounces off roof
	if (ball->pos_y + ball_half_size > arena_half_size_y) {
		ball->pos_y = arena_half_size_y - ball_half_size;
		ball->dp_y *= -1;
	}
	else if (ball->pos_y - ball_half_size < -arena_half_size_y) {
		ball->pos_y = -arena_half_size_y + ball_half_size;
		ball->dp_y *= -1;
	}

	// Ball resets when out of bounds
	// goes out on right side of arena
	if (ball->pos_x + ball_half_size > arena_half_size_x) {
		ball->dp_x *= -1;
		ball->dp_y = 0;
		ball->pos_x = 0;
		ball->pos_y = 0;

		player_1->score++;
	}
	// goes out on left side of arena
	else if (ball->pos_x - ball_half_size < -arena_half_size_x) {
		ball->dp_x *= -1;
		ball->dp_y = 0;
		ball->pos_x = 0;
		ball->pos_y = 0;

		player_2->score++;
	}
}


void draw_player_1(int frame)
{
	if (frame == 0) {
		// body
		draw_rect(80, player_1.pos_y, 2.5f, 6, color_red);

		// top arm
		draw_rect(78, player_1.pos_y + 6, 2.5f + 2.5f, 1, color_red);

		draw_rect(74, player_1.pos_y + 6 - 2.5f, 1, 3, color_red);

		// bottom arm
		draw_rect(78, player_1.pos_y - 6, 2.5f + 2.5f, 1, color_red);

		draw_rect(74, player_1.pos_y - 6 + 2.5f, 1, 3, color_red);

		// head
		draw_rect(78, player_1.pos_y, 1.25, 3, 0xff6666);
	}else if (frame == 1) {
		// body
		draw_rect(80, player_1.pos_y, 2.5f, 6, color_red);

		// top arm
		draw_rect(78, player_1.pos_y + 6, 2.5f + 1.5f + .3f, 1, color_red);

		draw_rect(74, player_1.pos_y + 6 - 1.5f, 1.3f, 3.3f, color_red);

		// bottom arm
		draw_rect(78, player_1.pos_y - 6, 2.5f + 1.5f + .3f, 1, color_red);

		draw_rect(74, player_1.pos_y - 6 + 1.5f, 1.3f, 3.3f, color_red);

		// head
		draw_rect(78, player_1.pos_y, 1.25, 3, 0xff6666);
	}
}

void draw_player_2(int frame)
{
	if (frame == 0) {
		// Draw Player 2
			// body
		draw_rect(-80, player_2.pos_y, 2.5f, 6, color_blue);

		// top arm
		draw_rect(-78, player_2.pos_y + 6, 4, 1, color_blue);

		draw_rect(-74, player_2.pos_y + 6 - 1.5f, 1, 3, color_blue);

		// bottom arm
		draw_rect(-78, player_2.pos_y - 6, 4, 1, color_blue);

		draw_rect(-74, player_2.pos_y - 6 + 1.5f, 1, 3, color_blue);

		// head
		draw_rect(-78, player_2.pos_y, 1.25, 3, 0x6699ff);
	} else if (frame == 1) {
		// body
		draw_rect(-80, player_2.pos_y, 2.5f, 6, color_blue);

		// top arm
		draw_rect(-78, player_2.pos_y + 6, 2.5f + 1.5f + .3f, 1, color_blue);

		draw_rect(-74, player_2.pos_y + 6 - 1.5f, 1.3f, 3.3f, color_blue);

		// bottom arm
		draw_rect(-78, player_2.pos_y - 6, 2.5f + 1.5f + .3f, 1, color_blue);

		draw_rect(-74, player_2.pos_y - 6 + 1.5f, 1.3f, 3.3f, color_blue);

		// head
		draw_rect(-78, player_2.pos_y, 1.25, 3, 0x6699ff);
	}
}

// Main game run function
void simulate_game(Input* input, float delta_time) 
{
	clear_screen(0xff5500);
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0xffaa33);


	//Gameplay gamemode
	if (current_gamemode == GM_GAMEPLAY) {
		// Player 1 Input
		float player_1_ddp = 0.f;

		if (!enemy_is_ai) {
			// Human for player 1
			if (is_down(BUTTON_UP)) player_1_ddp += movement_force * delta_time;
			if (is_down(BUTTON_DOWN)) player_1_ddp -= movement_force * delta_time;

		} else {
			// AI for player 1
			// Basic AI
			///if (ball.pos_y > player_1.pos_y) player_1_ddp += movement_force * delta_time;
			// else if (ball.pos_y < player_1.pos_y) player_1_ddp -= movement_force * delta_time;
			// Better AI
			player_1_ddp = (ball.pos_y - player_1.pos_y) * 100;
			if (player_1_ddp > movement_force) player_1_ddp = movement_force;
			if (player_1_ddp < -movement_force) player_1_ddp = -movement_force;
		}

		// Player 2 Input
		float player_2_ddp = 0.f;
		if (is_down(BUTTON_W)) player_2_ddp += movement_force * delta_time;
		if (is_down(BUTTON_S)) player_2_ddp -= movement_force * delta_time;

		//draw_ball(ball.pos_x, ball.pos_y, 2, color_white);

		// Player Movement
		simulate_player(&player_1, player_1_ddp, delta_time);
		simulate_player(&player_2, player_2_ddp, delta_time);

		// Simulates ball movement
		simulate_ball(&ball, &player_1, &player_2, delta_time);

		// Rendering
		// Draw Ball
		draw_rect(ball.pos_x, ball.pos_y, 1, 1, color_white);

		// Draw Player 1
		// Draw Player 1
		if (player_1_frame == 0)
			draw_player_1(0);
		else if (player_1_frame == 1)
			draw_player_1(1);

		// Draw Player 2
		if(player_2_frame == 0)
			draw_player_2(0);
		else if (player_2_frame == 1)
			draw_player_2(1);


		// Player Animation Controller
		if (player_1_frame != 0 && player_1_timer <= 0)
		{
			player_1_frame--;
			player_1_timer = .3f;
		}
		if (player_2_frame != 0 && player_2_timer <= 0)
		{
			player_2_frame--;
			player_2_timer = .3f;
		}

		player_1_timer -= delta_time;
		player_2_timer -= delta_time;

		// draw net
		draw_rect(0, 0, .5f, arena_half_size_y, 0xffffff);


		// Display Score
		draw_number(player_1.score, -10, 40, 1.f, 0xbbffbb);
		draw_number(player_2.score, 10, 40, 1.f, 0xbbffbb);

		// Draw Player Controls
		if(!enemy_is_ai)
			draw_text("ARROW UP/ARROW DOWN", 30, -45.5, .5, 0xffffff);
		draw_text("W/S", -85, -45.5, .5, 0xffffff);

		//circleBres(1000, 1000, 300);

	} else{
	//Main menu gamemode
		// Swap between main menu buttons
		if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT) || pressed(BUTTON_A) || pressed(BUTTON_D) || pressed(BUTTON_W)) {
			hot_button = !hot_button;
		}

		// Selects currently selected button
		if (pressed(BUTTON_ENTER) || pressed(BUTTON_SPACE))
		{
			current_gamemode = GM_GAMEPLAY;
			enemy_is_ai = hot_button ? 0 : 1;
		}
		
		// Shows single player as highlighted
		if (hot_button == 0) {
			draw_text("SINGLE PLAYER", -80, -10, 1, 0xff0000);
			draw_text("MULTIPLAYER", 20, -10, 1, 0xaaaaaa);
		}
		// Shows multiplayer as highlighted
		else {
			draw_text("SINGLE PLAYER", -80, -10, 1, 0xaaaaaa);
			draw_text("MULTIPLAYER", 20, -10, 1, 0xff0000);
		}

		// Main menu controls text
		draw_text("ARROW LEFT/ARROW RIGHT", -30, -25, .5, 0xffffff);
		draw_text("ENTER TO SELECT", -20, -30, .5, 0xffffff);

		// Title text
		draw_text("ALMOST VOLLEYBALL", -73, 35, 1.25f, 0xffffff);
		draw_text("BY CALEB DETERS", -73, 22, .5, 0xffffff);
		draw_text("TUTORIAL BY YOUTUBE.COM/DANAIDIAN", - 73, 15, .5, 0xffffff);
	}
}