#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <ctime>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

float delay = 0.2;
int H = 30, V = 20;
int size = 16;
int w = size*50;
int h = size*20;
int score = 0;
int di = 4, n = 1, d = 1, num = 1, k = 1;
bool play = true;
SoundBuffer music;
Sound m(music);


struct Snake
{
    int x,y;
}  s[100];

struct Food
{
    int x,y;
} f;

void Tick()
{
    // chiều dài
    if(s[0].x<30 || s[0].x>0 || s[0].y >0 || s[0].y<20)
    {
        for (int i = num; i>0 ; --i)
        {
            s[i].x=s[i-1].x;
            s[i].y=s[i-1].y;
        }
    }

    // hướng di chuyển
    if (di == 0) s[0].y += 1;
    if (di == 1) s[0].x -= 1;
    if (di == 2) s[0].x += 1;
    if (di == 3) s[0].y -= 1;
    if (di == 4) s[0].x = 0, s[0].y =0;

    // ăn mồi
    if ((s[0].x == f.x) && (s[0].y == f.y))
    {
        if(delay > 0.05) delay -=0.005;
        else delay = 0.05;
        m.play();
        score += 10;
        num++;
        f.x = rand() % H;
        f.y = rand() % V;
    }
}

bool Lost1()
{
    int con = 1;
    for (int i = 1; i < num; i++)
        if (s[0].x == s[i].x && s[0].y == s[i].y)
        {
            con = 0;
        }
    if(con == 0) return true;
    else return false;
}

bool Lost2()
{
    if(s[0].x > 29 || s[0].x < 0 || s[0].y < 0 || s[0].y > 19)
    {
        return true;
    }
    else
        return false;
}

int main()
{
    srand(time(0));

    RenderWindow game(VideoMode(w, h), "Snake Game!");

    SoundBuffer music1, music2;
    music2.loadFromFile("punch.wav");
    music1.loadFromFile("musicbkg.wav");
    Sound m1(music1), m2(music2);
    m1.setLoop(true);
    m1.play();

    music.loadFromFile("swoosh.wav");

    Texture t1, t2, t3, t4, t5, t6, t7;
    t1.loadFromFile("bkg2.png");
    t2.loadFromFile("blue.png");
    t3.loadFromFile("apple.png");
    t5.loadFromFile("head.png");
    t4.loadFromFile("bkgame.png");
    t6.loadFromFile("menu1.png");
    t7.loadFromFile("menu2.png");

    Sprite sprite1(t1);
    Sprite sprite2(t2);
    Sprite sprite3(t3);
    Sprite sprite4(t4);
    Sprite sprite5(t5);
    Sprite sprite6(t6);
    Sprite sprite7(t7);

    Clock clock;
    float timer=0;

    f.x = rand() % H;
    f.y = rand() % V;
    while(game.isOpen())
    {
        Event e;
        while (game.pollEvent(e))
        {
            if ((Keyboard::isKeyPressed(Keyboard::Escape)) || e.type == Event::Closed)
                game.close();
        }

        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer+=time;

        if(k == 1 || k ==0)
        {
            if ((Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) && n == 1 ) di = 1, n = 0, d = 1;
            if ((Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) && n == 1 ) di = 2, n = 0, d = 1;
            if ((Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) && d == 1 ) di = 3, d = 0, n = 1;
            if ((Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) && d == 1 ) di = 0, d = 0, n = 1;
        }

        if (timer > delay)
        {
            timer = 0;
            Tick();
        }

        if (Lost1() == true || Lost2() == true)
        {
            m2.play();
            n = 1, d = 1;
            num = 1;
            s[0].x = 0;
            s[0].y = 0;
            k = 2;
            di = 4;
        }

        game.clear();

        // vẽ map và rắn
        sprite1.setPosition(480,0);
        game.draw(sprite1);
        sprite4.setPosition(0,0);
        game.draw(sprite4);

        for (int i = 0; i < num; i++)
        {
            if(i == 0)
            {
                sprite5.setPosition(s[i].x*size, s[i].y*size);
                game.draw(sprite5);
            }
            else
            {
                sprite2.setPosition(s[i].x*size, s[i].y*size);
                game.draw(sprite2);
            }
        }
        // vẽ đồ ăn
        sprite3.setPosition(f.x*size,  f.y*size);
        game.draw(sprite3);

        Font font;
        Text text1;
        font.loadFromFile("VHMEMO.ttf");
        text1.setFont(font);
        text1.setString("Snake Game!");
        text1.setCharacterSize(30);
        text1.setColor(Color::Black);
        text1.setPosition(500,16);
        game.draw(text1);

        string s="Score :"+to_string(score);
        Text text;
        text.setFont(font);
        text.setString(s);
        text.setCharacterSize(20);
        text.setColor(Color::Black);
        text.setPosition(500,60);
        game.draw(text);

        //menu
        if(k == 1)
        {
            sprite6.setPosition(0,0);
            game.draw(sprite6);

            if(Mouse::isButtonPressed(Mouse::Left) && Mouse::getPosition(game).x > 220 && Mouse::getPosition(game).x < 450
                    && Mouse::getPosition(game).y > 185 && Mouse::getPosition(game).y < 255)
            {
                game.close();
            }

            if(Mouse::isButtonPressed(Mouse::Left) && Mouse::getPosition(game).x > 220 && Mouse::getPosition(game).x < 450
                    && Mouse::getPosition(game).y > 75 && Mouse::getPosition(game).y < 150)
            {
                k = 0;
            }
        }
        if(k == 2)
        {
            sprite7.setPosition(0,0);
            game.draw(sprite7);
            text1.setPosition(500,80);
            game.draw(text1);
            text.setPosition(500,117);
            game.draw(text);

            if(Mouse::isButtonPressed(Mouse::Left) && Mouse::getPosition(game).x > 192 && Mouse::getPosition(game).x < 470
                    && Mouse::getPosition(game).y > 174 && Mouse::getPosition(game).y < 241)
            {
                game.close();
            }

            if(Mouse::isButtonPressed(Mouse::Left) && Mouse::getPosition(game).x > 192 && Mouse::getPosition(game).x < 470
                    && Mouse::getPosition(game).y > 68 && Mouse::getPosition(game).y < 135)
            {
                num = 1;
                score = 0;
                k = 0;
            }

        }

        game.display();
    }

    return 0;
}

