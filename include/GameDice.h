#ifndef GAMEDICE_H
#define GAMEDICE_H

class GameVisitor;

/**
 * The two dice used to determine the tiles that give resources.
 */
class GameDice {
private:
	int first;
	int second;
public:
	GameDice();
	
	int roll();
	int getFirst() const;
	int getSecond() const;
	virtual void accept(GameVisitor& visitor);
};

#endif	
