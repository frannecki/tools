#include <time.h>
#include <vector>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>

struct Position {
	int x, y;
	Position(int x_, int y_) : x(x_), y(y_) {};
	friend bool operator == (const Position& p1, const Position& p2) {
		return p1.x == p2.x && p1.y == p2.y;
	}
};

struct bullet {
	int origin;
	int orient;
	float timer;
	Position pos;
	bullet(int o, int x, int y, int ori = 0) : orient(o), pos(x, y), timer(0), origin(ori) {};
};

struct chariot {
	int type;
	int orient;
	Position pos;
	chariot(int o, int x, int y) : orient(o), pos(x, y) {};
};

struct gt {
	int alive;
	Position pos;
	gt(int a = 1, int x = 0, int y = 0) : alive(a), pos(x, y) {};
};

const float velocity[5] = { .00, .10, .15, .20, .25 };

const int len = 10;
int x = 2, y = 9;
int field[len][len] = { 0 };

// horizontal and vertical coordinate change
const int mov_h[4] = {0, 1, 0, -1};
const int mov_v[4] = {-1, 0, 1, 0};

// offset caused by rotation
const int offset_h[4] = {0, -1, -1, 0};
const int offset_v[4] = {0, 0, -1, -1};

bool check() {
	if (x < 0 || x >= len || y < 0 || y >= len)  return false;
    else  return field[y][x]<=1;
}

void generateMap() {
	for (int i = 0; i < len; ++i) {
		field[1][i] = 3;
		field[4][i] = 2;
		field[6][i] = 1;
	}
}

void main() {
	
	generateMap();
	
	sf::RenderWindow window(sf::VideoMode(640, 640), "Desperate Strike");
	sf::Texture bg, items[8];
	std::string filenames[] = {"tree.png", "wooden.png", "crate.png", "target.png", 
		"bullet.png", "tanks/tank1.png", "tanks/tank3.png", "tanks/tank4.png"};
	/// 8 items are tree, wooden crate, crate, target, shell, self, ally and enemy, respectively.
	for (int i = 0; i < 8; ++i) {
		items[i].loadFromFile("./images/" + filenames[i]);
	}
	bg.loadFromFile("./images/bg.png");

	sf::Sprite background(bg);
	sf::Sprite blocks[8];
	for (int i = 0; i < 8; ++i) {
		blocks[i] = sf::Sprite(items[i]);
		blocks[i].setScale(.125, .125);
		if (i == 4)  blocks[i].setScale(.03125, .03125);  // bullets 16 * 16
	}

	for (int i = 0; i < 2; ++i) {
		field[9][4+i] = 4;  // targets
	}

	sf::Clock clock;
	float delay = 0.1;
	bool pause = false, gameover = false;

	int dx = 0, dy = 0, orient = 0, turn = 0, life = 2;

	std::vector<bullet> shells;
	std::vector<chariot> allies, enemies;

	while (window.isOpen()) {
		if (gameover)  break;
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		sf::Event e;
		while (window.pollEvent(e)){
			if (e.type == sf::Event::Closed) {
				window.close();
			}
			else if (e.type == sf::Event::KeyPressed) {
				switch (e.key.code) {
				case(sf::Keyboard::Escape):  return;
				case(sf::Keyboard::P):  pause = !pause; break;
				case(sf::Keyboard::W):  turn = 0; dy -= 1; break;
				case(sf::Keyboard::S):  turn = 2; dy += 1; break;
				case(sf::Keyboard::A):  turn = 3; dx -= 1; break;
				case(sf::Keyboard::D):  turn = 1; dx += 1; break;
				case(sf::Keyboard::J):  shells.push_back(bullet(orient, x, y)); break;
				default: break;
				}
			}
		}

		if (pause)  continue;

		/// motion of vehicle
		if (turn != orient) {
			int diff = turn - orient;
			if (diff < 0)  diff += 4;
			blocks[5].rotate(90 * diff);
			orient = turn;
		}
		int nx = x, ny = y;
		x += dx;  y += dy;
		if (!check()) {
			x = nx;  y = ny;
		}
		if (field[ny][nx] != 1)  field[ny][nx] = 0;
		if(field[y][x] != 1)  field[y][x] = 6;


		/// check field blocks hit by bullets
		for (int i = 0; i < shells.size();) {
			shells[i].timer += time;
			if (shells[i].timer >= delay) {
				shells[i].timer = 0;
				if (field[shells[i].pos.y][shells[i].pos.x] != 1) {
					field[shells[i].pos.y][shells[i].pos.x] = 0;  // bullet left current position
				}
				shells[i].pos.x += mov_h[shells[i].orient];
				shells[i].pos.y += mov_v[shells[i].orient];
				int px = shells[i].pos.x;
				int py = shells[i].pos.y;
				if (px < 0 || px >= len || py < 0 || py >= len || field[py][px] >= 2)
				{
					if (px >= 0 && px < len && py >= 0 && py < len) {
						int grid_status = field[py][px];
						if (grid_status != 3) {
							field[py][px] = 0;
							if (grid_status == 4) {  // bullet hit target
								if (shells[i].origin == 2 || shells[i].origin == 0) {
									if(--life<=0)  gameover = true;
								}
							}
							else if (grid_status == 6 || grid_status == 7) {  // bullet hit vehicle
								if (shells[i].origin == 2) {
									if(grid_status==6)  gameover = true;
									else {
										//for()
									}
								}
							}
							else if (grid_status == 8) {  // bullet hit enemy vehicle
								if (shells[i].origin <= 1) {
									// for()
								}
							}
						}
					}
					shells[i] = shells[(int)shells.size() - 1];
					shells.pop_back();
				}
				else {
					if (field[py][px] == 0) {
						field[py][px] = 5;
					}
					++i;
				}
			}
			else  ++i;
		}
		
		/// check for bullet collions
		for (int i = 0; i < shells.size(); ++i) {
			for (int j = i + 1; j < shells.size(); ++j) {
				if (shells[i].pos == shells[j].pos && abs(shells[i].orient - shells[j].orient) == 2)
				{
					shells[j] = shells[(int)shells.size()-1];
					shells.pop_back();
					shells[i] = shells[(int)shells.size() - 1];
					shells.pop_back();
					break;
				}
			}
		}

		window.clear(sf::Color::White);
		window.draw(background);

		/// draw blocks
		for (int i = 0; i < len; ++i) {
			for (int j = 0; j < len; ++j) {
				int status = field[i][j];
				if (status > 0) {
					if (status == 5) {
						blocks[4].setPosition(64 * j + 24, 64 * i + 24);
					}
					else if (status == 6) {
						blocks[5].setPosition(64 * (x - offset_h[orient]), 64 * (y - offset_v[orient]));;
					}
					else  blocks[status-1].setPosition(64 * j, 64 * i);
					window.draw(blocks[field[i][j] - 1]);
				}
			}
		}
		window.display();
		dx = dy = 0;
	}

	sf::Font font;
	font.loadFromFile("./fonts/ALGER.TTF");
	sf::Text text("GAMEOVER", font, 80);
	text.setPosition(100, 200);

	while (window.isOpen()) {
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
		}
		window.clear(sf::Color::White);
		window.draw(background);
		window.draw(text);
		window.display();
	}
}