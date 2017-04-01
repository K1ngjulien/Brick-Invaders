/*
Brick Invaders, Julian Kandlhofer, March 2017




*/


#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#define Unit 16

// Brick Color rgb(122, 15, 15)

class Brick     //Class that defines the Bricks
{
    sf::RectangleShape rect;
    sf::Font font;
    int hitcount;
    sf::Text txt;

public:

    Brick()
    {
        rect.setFillColor(sf::Color(122,15,15));
        rect.setSize(sf::Vector2f(Unit*2,Unit));
        rect.setOutlineColor(sf::Color::Black);
        rect.setOutlineThickness(1.f);

        if(!font.loadFromFile("verdana.ttf"))
            std::cout<<"Error loading font!\n";

        txt.setFont(font);
        txt.setCharacterSize(10);
        txt.setFillColor(sf::Color::Black);


        std::string buf = std::to_string(hitcount);
        txt.setString(buf);
    }

    int getHitCount()
    {
        return hitcount;
    }

    sf::FloatRect getRect()     //Returns a Floatrect of the Position and size
    {
        return sf::Rect<float>(rect.getPosition(),rect.getSize());
    }

    void setPos(sf::Vector2f pos)   //Sets the position
    {
        rect.setPosition(pos);

        txt.setPosition(rect.getPosition());
        std::cout<<pos.x/Unit<<"\t"<<pos.y/Unit<<std::endl;
    }

    bool checkColl(sf::FloatRect object)    //Checks if something is intersecting the floatrect
    {
        bool ret=object.intersects(getRect());

        if(ret)
        {
            rect.setFillColor(sf::Color::Green);
            hitcount++;
        }
        return ret;
    }

    void draw (sf::RenderWindow &window)    //Draws the object
    {
        std::string buf = std::to_string(hitcount);

        //if(buf!=txt.getString())
        txt.setString(buf);

        window.draw(rect);
        window.draw(txt);
    }
};

class Bullet        //Class to define the Bullets
{
    sf::CircleShape cirk;
    float vel;

public:
    bool hit;

    Bullet()
    {
        vel=0.3f/(Unit/16);
        hit=false;
        cirk.setRadius(Unit/4);
        cirk.setOrigin(cirk.getRadius(),cirk.getRadius());
        cirk.setFillColor(sf::Color(121,121,121));
        cirk.setPosition(sf::Vector2f(Unit*16,Unit*31));
    }

    void setPos(sf::Vector2f pos)
    {
        cirk.setPosition(pos);
        //std::cout<<pos.x/Unit<<"\t"<<pos.y/Unit<<std::endl;
    }

    sf::FloatRect getRect()
    {
        return cirk.getGlobalBounds();
    }

    bool update()           //Returns if the Object is offscreen
    {
        sf::Vector2f pos;
        pos=cirk.getPosition();
        pos.y-=vel;
        setPos(pos);
        return (cirk.getPosition().y<0)||(hit);


    }

    void draw (sf::RenderWindow &window)
    {


        window.draw(cirk);


    }
};

class Player
{
    sf::CircleShape triang;
    int dir;        //-1=Left, 0= Stationary, 1=Right

public:

    Player()
    {
        triang.setPointCount(3);
        triang.setRadius(2*Unit);
        triang.setOrigin(triang.getRadius(),triang.getRadius());
        triang.setOutlineColor(sf::Color::Black);
        triang.setOutlineThickness(3.f);
        triang.setFillColor(sf::Color::Green);

        triang.setPosition(Unit*16,Unit*30);

        dir=0;
    }

    sf::Vector2f getPos()
    {
        return triang.getPosition();
    }

    void setDir(int dir)
    {
        this->dir=dir;
    }

    void update()
    {
        float vel=Unit/8;
        sf::Vector2f buf(triang.getPosition());
        buf.x+=vel*dir;
        if((buf.x>Unit)&&(buf.x<31*Unit))
            triang.setPosition(buf);
        dir=0;
    }

    void draw (sf::RenderWindow &window)
    {
        update();
        window.draw(triang);
    }
};


int main()
{
    sf::RenderWindow window(sf::VideoMode(Unit*32,Unit*32),"Test",sf::Style::Titlebar | sf::Style::Close);
    std::vector <Brick> bricks(32);
    std::vector <Bullet> bullets;
    Player player;

    sf::Vector2f playerpos(Unit*16,Unit*31);

    //bull.setPos(sf::Vector2f(Unit*16,Unit*31));

    sf::Clock timer;
    timer.restart();

    sf::Time lastBullet;
    lastBullet.Zero;



    int j=0;//Vertical
    int k=0;//Horizontal
    for(int i=0; i<bricks.size(); i++)
    {
        int x=4*k*Unit+Unit;
        int y=2*j*Unit+Unit;
        k++;

        if(x/Unit>=29)
        {
            j++;
            k=0;

        }

        bricks.at(i).setPos(sf::Vector2f(x,y));
    }

    while(window.isOpen())
    {
        window.clear(sf::Color(240, 240, 240));

        sf::Event evnt;

        while(window.pollEvent(evnt))
        {
            switch(evnt.type)
            {
            case sf::Event::Closed:
            {
                window.close();
                break;
            }


            }
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            player.setDir(-1);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            player.setDir(1);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            player.setDir(0);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        {
            if(timer.getElapsedTime().asMilliseconds()-lastBullet.asMilliseconds()>500)
            {

                bullets.resize(bullets.size()+1);
                bullets.at(bullets.size()-1).setPos(player.getPos());

                lastBullet=timer.getElapsedTime();
            }
        }

        for(int i=bricks.size()-1; i>=0 ; i--)
        {
            for(int j=0; j<bullets.size(); j++)
            {
                if(bricks.at(i).checkColl(bullets.at(j).getRect()))
                    bullets.at(j).hit=true;
            }
            if(bricks.at(i).getHitCount()>=5)
            {
                bricks.erase (bricks.begin()+i);
                std::cout<<"Erased a brick\n ";

            }
            else
                bricks.at(i).draw(window);
        }

        for(int i=bullets.size()-1; i>0 ; i--)
        {
            if(bullets.at(i).update())
            {
                bullets.erase (bullets.begin()+i);
                std::cout<<"Erased: "<<bullets.size()<<std::endl;
            }
            else
            {
                bullets.at(i).draw(window);
            }


        }
        player.draw(window);

        window.display();
    }
}
