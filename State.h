#ifndef State_hpp
#define State_hpp

#include <stack>
#include <string>

#include "Types.h"
#include "Fonts.h"

class State : public sf::Drawable
{
public:
    virtual ~State();
    
    virtual void update(float deltaTime, const Vector2f& mousePos) = 0;
    virtual void updateSfmlEvent(sf::Event& ev) = 0;
    
protected: 
    sf::RenderWindow* window;
    sf::Text* debugText;
    std::stack<State*>* statesStack;
};

typedef std::stack<State*> StatesStack;

#endif
