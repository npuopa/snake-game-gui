#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

static bool allowMove = false;
Color green = { 169,216,81,255 };
Color darkGreen = { 163,208,74,255 };
Color blue = { 91,123,249,255 };

const int cellSize = 30;
const int cellCount = 17;
const int offset = 55;

double lastUpdateTime = 0;

bool ElementInDeque(Vector2 element, deque<Vector2> deque)
{
    for (unsigned int i = 0; i < deque.size(); i++)
    {
        if (Vector2Equals(deque[i], element))
        {
            return true;
        }
    }
    return false;
}

bool EventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Snake
{
public:
    deque<Vector2> body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };
    Vector2 direction = { 1, 0 };
    bool addSegment = false;
    Texture2D head_up, head_down, head_right, head_left;
    Texture2D tail_up, tail_down, tail_right, tail_left;
    Texture2D body_bottomleft, body_bottomright, body_topleft, body_topright;
    Texture2D body_horizontal, body_vertical;

    enum Directions {LEFT, RIGHT, DOWN, UP};

    Snake() {
        Image Im = LoadImage("D:\\Projects\\Raylib_snake\\Graphics\\head_right.png");
        head_right = LoadTextureFromImage(Im);
        UnloadImage(Im);

        Im = LoadImage("D:\\Projects\\Raylib_snake\\Graphics\\head_left.png");
        head_left = LoadTextureFromImage(Im);
        UnloadImage(Im);

        Im = LoadImage("D:\\Projects\\Raylib_snake\\Graphics\\head_up.png");
        head_up = LoadTextureFromImage(Im);
        UnloadImage(Im);

        Im = LoadImage("D:\\Projects\\Raylib_snake\\Graphics\\head_down.png");
        head_down = LoadTextureFromImage(Im);
        UnloadImage(Im);

        Im = LoadImage("D:\\Projects\\Raylib_snake\\Graphics\\tail_up.png");
        tail_down = LoadTextureFromImage(Im);
        UnloadImage(Im);

        Im = LoadImage("D:\\Projects\\Raylib_snake\\Graphics\\tail_down.png");
        tail_up = LoadTextureFromImage(Im);
        UnloadImage(Im);

        Im = LoadImage("D:\\Projects\\Raylib_snake\\Graphics\\tail_left.png");
        tail_right = LoadTextureFromImage(Im);
        UnloadImage(Im);

        Im = LoadImage("D:\\Projects\\Raylib_snake\\Graphics\\tail_right.png");
        tail_left = LoadTextureFromImage(Im);
        UnloadImage(Im);

        Im = LoadImage("D:\\Projects\\Raylib_snake\\Graphics\\body_topright.png");
        body_topright = LoadTextureFromImage(Im);
        UnloadImage(Im);

        Im = LoadImage("D:\\Projects\\Raylib_snake\\Graphics\\body_topleft.png");
        body_topleft = LoadTextureFromImage(Im);
        UnloadImage(Im);

        Im = LoadImage("D:\\Projects\\Raylib_snake\\Graphics\\body_bottomright.png");
        body_bottomright = LoadTextureFromImage(Im);
        UnloadImage(Im);

        Im = LoadImage("D:\\Projects\\Raylib_snake\\Graphics\\body_bottomleft.png");
        body_bottomleft = LoadTextureFromImage(Im);
        UnloadImage(Im);

        Im = LoadImage("D:\\Projects\\Raylib_snake\\Graphics\\body_horizontal.png");
        body_horizontal = LoadTextureFromImage(Im);
        UnloadImage(Im);

        Im = LoadImage("D:\\Projects\\Raylib_snake\\Graphics\\body_vertical.png");
        body_vertical = LoadTextureFromImage(Im);
        UnloadImage(Im);
    }

    ~Snake() {
        UnloadTexture(head_up);
        UnloadTexture(head_down);
        UnloadTexture(head_right);
        UnloadTexture(head_left);
        UnloadTexture(tail_up);
        UnloadTexture(tail_down);
        UnloadTexture(tail_right);
        UnloadTexture(tail_left);
        UnloadTexture(body_bottomleft);
        UnloadTexture(body_bottomright);
        UnloadTexture(body_topleft);
        UnloadTexture(body_topright);
        UnloadTexture(body_horizontal);
        UnloadTexture(body_vertical);
    }

    void Draw()
    {
        if (direction.x == 1) { //head to the right
            DrawTexture(head_right, offset + static_cast<int>(body[0].x) * cellSize, offset + static_cast<int>(body[0].y) * cellSize, WHITE);
        }
        else if (direction.x == -1) { //left
            DrawTexture(head_left, offset + static_cast<int>(body[0].x) * cellSize, offset + static_cast<int>(body[0].y) * cellSize, WHITE);
        }
        else if (direction.y == 1) { //down
            DrawTexture(head_down, offset + static_cast<int>(body[0].x) * cellSize, offset + static_cast<int>(body[0].y) * cellSize, WHITE);
        }
        else { //up
            DrawTexture(head_up, offset + static_cast<int>(body[0].x) * cellSize, offset + static_cast<int>(body[0].y) * cellSize, WHITE);
        }

        const size_t n = body.size();

        float xTail = body[n - 1].x, yTail = body[n - 1].y;
        float xBody = body[n - 2].x, yBody = body[n - 2].y;

        Directions dir;

        if (xTail < xBody) { // tail to the right
            DrawTexture(tail_right, offset + static_cast<int>(xTail) * cellSize, offset + static_cast<int>(yTail) * cellSize, WHITE);
            dir = RIGHT;
        }
        else if (xTail > xBody) { // left
            DrawTexture(tail_left, offset + static_cast<int>(xTail) * cellSize, offset + static_cast<int>(yTail) * cellSize, WHITE);
            dir = LEFT;
        }
        else if (yTail < yBody) { //down
            DrawTexture(tail_down, offset + static_cast<int>(xTail) * cellSize, offset + static_cast<int>(yTail) * cellSize, WHITE);
            dir = DOWN;
        }
        else { // up
            DrawTexture(tail_up, offset + static_cast<int>(xTail) * cellSize, offset + static_cast<int>(yTail) * cellSize, WHITE);
            dir = UP;
        }

        for (size_t i = n - 2; i > 0; i--)
        {
            float xPrev = body[i + 1].x, yPrev = body[i + 1].y;
            float xNext = body[i - 1].x, yNext = body[i - 1].y;

            if (yPrev < yNext && xPrev < xNext) {
                if (dir == RIGHT) {
                    DrawTexture(body_bottomleft, offset + static_cast<int>(body[i].x) * cellSize, offset + static_cast<int>(body[i].y) * cellSize, WHITE);
                    dir = DOWN;
                }
                else {
                    DrawTexture(body_topright, offset + static_cast<int>(body[i].x) * cellSize, offset + static_cast<int>(body[i].y) * cellSize, WHITE);
                    dir = RIGHT;
                }
            }
            else if (yPrev > yNext && xPrev > xNext) {
                if (dir == UP) {
                    DrawTexture(body_bottomleft, offset + static_cast<int>(body[i].x) * cellSize, offset + static_cast<int>(body[i].y) * cellSize, WHITE);
                    dir = LEFT;
                }
                else {
                    DrawTexture(body_topright, offset + static_cast<int>(body[i].x) * cellSize, offset + static_cast<int>(body[i].y) * cellSize, WHITE);
                    dir = UP;
                }
            }
            else if (yPrev < yNext && xPrev > xNext) {
                if (dir == DOWN) {
                    DrawTexture(body_topleft, offset + static_cast<int>(body[i].x) * cellSize, offset + static_cast<int>(body[i].y) * cellSize, WHITE);
                    dir = LEFT;
                }
                else {
                    DrawTexture(body_bottomright, offset + static_cast<int>(body[i].x) * cellSize, offset + static_cast<int>(body[i].y) * cellSize, WHITE);
                    dir = DOWN;
                }
            }
            else if (yPrev > yNext && xPrev < xNext) {
                if (dir == UP) {
                    DrawTexture(body_bottomright, offset + static_cast<int>(body[i].x) * cellSize, offset + static_cast<int>(body[i].y) * cellSize, WHITE);
                    dir = RIGHT;
                }
                else {
                    DrawTexture(body_topleft, offset + static_cast<int>(body[i].x) * cellSize, offset + static_cast<int>(body[i].y) * cellSize, WHITE);
                    dir = UP;
                }
            }
            else if(xPrev == xNext) {
                DrawTexture(body_vertical, offset + static_cast<int>(body[i].x) * cellSize, offset + static_cast<int>(body[i].y) * cellSize, WHITE);
            }
            else {
                DrawTexture(body_horizontal, offset + static_cast<int>(body[i].x) * cellSize, offset + static_cast<int>(body[i].y) * cellSize, WHITE);
            }
        }
    }

    void Update()
    {
        body.push_front(Vector2Add(body[0], direction));
        if (addSegment == true)
        {
            addSegment = false;
        }
        else
        {
            body.pop_back();
        }
    }

    void Reset()
    {
        body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };
        direction = { 1, 0 };
    }
};

