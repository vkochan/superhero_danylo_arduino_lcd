#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978

int melody1_notes[] = { //tone array
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0,
    NOTE_B3, NOTE_B3, NOTE_B3, NOTE_B3, NOTE_A3, NOTE_G3,
    NOTE_B3, NOTE_B3, NOTE_B3, NOTE_B3, NOTE_A3, NOTE_G3,
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_G3, 0
};
int melody1_time[] = {4, 8, 8, 4, 4, 4, 8, 8, 8, 8, 4, 4, 8, 8, 8, 8, 4, 4, 4, 8, 8, 4, 4};

int melody2_notes[] = {NOTE_GS7, NOTE_DS8, NOTE_GS7, 0, NOTE_DS8, NOTE_DS8, 0, NOTE_GS7, NOTE_GS7};
int melody2_time[] = {4, 8, 8, 4, 8, 8, 4, 4, 4};

int player_fire_sound[] = {NOTE_G4, NOTE_G5, NOTE_G6};
int player_fire_time[] = {35, 35, 35};

int enemy_shoot_sound[] = {NOTE_C4, NOTE_C5, NOTE_C6};
int enemy_shoot_time[] = {35, 35, 35};

int enemy_died_sound[] = {
  550, 404, 315, 494, 182, 260, 455, 387, 340, 550
};

int enemy_died_time[] = {
  40, 40, 40, 40, 40, 40, 40, 40, 40, 40
};

int *note_durations_ptr = NULL;
int *note_array_ptr = NULL;
unsigned long note_next_time = 0;
bool note_abs_time = false;
int note_num = 0;
int note_curr = 0;

#define ROWS 2
#define COLS 16

#define JOY_X_PIN     A0
#define JOY_Y_PIN     A1
#define FIRE_PIN      8

static LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

#define PLAYER_CHAR_LEFT     0
#define PLAYER_CHAR_RIGHT    1
#define ENEMY_CHAR           2
#define BULLET_CHAR          '-'

static byte player_right[] = {
  B00110,
  B01111,
  B01110,
  B00100,
  B11111,
  B11110,
  B00100,
  B00110
};

static byte player_left[] = {
  B01100,
  B11110,
  B01110,
  B00100,
  B11111,
  B01111,
  B00100,
  B01100
};

static byte enemy[] = {
  B00100,
  B01110,
  B10101,
  B01010,
  B11111,
  B10101,
  B00100,
  B11011
};

/*
byte player[] = {
  B01110,
  B11111,
  B01110,
  B01010,
  B00100,
  B11111,
  B10101,
  B00100
};
*/
/*
static byte enemy[] = {
  B00000,
  B01110,
  B10101,
  B11011,
  B01110,
  B01110,
  B00000,
  B00000
};
*/
static int joy_x = 0;
static int joy_y = 0;

enum PlayerMove {
  PlayerMove_None,
  PlayerMove_Up,
  PlayerMove_Down,
  PlayerMove_Left,
  PlayerMove_Right,
};

#define DRAW_DELAY  200
static unsigned long last_draw = 0;

bool started = false;
bool game_over = false;

#define EXPLODE_NUM 3

static int effect_map[ROWS] = {};
static uint8_t *effect_ptr[ROWS][COLS] = {};
static uint8_t effect_num[ROWS][COLS] = {};
static uint8_t effect_idx[ROWS][COLS] = {};

static int effect_static_map[ROWS] = {};
static uint8_t *effect_static_ptr[ROWS][COLS] = {};
static uint8_t effect_static_num[ROWS][COLS] = {};
static uint8_t effect_static_idx[ROWS][COLS] = {};

static uint8_t hole_effect[] = { 'o', 'o', 'o', 'O', 'O', 'O' };

static uint8_t explode_effect[EXPLODE_NUM] = {'*', 'X', '"' };
static int explode_time = 0;

static int button_debounce_time = 50;
static unsigned long button_debounce_timeout = 0;
static bool button_is_pressed = false;

static byte player_char = PLAYER_CHAR_RIGHT;
static byte player_row = 0;
static byte player_col = 0;
static bool player_dead = false;
static byte player_move = PlayerMove_None;
static byte bullet_row = 0;
static byte bullet_col = 1;
static int player_bullet_map_right[ROWS] = {0, 0};
static byte enemy_reborn_row = 0;
static byte enemy_reborn_col = 8;
static int enemy_bullet_map[ROWS] = {0, 0};
static unsigned long enemy_next_fire = 0;
static unsigned long enemy_fire_time = 3000;
static byte enemy_row = 0;
static byte enemy_col = 8;
static bool enemy_alive = false;
static bool enemy_move = false;
static unsigned long enemy_move_time = 2000;
static unsigned long enemy_move_next = 0;
static unsigned long enemy_last_alive = 0;
static int enemy_reborn = 2000;
static bool is_explode = false;
static bool start_fire = false;

