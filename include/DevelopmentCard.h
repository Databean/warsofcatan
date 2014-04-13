//
//  DevelopmentCard.h
//  Catan parts
//
//  Created by Ankit Bhutani on 2/22/14.
//  Copyright (c) 2014 Ankit Bhutani. All rights reserved.
//

#ifndef Catan_parts_DevelopmentCard_h
#define Catan_parts_DevelopmentCard_h


#include "Util.h"
#include "Player.h"
#include "GameVisitor.h"

#include <stdexcept>


enum DevCardType { KNIGHT, VICTORYPOINT, YEAROFPLENTY, MONOPOLY, ROADBUILDING };


/**
 * A card which can be held in a player's hand and be used to perform an action.
 */
class DevelopmentCard {

private:
    DevCardType type;
public:
    DevelopmentCard() {}
    virtual ~DevelopmentCard();

    virtual DevCardType getType() const = 0;

	virtual void accept(GameVisitor& visitor);
	virtual bool operator==(const DevelopmentCard&);
};

/**
 * A development card used to move the robber and take a resource from another player.
 */
class KnightCard : public DevelopmentCard {
private:

public:
	KnightCard() {}
    virtual DevCardType getType() const;
};

/**
 * A development card that gives a permanent victory point on usage.
 */
class VictoryPointCard : public DevelopmentCard {
public:
    VictoryPointCard() {}
    virtual DevCardType getType() const;
};

/**
 * A development card used to retrieve two resources of any type from the bank.
 */
class YearOfPlentyCard : public DevelopmentCard {
public:
    YearOfPlentyCard() {}
    virtual DevCardType getType() const;
};

/**
 * A development card used to take all resources of a particular type from all players.
 */
class MonopolyCard : public DevelopmentCard {
public:
    MonopolyCard() {}
    virtual DevCardType getType() const;
};

/**
 * A development card used to build two roads at no cost.
 */
class RoadBuildingCard : public DevelopmentCard {
private:

public:
	RoadBuildingCard() {};
    virtual DevCardType getType() const;
};




#endif
