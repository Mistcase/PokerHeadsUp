#ifndef State_hpp
#define State_hpp

#include <stack>

#include <Types.h>
#include <Fonts.h>

class State;
typedef std::stack<State*> StatesStack;

class State : public sf::Drawable
{
public:
    virtual ~State();
    
    virtual void update(float deltaTime, const Vector2f& mousePos) = 0;
    virtual void updateSfmlEvent(sf::Event& ev) = 0;

    bool requestsExit() const;
    void exitState();
    
private:
    bool active = true;

protected: 
    sf::RenderWindow* window;
    sf::Text* debugText;
    StatesStack* statesStack;
};

#endif