static uint8_t screen[ROWS][COLS] = {' '};

void frame_clear()
{
  for (int r = 0; r < ROWS; r++)
  {
    for (int c = 0; c < COLS; c++)
    {
      screen[r][c] = ' ';
    }
  }
}

void frame_write(int row, int col, uint8_t ch)
{
  screen[row][col] = ch;
}

void frame_draw()
{
  for (int r = 0; r < ROWS; r++)
  {
    for (int c = 0; c < COLS; c++)
    {
      lcd.setCursor(c, r);
      lcd.write(screen[r][c]);
    }
  }
}

void sound_start(int note, int duration)
{
  tone(3, note, duration);
}

void sound_stop(void)
{
  noTone(3);
}

void __play(int *notes, int *duration, int num) {
   note_next_time = 0;
   note_array_ptr = notes;
   note_durations_ptr = duration;
   note_num = num;
   note_curr = 0;
}

void play_sound(int *notes, int *duration, int num) {
  __play(notes, duration, num);
  note_abs_time = true;
}

void play_melody(int *notes, int *duration, int num)
{
   __play(notes, duration, num);
  note_abs_time = false;
}

void play_game_over(void)
{
  play_melody(melody1_notes, melody1_time, 23);
}

void sound_handle(void)
{
   if (note_array_ptr == NULL || note_durations_ptr == NULL || note_num == 0)
      return;

   if (millis() >= note_next_time) {
     int note_time = note_durations_ptr[note_curr];

     if (!note_abs_time)
       note_time = 1000 / note_time;

     sound_start(note_array_ptr[note_curr], note_time);

     note_curr++;

     if (note_curr < note_num)
      note_next_time = millis() + note_time * 1.30;
   }

   if (note_curr == note_num) {
     note_next_time = 0;
     note_array_ptr = NULL;
     note_durations_ptr = NULL;
     note_num = 0;
     note_curr = 0;
     sound_stop();
   }
}

void setup() {
  Serial.begin(9600);

  pinMode(FIRE_PIN, INPUT_PULLUP);
  
  lcd.init();
  lcd.backlight();
  lcd.createChar(PLAYER_CHAR_RIGHT, player_right);
  lcd.createChar(PLAYER_CHAR_LEFT, player_left);
  lcd.createChar(ENEMY_CHAR, enemy);

  lcd.setCursor(2, 0);
  lcd.print("Super Danylo!");

  lcd.setCursor(8, 1);
  lcd.write(0); 
  delay(2000);  // pause 2 seconds
}

void button_handle()
{
  int button_current_state = digitalRead(FIRE_PIN);

  if (button_current_state == LOW && button_debounce_timeout == 0) {
    button_debounce_timeout = millis() + button_debounce_time;
  } else if (button_debounce_timeout && millis() >= button_debounce_timeout) {
    button_debounce_timeout = 0;
    if (button_current_state == LOW) { 
      if (!start_fire) {
          play_sound(player_fire_sound, player_fire_time, 3);
          start_fire = true;
      }
    }    
  }

  joy_x = analogRead(JOY_X_PIN);
  joy_y = analogRead(JOY_Y_PIN);

  if (joy_y < 400) {
     if (player_row > 0) {
       player_move = PlayerMove_Down;
     }
  } else if (joy_y > 600) {
    if (player_row < ROWS - 1) {
      player_move = PlayerMove_Up;
    }
  }

  if (joy_x < 400) {
    if (player_col > 0) {
      player_move = PlayerMove_Left;
    }
  } else if (joy_x > 600) {
    if (player_col < COLS - 1) {
      player_move = PlayerMove_Right;
    }
  }
}

void effect_add(int r, int c, uint8_t *ptr, int num)
{
  effect_map[r] = effect_map[r] | (1 << c);
  effect_ptr[r][c] = ptr;
  effect_num[r][c] = num;
  effect_idx[r][c] = 0;
}

void effect_add_static(int r, int c, uint8_t *ptr, int num)
{
  effect_static_map[r] = effect_static_map[r] | (1 << c);
  effect_static_ptr[r][c] = ptr;
  effect_static_num[r][c] = num;
  effect_static_idx[r][c] = 0;
}


void draw_effects(void)
{
  for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
      int bit = 1 << c;

      if (effect_map[r] & bit) {
        int idx = effect_idx[r][c];
        frame_write(r, c, *effect_ptr[r][c]);
        idx++;
        effect_idx[r][c] = idx;

        if (idx >= effect_num[r][c]) {
          effect_map[r] = effect_map[r] & ~bit;
          effect_num[r][c] = 0;
          effect_idx[r][c] = 0;
          effect_ptr[r][c] = NULL;
        }
      } else if (effect_static_map[r] & bit) {
        int idx = effect_static_idx[r][c];
        frame_write(r, c, effect_static_ptr[r][c][idx]);
        idx++;
        effect_static_idx[r][c] = idx;

        if (idx >= effect_static_num[r][c]) {
          effect_static_idx[r][c] = 0;
        }
      }
    }
  }
}