class Food
{

public:
    Vector2 position;
    Texture2D texture;

    Food(deque<Vector2> snakeBody)
    {
        Image image = LoadImage("D:\\Projects\\Raylib_snake\\Graphics\\apple.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos(snakeBody);
    }

    ~Food()
    {
        UnloadTexture(texture);
    }

    void Draw()
    {
        DrawTexture(texture, offset + static_cast<int>(position.x) * cellSize, offset + static_cast<int>(position.y) * cellSize, WHITE);
        DrawTexture(texture, offset, 10, WHITE);
    }

    Vector2 GenerateRandomCell()
    {
        float x = static_cast<float>(GetRandomValue(0, cellCount - 1));
        float y = static_cast<float>(GetRandomValue(0, cellCount - 1));
        return Vector2{ x, y };
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakeBody)
    {
        Vector2 position = GenerateRandomCell();
        while (ElementInDeque(position, snakeBody))
        {
            position = GenerateRandomCell();
        }
        return position;
    }
};

class Game
{
public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true;
    int score = 0;
    Sound eatSound;
    Sound wallSound;

    Game()
    {
        InitAudioDevice();
        eatSound = LoadSound("D:\\Projects\\Raylib_snake\\Sounds\\eat.mp3");
        wallSound = LoadSound("D:\\Projects\\Raylib_snake\\Sounds\\wall.mp3");
    }

