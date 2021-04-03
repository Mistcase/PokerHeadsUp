#ifndef State_hpp
#define State_hpp

#include <SFML/Graphics.hpp>
#include <stack>
#include <string>

#include "Fonts.h"

class State : public sf::Drawable
{
public:
    virtual ~State();
    
    virtual void update(float deltaTime, sf::Vector2f mousePos) = 0;
    virtual void updateSfmlEvent(sf::Event& ev) = 0;
    
protected: 
    sf::RenderWindow* window;
    sf::Text* debugText;
    std::stack<State*>* statesStack;
};

typedef std::stack<State*> StatesStack;

#endif
