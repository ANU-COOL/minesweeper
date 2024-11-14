#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>

using namespace sf;

int main() {
    srand(time(0));

    RenderWindow app(VideoMode(400, 400), "Start The Minesweeper");

    int w = 32;  // Tile width
    int grid[12][12] = { 0 };  // Game grid (0 - empty, 9 - mine)
    int sgrid[12][12] = { 10 }; // Showing grid (10 - hidden, 11 - flagged, 0-8 numbers)

    // Load texture for tiles
    Texture t;
    if (!t.loadFromFile("images/tiles.jpg")) {
        std::cout << "Error loading texture!" << std::endl;
        return -1;
    }
    Sprite s(t);

    // Generate mines and numbers
    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= 10; j++) {

            sgrid[i][j] = 10; //initial texture

            // Randomly place mines (9 represents a mine)

            if (rand() % 5 == 0) {
                grid[i][j] = 9;
            }
            else {
                grid[i][j] = 0;
            }
        }
    }

    // Calculate the numbers for each cell based on neighboring mines
    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= 10; j++) {
            if (grid[i][j] == 9) continue;  // Skip mines

            int n = 0;
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue;
                    if (grid[i + dx][j + dy] == 9) {
                        n++;
                    }
                }
            }
            grid[i][j] = n;
        }
    }
    int score = 0;

    // Main game loop
    while (app.isOpen()) {
        Vector2i pos = Mouse::getPosition(app);
        int x = pos.x / w;
        int y = pos.y / w;

        Event e;
        while (app.pollEvent(e)) {
            if (e.type == Event::Closed) {
                app.close();
            }

            if (e.type == Event::MouseButtonPressed) {
                if (e.mouseButton.button == Mouse::Left) {
                    if (grid[x][y] == 9) {
                        sgrid[x][y] = 9; // If it's a mine, reveal it
                        std::cout << "Game Over!" << std::endl;
                        std::cout << "youre score - " << score<< std::endl;
                        // Here you can add logic for game over
                    }
                    else {
                        sgrid[x][y] = grid[x][y]; // Reveal number
                        score += grid[x][y];
                    }
                }
                else if (e.mouseButton.button == Mouse::Right) {
                    // Flagging a tile (right-click)
                    sgrid[x][y] = 11; // Flag tile
                }
            }
        }

        app.clear(Color::White);
        for (int i = 1; i <= 10; i++)
            for (int j = 1; j <= 10; j++) {

                if (sgrid[x][y] == 9) sgrid[i][j] = grid[i][j];

                s.setTextureRect(IntRect(sgrid[i][j] * w, 0, w, w));
                s.setPosition(i * w, j * w);
                app.draw(s);
            }

        app.display();
    }

    return 0;
}