    ~Game()
    {
        UnloadSound(eatSound);
        UnloadSound(wallSound);
        CloseAudioDevice();
    }

    void Draw()
    {
        food.Draw();
        snake.Draw();
    }

    void Update()
    {
        if (running)
        {
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
            CheckCollisionWithTail();
        }
    }

    void CheckCollisionWithFood()
    {
        if (Vector2Equals(snake.body[0], food.position))
        {
            food.position = food.GenerateRandomPos(snake.body);
            snake.addSegment = true;
            score++;
            PlaySound(eatSound);
        }
    }

    void CheckCollisionWithEdges()
    {
        if (snake.body[0].x == cellCount || snake.body[0].x == -1)
        {
            GameOver();
        }
        if (snake.body[0].y == cellCount || snake.body[0].y == -1)
        {
            GameOver();
        }
    }

    void GameOver()
    {
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        running = false;
        score = 0;
        PlaySound(wallSound);
    }

    void CheckCollisionWithTail()
    {
        deque<Vector2> headlessBody = snake.body;
        headlessBody.pop_front();
        if (ElementInDeque(snake.body[0], headlessBody))
        {
            GameOver();
        }
    }
};

int main()
{
    cout << "Starting the game..." << endl;
    InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, "Snake");
    SetTargetFPS(60);

    Game game = Game();

    //HideCursor();

    while (WindowShouldClose() == false)
    {
        BeginDrawing();

        if (EventTriggered(0.1))
        {
            allowMove = true;
            game.Update();
        }

        if (IsKeyPressed(KEY_W) && game.snake.direction.y != 1 && allowMove)
        {
            game.snake.direction = { 0, -1 };
            game.running = true;
            allowMove = false;
        }
        if (IsKeyPressed(KEY_S) && game.snake.direction.y != -1 && allowMove)
        {
            game.snake.direction = { 0, 1 };
            game.running = true;
            allowMove = false;
        }
        if (IsKeyPressed(KEY_A) && game.snake.direction.x != 1 && allowMove)
        {
            game.snake.direction = { -1, 0 };
            game.running = true;
            allowMove = false;
        }
        if (IsKeyPressed(KEY_D) && game.snake.direction.x != -1 && allowMove)
        {
            game.snake.direction = { 1, 0 };
            game.running = true;
            allowMove = false;
        }

        // Drawing
        ClearBackground({86, 139, 52, 255});
        deque<Color> colors = {green, darkGreen};
        for (int i = 0; i < cellCount; ++i) {
            for (int j = 0; j < cellCount; ++j) {
                DrawRectangle(offset + cellSize * i, offset + cellSize * j, cellSize, cellSize, colors.front());
                colors.push_back(colors.front());
                colors.pop_front();
            }
        }
        DrawText(TextFormat("%i", game.score), offset + cellSize * 1 + 15, 18, 30, RAYWHITE);
        game.Draw();

        EndDrawing();
    }
    CloseWindow();
    return 0;
}