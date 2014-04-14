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



class DevelopmentCard {

protected:
    DevCardType type;
    GameBoard& board;
public:
    DevelopmentCard(GameBoard& board);
    virtual ~DevelopmentCard();

    virtual DevCardType getType() const = 0;

	virtual void accept(GameVisitor& visitor);
	virtual bool operator==(const DevelopmentCard&);
};



class KnightCard : public DevelopmentCard {
private:

public:
	KnightCard(GameBoard& board);
//	virtual ~KnightCard();

    virtual DevCardType getType() const;
    void playCard(Player *player, Coordinate target);

};



class VictoryPointCard : public DevelopmentCard {
public:
    VictoryPointCard(GameBoard& board);
//    virtual ~VictoryPointCard();

    virtual DevCardType getType() const;

};


class YearOfPlentyCard : public DevelopmentCard {
public:
    YearOfPlentyCard(GameBoard& board);
//    virtual ~YearOfPlentyCard();

    virtual DevCardType getType() const;
    void playCard(Player *player, int rType1, int rType2);

};




class MonopolyCard : public DevelopmentCard {
public:
    MonopolyCard(GameBoard& board);
//    virtual ~MonopolyCard();

    virtual DevCardType getType() const;
    void playCard(Player *player, int rType);

};



class RoadBuildingCard : public DevelopmentCard {
private:

public:
	RoadBuildingCard(GameBoard& board);
//	virtual ~RoadBuildingCard();

    virtual DevCardType getType() const;
    void playCard(Player* player, Coordinate start1, Coordinate end1, Coordinate start2, Coordinate end2);
};




#endif
