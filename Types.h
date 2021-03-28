#pragma once
#ifndef _TYPES_H
#define _TYPES_H

#include <vector>
#include <map>
#include <array>
#include <utility>

using std::vector;
using std::map;
using std::array;
using std::pair;

class Card;

typedef pair<Card, Card> PlayerHand;
typedef array<Card, 5> BoardCards;

#define CardsContainer(containerType) containerType<Card>

const unsigned int STRAIGHT_POSSIBLE_VARIATIONS_COUNT = 10;
const unsigned int STRAIGHT_CARDS_COUNT = 5;

#endif