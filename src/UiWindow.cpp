#include "Application.h"
#include "UiWindow.h"
#include "mdRender.h"
#include "mdGuiManager.h"


UiWindow::UiWindow(window_type _type, std::pair<int, int> pos, scene* callback) : Widgets(WINDOW, pos, callback){
	data = config.child("gui").child("window_rect");
	window_rect = { data.attribute("x").as_int(),data.attribute("y").as_int(), data.attribute("w").as_int(), data.attribute("h").as_int() };
	current_rect = &window_rect;
}

UiWindow::~UiWindow(){
	current_rect = nullptr;
}

void UiWindow::draw()	{
	App->render->drawSprite(5, App->gui->getAtlas(), position.first, position.second, current_rect, 4, false, 1.0f, 0.0, 0, 0, false);
}
