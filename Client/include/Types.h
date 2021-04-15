#pragma once
#ifndef _TYPES_H
#define _TYPES_H

#include "SfmlGui.h"

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <map>
#include <array>
#include <utility>
#include <memory>

using namespace sfml_gui;

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

typedef unsigned int Balance;
typedef unsigned int Bet;
typedef std::string AnsiString;

#endif