#include "Application.h"
#include "characterSelScene.h"
#include "mdGuiManager.h"



characterSelScene::characterSelScene(bool active) : scene(CHAR_SEL_SCENE)	{
	scene_active = active;

	buttons_node = scene_config.child("char_sel_scene").child("buttons");
	labels_node = scene_config.child("char_sel_scene").child("labels");
	textures_node = scene_config.child("char_sel_scene").child("textures");
	setRects();
	//By default, both player have Warrior selected
	player1.character = player2.character = WARRIOR;
}


characterSelScene::~characterSelScene()	{}

bool characterSelScene::start()	{
	character_potraits = App->textures->load("assets/character_sel_portraits.png");
	vs_tex = App->textures->load("assets/vs.png");
		
	loadSceneUi();
	assignFocus();
	
	return true;
}

bool characterSelScene::update(float dt)	{
	setCurrentCharDisplay();
	loadSceneTextures();
	checkSceneInput();
	App->gui->draw();

	return true;
}

bool characterSelScene::onEvent(Buttons * button)	{
	switch (button->button_type)
	{
	default:
		break;
	case SELECT_WARRIOR:
		player1.character = WARRIOR;
		break;
	case SELECT_MAGE:
		player1.character = MAGE;
		break;
	case SELECT_ROGUE:
		player1.character = ROGUE;
		break;
	case SELECT_PALADIN:
		player1.character = PALADIN;
		break;
	}

	player1.has_selected_character = true;
	return true;
}

void characterSelScene::loadSceneUi()	{
	scene_title = (Labels*)App->gui->createLabel(labels_node.child("title").child("content").attribute("value").as_string(), { (Uint8)labels_node.child("title").child("color").attribute("r").as_int(),(Uint8)labels_node.child("title").child("color").attribute("g").as_int(),(Uint8)labels_node.child("title").child("color").attribute("b").as_int(),(Uint8)labels_node.child("title").child("color").attribute("a").as_int() },
	App->fonts->extra_large_size, { labels_node.child("title").child("pos").attribute("x").as_int(), labels_node.child("title").child("pos").attribute("y").as_int() }, this);

	b_warrior = (Buttons*)App->gui->createButton(SELECT_WARRIOR, CHARACTER_SELECTION, 0, { 680, 300 }, this);
	b_mage = (Buttons*)App->gui->createButton(SELECT_MAGE, CHARACTER_SELECTION, 0, { 680, 450 }, this);
	b_rogue = (Buttons*)App->gui->createButton(SELECT_ROGUE, CHARACTER_SELECTION, 0, { 680, 600 }, this);
	b_paladin = (Buttons*)App->gui->createButton(SELECT_PALADIN, CHARACTER_SELECTION, 0, { 680, 750 }, this);
}

void characterSelScene::loadSceneTextures()	{
	//PROVISIONAL: Hardcoded values
	//Player 1 portrait (left)
	if (player1.portrait == &paladin_portrait || player1.portrait == &warrior_portrait)
		App->render->drawSprite(3, character_potraits, -190, 350, player1.portrait, 3, true, 1.0f, 0, 0, 0, false);
	else
		App->render->drawSprite(3, character_potraits, -190, 350, player1.portrait, 3, false, 1.0f, 0, 0, 0, false);

	//Player 2 portrait (right)
	if (player2.portrait == &rogue_portrait || player2.portrait == &mage_portrait)
		App->render->drawSprite(3, character_potraits, 800, 350, player2.portrait, 3, true, 1.0f, 0, 0, 0, false);
	else
		App->render->drawSprite(3, character_potraits, 800, 350, player2.portrait, 3, false, 1.0f, 0, 0, 0, false);

	//VS
//	App->render->drawSprite(4, vs_tex, 700, 400, 0, 2, false, 1.0f, 0, 0, 0, false);

	//CHARACTER MINIATURES
	//Left
	App->render->drawSprite(3, App->gui->atlas, 684, 306, &warrior_miniature, 3, false, 1.0f, 0, 0, 0, false);
	App->render->drawSprite(3, App->gui->atlas, 684, 456, &mage_miniature, 3, false, 1.0f, 0, 0, 0, false);
	App->render->drawSprite(3, App->gui->atlas, 684, 606, &rogue_miniature, 3, false, 1.0f, 0, 0, 0, false);
	App->render->drawSprite(3, App->gui->atlas, 684, 756, &paladin_miniature, 3, false, 1.0f, 0, 0, 0, false);

	//Right
	App->render->drawSprite(3, App->gui->atlas, 1054, 306, &warrior_miniature, 3, false, 1.0f, 0, 0, 0, false);
	App->render->drawSprite(3, App->gui->atlas, 1054, 456, &mage_miniature, 3, false, 1.0f, 0, 0, 0, false);
	App->render->drawSprite(3, App->gui->atlas, 1054, 606, &rogue_miniature, 3, false, 1.0f, 0, 0, 0, false);
	App->render->drawSprite(3, App->gui->atlas, 1054, 756, &paladin_miniature, 3, false, 1.0f, 0, 0, 0, false);
}

