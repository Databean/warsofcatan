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


enum DevCardType { KNIGHT, VICTORYPOINT, YEAROFPLENTY, MONOPOLY, ROADBUILDING };



class DevelopmentCard {

private:
    Player* owner;
    DevCardType type;
public:
    DevelopmentCard(Player* player);
    virtual ~DevelopmentCard();

    virtual DevCardType getType() = 0;
    virtual void playCard() = 0;

};



class KnightCard : public DevelopmentCard {
private:

public:
	KnightCard(Player* player);
//	virtual ~KnightCard();

    DevCardType getType();
    void playCard();

};



class VictoryPointCard : public DevelopmentCard {
public:
    VictoryPointCard(Player* player);
//    virtual ~VictoryPointCard();

    DevCardType getType();
    void playCard();

};


class YearOfPlentyCard : public DevelopmentCard {
public:
    YearOfPlentyCard(Player* player);
//    virtual ~YearOfPlentyCard();

    DevCardType getType();
    void playCard();

};




class MonopolyCard : public DevelopmentCard {
public:
    MonopolyCard(Player* player);
//    virtual ~MonopolyCard();

    DevCardType getType();
    void playCard();

};



class RoadBuildingCard : public DevelopmentCard {
private:

public:
	RoadBuildingCard(Player* player);
//	virtual ~RoadBuildingCard();

    DevCardType getType();
    void playCard();
};




#endif
