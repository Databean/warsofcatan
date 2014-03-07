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


enum DevCardType { KNIGHT, VICTORYPOINT, YEAROFPLENTY, MONOPOLY, ROADBUILDING };



class DevelopmentCard {

private:
    Player* owner;
    DevCardType type;
public:
    DevelopmentCard(Player* player);
    virtual ~DevelopmentCard();

    virtual DevCardType getType() const = 0;
    virtual void playCard() = 0;

    virtual Player* getOwner();
	virtual void accept(GameVisitor& visitor);
	virtual bool operator==(const DevelopmentCard&);
};



class KnightCard : public DevelopmentCard {
private:

public:
	KnightCard(Player* player);
//	virtual ~KnightCard();

    virtual DevCardType getType() const;
    virtual void playCard();

};



class VictoryPointCard : public DevelopmentCard {
public:
    VictoryPointCard(Player* player);
//    virtual ~VictoryPointCard();

    virtual DevCardType getType() const;
    virtual void playCard();

};


class YearOfPlentyCard : public DevelopmentCard {
public:
    YearOfPlentyCard(Player* player);
//    virtual ~YearOfPlentyCard();

    virtual DevCardType getType() const;
    virtual void playCard();

};




class MonopolyCard : public DevelopmentCard {
public:
    MonopolyCard(Player* player);
//    virtual ~MonopolyCard();

    virtual DevCardType getType() const;
    virtual void playCard();

};



class RoadBuildingCard : public DevelopmentCard {
private:

public:
	RoadBuildingCard(Player* player);
//	virtual ~RoadBuildingCard();

    virtual DevCardType getType() const;
    virtual void playCard();
};




#endif