void enemy_died()
{
    effect_add(enemy_row, enemy_col, explode_effect, 3);
    play_sound(enemy_died_sound, enemy_died_time, 10);
    enemy_last_alive = millis();
    enemy_alive = false;
}

void enemy_fire()
{
    if (millis() >= enemy_next_fire && enemy_row < ROWS && enemy_col > 0) {
      enemy_bullet_map[enemy_row] = enemy_bullet_map[enemy_row] | (1 << (enemy_col - 1));
      enemy_next_fire = millis() + enemy_fire_time;
      play_sound(enemy_shoot_sound, enemy_shoot_time, 3);
    }
}

void enemy_born()
{
    enemy_alive = true;
    enemy_row = random() % 2;
    enemy_col = enemy_reborn_col;
}

void check_target()
{
  for (int r = 0; r < ROWS; r++) {
    for (int b = 0; b < COLS; b++) {
       //player
       if ((1 << b) & player_bullet_map_right[r]) {
         if (enemy_alive && r == enemy_row && b == enemy_col) {
          player_bullet_map_right[r] = player_bullet_map_right[r] & (~(1 << b));
          enemy_died();       
         }
       }
       //enemy
       if ((1 << b) & enemy_bullet_map[r]) {
         if (enemy_alive && r == player_row && b == player_col) {
          enemy_bullet_map[r] = enemy_bullet_map[r] & (~(1 << b));
          player_dead = true;     
         }
       }
    }
  }
}

void draw_ground(void)
{
  for (int c = 0; c < COLS; c++) {
    frame_write(1, c, '.');
  }
}

void draw_map(void)
{
  draw_ground();
  frame_write(1, 1, '\\');
  frame_write(0, 10, 200);
}

void on_draw()
{
  draw_map();
  draw_effects();

  if (start_fire) {
    if (player_row < ROWS && player_col < COLS - 2) {
      player_bullet_map_right[player_row] = player_bullet_map_right[player_row] | (1 << (player_col + 1));
    }
    start_fire = false;
  }

  if (!enemy_alive) {
    if (millis() - enemy_last_alive >= enemy_reborn) {
      enemy_born();
    }
  } else if (!is_explode) {
      enemy_fire();
  }

  if (player_move == PlayerMove_Down)
    player_row--;
  else if (player_move == PlayerMove_Up)
    player_row++;
  else if (player_move == PlayerMove_Left)
    player_col--;
  else if (player_move == PlayerMove_Right)
    player_col++;
  
  player_move = PlayerMove_None;
    
  frame_write(player_row, player_col, player_char);

  if (enemy_alive) {
       frame_write(enemy_row, enemy_col, ENEMY_CHAR);
        if (enemy_col == 0) {
         enemy_died();
       } else if (millis() >= enemy_move_next) {
         check_target();
         enemy_row = random() % 2;
         enemy_move_next = millis() + enemy_move_time;
       }
  }

  for (int r = 0; r < ROWS; r++) {
    for (int b = 0; b < COLS; b++) {
      //player
      if ((1 << b) & player_bullet_map_right[r]) {
         frame_write(r, b, BULLET_CHAR);
      }
      //enemy
      if ((1 << b) & enemy_bullet_map[r]) {
         frame_write(r, b, '+');
      }
    }
    player_bullet_map_right[r] = player_bullet_map_right[r] << 1;
    player_bullet_map_right[r] = player_bullet_map_right[r] & ((1 << COLS)-1);
    enemy_bullet_map[r] = enemy_bullet_map[r] >> 1;
    enemy_bullet_map[r] = enemy_bullet_map[r] & ((1 << COLS)-1);
  }
  check_target();
}

void loop() {
   if (!started) {
     //put some game start code here
     play_melody(melody2_notes, melody2_time, 9);
     effect_add_static(0, 12, hole_effect, 6);
     effect_add_static(1, 4, hole_effect, 6);
     started = true;
   }

   button_handle();
   sound_handle();

   if (!player_dead && millis() - last_draw >= DRAW_DELAY) {
    frame_clear();
    on_draw();
    frame_draw();
    last_draw = millis();
  }

  if (player_dead && !game_over) {
    frame_clear();
    frame_draw();
    lcd.setCursor(4, 0);
    lcd.print("GAME OVER");
    play_game_over();
    game_over = true;
  }
}