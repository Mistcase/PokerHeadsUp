#pragma once
#ifndef _TYPES_H
#define _TYPES_H

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>
#include <map>
#include <array>
#include <utility>
#include <memory>

using std::vector;
using std::map;
using std::array;
using std::pair;
using std::shared_ptr;

using sf::String;
using sf::Vector2f;
using sf::Vector2u;


using sf::Drawable;
using sf::Color;
using sf::RenderTarget;
using sf::RenderStates;
using sf::RenderWindow;

class Card;

typedef pair<Card, Card> PlayerHand;
typedef array<Card, 5> BoardCards;

typedef unsigned int Balance;
typedef std::string AnsiString;

#define CardsContainer(containerType) containerType<Card>

const unsigned int STRAIGHT_POSSIBLE_VARIATIONS_COUNT = 10;
const unsigned int STRAIGHT_CARDS_COUNT = 5;

#endif