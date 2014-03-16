#include "GameView.h"

#include <stdexcept>

#include "GameBoard.h"
#include "GameController.h"
#include "Renderer.h"

using std::make_pair;
using std::pair;
using std::runtime_error;
using std::string;

GameView::GameView(GameBoard& model, GameController& controller) : model(model), controller(controller) {
	
}

GameView::~GameView() {
	
}

void GameView::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	DrawingGameVisitor visitor(*this);
	model.accept(visitor);
	
	glFlush();
}

bool GameView::acceptInput(SDL_Event& event) {
	if(event.type == SDL_QUIT) {
		return false;
	}
	return true;
}

DrawingGameVisitor::DrawingGameVisitor(GameView& view) : view(view) {
	
}

DrawingGameVisitor::~DrawingGameVisitor() {
	
}

void DrawingGameVisitor::visit(GameBoard& model) {
	
}

void DrawingGameVisitor::visit(Road& road) {
	
}

void DrawingGameVisitor::visit(Settlement& settlement) {
	
}

void DrawingGameVisitor::visit(City& city) {
	
}

void DrawingGameVisitor::visit(Player& player) {
	
}

void DrawingGameVisitor::visit(ResourceTile& tile) {
	Coordinate coord = tile.getLocation();
	static GLuint tileTextures = loadImageAsTexture("resources/tiles.bmp");
	glBindTexture(GL_TEXTURE_2D, tileTextures);
	typedef std::vector<pair<float, float> > texCoordList;
	static const std::map<resourceType, texCoordList> resourceTexCoords = {
		make_pair(WHEAT, texCoordList { make_pair(377, 73), make_pair(500, 285), 
			make_pair(380, 502), make_pair(136, 503), make_pair(10, 288), make_pair(134, 74)}),
		make_pair(SHEEP, texCoordList { make_pair(959, 75), make_pair(1076, 288),
			make_pair(955, 503), make_pair(712, 501), make_pair(586, 289), make_pair(708, 73)}),
		make_pair(WOOD, texCoordList { make_pair(1491, 60), make_pair(1618, 269),
			make_pair(1479, 490), make_pair(1260, 493), make_pair(1126, 283), make_pair(1246, 65)}),
		make_pair(STONE, texCoordList { make_pair(382, 689), make_pair(506, 898),
			make_pair(386, 1118), make_pair(142, 1120), make_pair(17, 905), make_pair(138, 691)}),
		make_pair(BRICK, texCoordList { make_pair(1496, 690), make_pair(1617, 908),
			make_pair(1490, 1120), make_pair(1248, 1118), make_pair(1124, 898), make_pair(1250, 688)}),
		make_pair(DESERT, texCoordList { make_pair(1496, 690), make_pair(1617, 908),
			make_pair(1490, 1120), make_pair(1248, 1118), make_pair(1124, 898), make_pair(1250, 688)}),
	};
	static Coordinate adjacentCoordDiffs[] = {Coordinate(0, 1), Coordinate(1, 0), Coordinate(1, -1), Coordinate(0, -1), Coordinate(-1, 0), Coordinate(-1, 1)};
	if(resourceTexCoords.find(tile.resource) == resourceTexCoords.end()) {
		throw runtime_error("Cannot draw this tile; it is invalid.");
	}
	const texCoordList& texCoords = resourceTexCoords.find(tile.resource)->second;
	glBegin(GL_TRIANGLE_FAN);
	texCoordPair(averagePoint(texCoords));
	vertexPair(coord);
	for(unsigned int i = 0; i < texCoords.size(); i++) {
		texCoordPair(texCoords[i]);
		Coordinate diff = adjacentCoordDiffs[i];
		vertexPair(Coordinate(coord.first + diff.first, coord.second + diff.second));
	}
	texCoordPair(texCoords[0]);
	vertexPair(Coordinate(coord.first + adjacentCoordDiffs[0].first, coord.second + adjacentCoordDiffs[0].second));
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void DrawingGameVisitor::visit(DevelopmentCard& card) {
	
}