void characterSelScene::assignFocus()	{
	if (!App->gui->p1_focus_elements.empty() && !App->gui->p2_focus_elements.empty())
	{
		App->entities->players[0]->focus = *App->gui->p1_focus_elements.begin();
		App->entities->players[1]->focus = *App->gui->p2_focus_elements.begin();
	}
}

void characterSelScene::checkSceneInput()	{
	//Check player 1 input and player 2 input to select characters and objects

}

void characterSelScene::assignCharacterToPlayer()	{

}

void characterSelScene::setRects()	{
	//PORTRAITS
	pugi::xml_node warrior = textures_node.child("warrior_portrait");
	pugi::xml_node mage = textures_node.child("mage_portrait");
	pugi::xml_node rogue = textures_node.child("rogue_portrait");
	pugi::xml_node paladin = textures_node.child("paladin_portrait");

	warrior_portrait.x = warrior.attribute("x").as_int();
	warrior_portrait.y = warrior.attribute("y").as_int();
	warrior_portrait.w = warrior.attribute("w").as_int();
	warrior_portrait.h = warrior.attribute("h").as_int();

	mage_portrait.x = mage.attribute("x").as_int();
	mage_portrait.y = mage.attribute("y").as_int();
	mage_portrait.w = mage.attribute("w").as_int();
	mage_portrait.h = mage.attribute("h").as_int();

	rogue_portrait.x = rogue.attribute("x").as_int();
	rogue_portrait.y = rogue.attribute("y").as_int();
	rogue_portrait.w = rogue.attribute("w").as_int();
	rogue_portrait.h = rogue.attribute("h").as_int();

	paladin_portrait.x = paladin.attribute("x").as_int();
	paladin_portrait.y = paladin.attribute("y").as_int();
	paladin_portrait.w = paladin.attribute("w").as_int();
	paladin_portrait.h = paladin.attribute("h").as_int();

	//MINIATURES
	pugi::xml_node warrior_m = textures_node.child("warrior_miniature");
	pugi::xml_node mage_m = textures_node.child("mage_miniature");
	pugi::xml_node rogue_m = textures_node.child("rogue_miniature");
	pugi::xml_node paladin_m = textures_node.child("paladin_miniature");

	warrior_miniature.x = warrior_m.attribute("x").as_int();
	warrior_miniature.y = warrior_m.attribute("y").as_int();
	warrior_miniature.w = warrior_m.attribute("w").as_int();
	warrior_miniature.h = warrior_m.attribute("h").as_int();

	mage_miniature.x = mage_m.attribute("x").as_int();
	mage_miniature.y = mage_m.attribute("y").as_int();
	mage_miniature.w = mage_m.attribute("w").as_int();
	mage_miniature.h = mage_m.attribute("h").as_int();

	rogue_miniature.x = rogue_m.attribute("x").as_int();
	rogue_miniature.y = rogue_m.attribute("y").as_int();
	rogue_miniature.w = rogue_m.attribute("w").as_int();
	rogue_miniature.h = rogue_m.attribute("h").as_int();

	paladin_miniature.x = paladin_m.attribute("x").as_int();
	paladin_miniature.y = paladin_m.attribute("y").as_int();
	paladin_miniature.w = paladin_m.attribute("w").as_int();
	paladin_miniature.h = paladin_m.attribute("h").as_int();

}

void characterSelScene::setCurrentCharDisplay()	{
	//Function to set all current character's info to screen

	switch (player1.character)
	{
	default:
		break;
	case WARRIOR:
		player1.portrait = &warrior_portrait;
		break;
	case MAGE:
		player1.portrait = &mage_portrait;
		break;
	case ROGUE:
		player1.portrait = &rogue_portrait;
		break;
	case PALADIN:
		player1.portrait = &paladin_portrait;
		break;
	}
}
