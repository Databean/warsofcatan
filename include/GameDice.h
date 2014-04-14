#ifndef GAMEDICE_H
#define GAMEDICE_H





class GameVisitor;

class GameDice {
	private:
		int first;
		int second;
		
		
	public:
		int getFirst();
		int getSecond();
		bool shown;
		void setFirst(int newFirst);
		void setSecond(int newSecond);
		virtual void accept(GameVisitor& visitor);



};

#endif	