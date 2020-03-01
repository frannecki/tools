#include <time.h>
#include <vector>
#include <string>
#include <random>
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
	float timer;
	Position pos;
	chariot(int x, int y, int o = 0, int t = 0) : orient(o), pos(x, y), type(t), timer(0) {};
};

struct gt {
	int alive;
	Position pos;
	gt(int a = 1, int x = 0, int y = 0) : alive(a), pos(x, y) {};
};


const int len = 10;
//int x = 2, y = 9;
int field[len][len] = { 0 };

// horizontal and vertical coordinate change
const int mov_h[4] = {0, 1, 0, -1};
const int mov_v[4] = {-1, 0, 1, 0};

// offset caused by rotation
const int offset_h[4] = {0, -1, -1, 0};
const int offset_v[4] = {0, 0, -1, -1};


bool check(int x, int y) {
	if (x < 0 || x >= len || y < 0 || y >= len)  return false;
    else  return field[y][x]<=1;
}

int generatePos() {
	int grid = std::rand() % (len * len);
	int row = grid / 10;
	int col = grid % 10;
	while (field[row][col] > 0) {
		grid = std::rand() % (len * len);
		row = grid / 10;
		col = grid % 10;
	}
	return grid;
}

void generateMap() {
	int fieldblocks[10][10] = {
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
		{0, 1, 1, 0, 0, 2, 3, 3, 0, 0},
		{0, 1, 1, 0, 0, 3, 3, 3, 0, 0},
		{0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
		{0, 1, 3, 3, 3, 0, 2, 2, 2, 0},
		{0, 1, 3, 3, 2, 0, 2, 2, 2, 0},
		{0, 1, 1, 0, 0, 0, 0, 0, 0, 0}, 
		{0, 0, 0, 2, 2, 2, 2, 0, 0, 0}, 
		{0, 0, 0, 2, 4, 4, 2, 0, 0, 0}
	};
	for (int i = 0; i < len; ++i) {
		for (int j = 0; j < len; ++j) {
			field[i][j] = fieldblocks[i][j];
		}
	}
}


void updateVehicleStatus(chariot &ch, int turn, int dx, int dy) {
	int nx = ch.pos.x, ny = ch.pos.y;
	ch.pos.x += dx;  ch.pos.y += dy;
	if (!check(ch.pos.x, ch.pos.y)) {
		ch.pos.x = nx;  ch.pos.y = ny;
	}
	if (field[ny][nx] > 1)  field[ny][nx] = 0;
	if (field[ch.pos.y][ch.pos.x] != 1)  field[ch.pos.y][ch.pos.x] = 6 + ch.type;
	ch.orient = turn;
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
		if (i == 4) {
			blocks[i].setScale(.03125, .03125);  // bullets 16 * 16
		}
	}

	for (int i = 0; i < 2; ++i) {
		field[9][4+i] = 4;  // targets
	}

	sf::Clock clock;
	float delay = 0.1, ally_enemy_delay = 1.0;
	bool pause = false, gameover = false;

	int dx = 0, dy = 0, orient = 0, turn = 0, life = 2;
	int ally_num = 2, enemy_num = 5;

	std::vector<bullet> shells;
	std::vector<chariot> allies, enemies;

	chariot player(2, 9);

	for (int i = 0; i < ally_num; ++i) {
		int grid = generatePos();
		int row = grid / 10, col = grid % 10;
		if(field[row][col]==0)  field[row][col] = 7;
		allies.push_back(chariot(col, row, std::rand() % 4, 1));
	}

	for (int i = 0; i < enemy_num; ++i) {
		int grid = generatePos();
		int row = grid / 10, col = grid % 10;
		if(field[row][col]==0)  field[row][col] = 8;
		enemies.push_back(chariot(col, row, std::rand() % 4, 2));
	}

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
				case(sf::Keyboard::J):  shells.push_back(bullet(player.orient, player.pos.x, player.pos.y)); break;
				default: break;
				}
			}
		}

		if (pause)  continue;

		/// motion of prime force vehicle
		updateVehicleStatus(player, turn, dx, dy);

		/// motion of allies
		for (int i = 0; i < allies.size(); ++i) {
			allies[i].timer += time;
			if (allies[i].timer > ally_enemy_delay) {
				allies[i].timer = 0;
			}
			else  continue;
			int operation = std::rand() % 5;
			if (operation == 4) {
				shells.push_back(bullet(allies[i].orient, allies[i].pos.x, allies[i].pos.y, 1));
			}
			else {
				int turn_orient = operation;
				dx = mov_h[turn_orient];
				dy = mov_v[turn_orient];
				updateVehicleStatus(allies[i], turn_orient, dx, dy);
			}
		}

		/// motion of enemies
		for (int i = 0; i < enemies.size(); ++i) {
			enemies[i].timer += time;
			if (enemies[i].timer > ally_enemy_delay) {
				enemies[i].timer = 0;
			}
			else {
				continue;
			}
			int operation = std::rand() % 5;
			if (operation == 4) {
				shells.push_back(bullet(enemies[i].orient, enemies[i].pos.x, enemies[i].pos.y, 2));
			}
			else {
				int turn_orient = operation;
				dx = mov_h[turn_orient];
				dy = mov_v[turn_orient];
				updateVehicleStatus(enemies[i], turn_orient, dx, dy);
			}
		}


		/// check field blocks hit by bullets
		for (int i = 0; i < shells.size();) {
			shells[i].timer += time;
			if (shells[i].timer >= delay) {
				shells[i].timer = 0;
				if (field[shells[i].pos.y][shells[i].pos.x] != 1) {
					field[shells[i].pos.y][shells[i].pos.x] = 0;  
					// bullet left current position
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
								else  field[py][px] = grid_status;
							}
							else if (grid_status == 6 || grid_status == 7) {  
								// vehicle hit by vehicle
								if (shells[i].origin == 2) {
									if(grid_status==6)  gameover = true;
									else {
										for (int k = 0; k < allies.size();) {
											if (allies[k].pos == shells[i].pos) {
												allies[k] = allies[(int)allies.size()-1];
												allies.pop_back();
											}
											else  ++k;
										}
									}
								}
								else  field[py][px] = grid_status;
							}
							else if (grid_status == 8) {  // bullet hit enemy vehicle
								if (shells[i].origin <= 1) {
									for (int k = 0; k < enemies.size();) {
										if (enemies[k].pos == shells[i].pos) {
											enemies[k] = enemies[(int)enemies.size() - 1];
											enemies.pop_back();
										}
										else  ++k;
									}
								}
								else  field[py][px] = grid_status;
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
		for (int i = 0; i < shells.size();) {
			bool collided = false;
			for (int j = i + 1; j < shells.size(); ++j) {
				if (shells[i].pos == shells[j].pos && 
					abs(shells[i].orient - shells[j].orient) == 2)
				{
					collided = true;
					shells[j] = shells[(int)shells.size()-1];
					shells.pop_back();
					shells[i] = shells[(int)shells.size() - 1];
					shells.pop_back();
					break;
				}
			}
			if(!collided)  ++i;
		}

		window.clear(sf::Color::White);
		window.draw(background);

		/// draw blocks
		for (int i = 0; i < len; ++i) {
			for (int j = 0; j < len; ++j) {
				int status = field[i][j];
				if (status > 0 && status < 6) {
					if (status==5) {
						blocks[4].setPosition(64 * j + 24, 64 * i + 24);
					}
					else  blocks[status-1].setPosition(64 * j, 64 * i);
					window.draw(blocks[status - 1]);
				}
			}
		}

		if (field[player.pos.y][player.pos.x] != 1) {
			blocks[5].setRotation(90 * player.orient);
			blocks[5].setPosition(64 * (player.pos.x - offset_h[player.orient]),
				64 * (player.pos.y - offset_v[player.orient]));
			window.draw(blocks[5]);;
		}

		for (auto ally : allies) {
			if (field[ally.pos.y][ally.pos.x] == 1)  continue;
			blocks[6].setRotation(90 * ally.orient);
			blocks[6].setPosition(64 * (ally.pos.x - offset_h[ally.orient]),
				64 * (ally.pos.y - offset_v[ally.orient]));
			window.draw(blocks[6]);
		}
		for (auto enemy : enemies) {
			if (field[enemy.pos.y][enemy.pos.x] == 1)  continue;
			blocks[7].setRotation(90 * enemy.orient);
			blocks[7].setPosition(64 * (enemy.pos.x - offset_h[enemy.orient]),
				64 * (enemy.pos.y - offset_v[enemy.orient]));
			window.draw(blocks[7]);
		}
		window.display();
		dx = dy = 0;
	}

	sf::Font font;
	font.loadFromFile("./fonts/ALGER.TTF");
	sf::Text text("GAMEOVER", font, 80);
	text.setPosition(100, 200);
	text.setColor(sf::Color::Cyan);

	while (window.isOpen()) {
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed || e.type == sf::Event::KeyPressed) {
				window.close();
			}
		}
		window.clear(sf::Color::White);
		window.draw(background);
		window.draw(text);
		window.display();
	}
}