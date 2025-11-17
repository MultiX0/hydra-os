#include "snake.h"
#include "fb.h"
#include "keyboard.h"

#define GAME_WIDTH 40
#define GAME_HEIGHT 20
#define MAX_SNAKE_LENGTH 100

/* Direction constants */
#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3

/* Snake segment structure */
struct segment {
    int x;
    int y;
};

/* Game state */
static struct segment snake[MAX_SNAKE_LENGTH];
static int snake_length;
static int direction;
static int next_direction;
static struct segment food;
static int score;
static int game_over;

/* Simple random number generator */
static unsigned int seed = 12345;

int rand(void) {
    seed = seed * 1103515245 + 12345;
    return (seed / 65536) % 32768;
}

void srand(unsigned int s) {
    seed = s;
}

/* Delay function (very crude) */
void delay(unsigned int count) {
    volatile unsigned int i, j;
    for (i = 0; i < count; i++) {
        for (j = 0; j < 10000; j++) {
            /* Just waste time */
        }
    }
}

/* Integer to string conversion */
void int_to_str(int num, char *str) {
    int i = 0;
    int is_negative = 0;
    
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }
    
    while (num > 0) {
        str[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    if (is_negative) {
        str[i++] = '-';
    }
    
    str[i] = '\0';
    
    /* Reverse the string */
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

/* Draw a character at a specific position */
void draw_at(int x, int y, char c) {
    if (x < 0 || x >= GAME_WIDTH || y < 0 || y >= GAME_HEIGHT) {
        return;
    }
    
    /* Move to the position (top offset of 2 lines for UI) */
    int pos = (y + 2) * 80 + (x + 20);  /* Center the game on screen */
    
    /* Direct framebuffer access */
    char *fb = (char *) 0x000B8000;
    fb[pos * 2] = c;
    fb[pos * 2 + 1] = 0x0F;  /* White on black */
}

/* Clear the game area */
void clear_game_area(void) {
    int x, y;
    for (y = 0; y < GAME_HEIGHT; y++) {
        for (x = 0; x < GAME_WIDTH; x++) {
            draw_at(x, y, ' ');
        }
    }
}

/* Draw the border */
void draw_border(void) {
    int i;
    
    /* Top and bottom borders */
    for (i = 0; i < GAME_WIDTH; i++) {
        draw_at(i, 0, '#');
        draw_at(i, GAME_HEIGHT - 1, '#');
    }
    
    /* Left and right borders */
    for (i = 0; i < GAME_HEIGHT; i++) {
        draw_at(0, i, '#');
        draw_at(GAME_WIDTH - 1, i, '#');
    }
}

/* Generate new food position */
void generate_food(void) {
    int valid;
    int i;
    
    do {
        valid = 1;
        food.x = (rand() % (GAME_WIDTH - 2)) + 1;
        food.y = (rand() % (GAME_HEIGHT - 2)) + 1;
        
        /* Check if food spawned on snake */
        for (i = 0; i < snake_length; i++) {
            if (snake[i].x == food.x && snake[i].y == food.y) {
                valid = 0;
                break;
            }
        }
    } while (!valid);
}

/* Initialize the game */
void init_game(void) {
    /* Clear screen and draw UI */
    fb_clear();
    fb_puts("SNAKE GAME - Use WASD to move, Q to quit\n");
    fb_puts("Score: 0\n");
    
    /* Initialize snake in the middle */
    snake_length = 3;
    snake[0].x = GAME_WIDTH / 2;
    snake[0].y = GAME_HEIGHT / 2;
    snake[1].x = snake[0].x - 1;
    snake[1].y = snake[0].y;
    snake[2].x = snake[1].x - 1;
    snake[2].y = snake[1].y;
    
    direction = DIR_RIGHT;
    next_direction = DIR_RIGHT;
    score = 0;
    game_over = 0;
    
    /* Seed random with snake position */
    srand(snake[0].x * snake[0].y * 12345);
    
    /* Generate first food */
    generate_food();
    
    /* Draw game area */
    clear_game_area();
    draw_border();
}

/* Update the score display */
void update_score(void) {
    char score_str[20];
    char *fb = (char *) 0x000B8000;
    int i;
    
    /* Clear the score line */
    for (i = 0; i < 20; i++) {
        fb[(1 * 80 + 7 + i) * 2] = ' ';
    }
    
    /* Write new score */
    int_to_str(score, score_str);
    i = 0;
    while (score_str[i] != '\0') {
        fb[(1 * 80 + 7 + i) * 2] = score_str[i];
        i++;
    }
}

/* Draw the snake */
void draw_snake(void) {
    int i;
    for (i = 0; i < snake_length; i++) {
        if (i == 0) {
            draw_at(snake[i].x, snake[i].y, 'O');  /* Head */
        } else {
            draw_at(snake[i].x, snake[i].y, 'o');  /* Body */
        }
    }
}

/* Draw the food */
void draw_food(void) {
    draw_at(food.x, food.y, '*');
}

/* Check collision with walls or self */
int check_collision(int x, int y) {
    int i;
    
    /* Check walls */
    if (x <= 0 || x >= GAME_WIDTH - 1 || y <= 0 || y >= GAME_HEIGHT - 1) {
        return 1;
    }
    
    /* Check self collision */
    for (i = 1; i < snake_length; i++) {
        if (snake[i].x == x && snake[i].y == y) {
            return 1;
        }
    }
    
    return 0;
}

/* Update game state */
void update_game(void) {
    int i;
    int new_x, new_y;
    int ate_food = 0;
    
    /* Update direction (prevent 180-degree turns) */
    if ((direction == DIR_UP && next_direction != DIR_DOWN) ||
        (direction == DIR_DOWN && next_direction != DIR_UP) ||
        (direction == DIR_LEFT && next_direction != DIR_RIGHT) ||
        (direction == DIR_RIGHT && next_direction != DIR_LEFT)) {
        direction = next_direction;
    }
    
    /* Calculate new head position */
    new_x = snake[0].x;
    new_y = snake[0].y;
    
    switch (direction) {
        case DIR_UP:
            new_y--;
            break;
        case DIR_DOWN:
            new_y++;
            break;
        case DIR_LEFT:
            new_x--;
            break;
        case DIR_RIGHT:
            new_x++;
            break;
    }
    
    /* Check collision */
    if (check_collision(new_x, new_y)) {
        game_over = 1;
        return;
    }
    
    /* Check if ate food */
    if (new_x == food.x && new_y == food.y) {
        ate_food = 1;
        score += 10;
        update_score();
        
        if (snake_length < MAX_SNAKE_LENGTH) {
            snake_length++;
        }
        
        generate_food();
    }
    
    /* Erase tail if didn't eat food */
    if (!ate_food) {
        draw_at(snake[snake_length - 1].x, snake[snake_length - 1].y, ' ');
    }
    
    /* Move snake body */
    for (i = snake_length - 1; i > 0; i--) {
        snake[i].x = snake[i - 1].x;
        snake[i].y = snake[i - 1].y;
    }
    
    /* Update head position */
    snake[0].x = new_x;
    snake[0].y = new_y;
}

/* Process input */
void process_input(void) {
    char c = keyboard_get_char();
    
    if (c != 0) {
        switch (c) {
            case 'w':
            case 'W':
                next_direction = DIR_UP;
                break;
            case 's':
            case 'S':
                next_direction = DIR_DOWN;
                break;
            case 'a':
            case 'A':
                next_direction = DIR_LEFT;
                break;
            case 'd':
            case 'D':
                next_direction = DIR_RIGHT;
                break;
            case 'q':
            case 'Q':
                game_over = 1;
                break;
        }
    }
}

/* Main game function */
void snake_game(void) {
    init_game();
    
    /* Game loop */
    while (!game_over) {
        /* Process input */
        process_input();
        
        /* Update game state */
        update_game();
        
        /* Draw everything */
        draw_snake();
        draw_food();
        
        /* Delay for game speed */
        delay(50);
    }
    
    /* Game over screen */
    fb_clear();
    fb_puts("GAME OVER!\n\n");
    fb_puts("Final Score: ");
    
    char score_str[20];
    int_to_str(score, score_str);
    fb_puts(score_str);
    fb_puts("\n\nPress any key to return to shell...\n");
    
    /* Wait for key press */
    while (keyboard_get_char() == 0) {
        /* Wait */
    }
}