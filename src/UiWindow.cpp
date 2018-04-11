#include "Application.h"
#include "UiWindow.h"
#include "mdRender.h"


UiWindow::UiWindow(window_type _type, std::pair<int, int> pos, Module * callback) : Widgets(WINDOW, pos, callback){
	data = config.child("gui").child("window_rect");
	window_rect = { data.attribute("x").as_int(),data.attribute("y").as_int(), data.attribute("w").as_int(), data.attribute("h").as_int() };
}

UiWindow::~UiWindow(){}

void UiWindow::draw()	{
	App->render->drawSprite(3, App->gui->getAtlas(), position.first, position.second, &window_rect, 4);
}
