#include "Application.h"
#include "characterSelScene.h"
#include "mdGuiManager.h"
#include "mdSceneManager.h"



characterSelScene::characterSelScene(bool active) : scene(CHAR_SEL_SCENE)	{
	scene_active = active;
	name = "Character Selection";

	buttons_node = scene_config.child("char_sel_scene").child("buttons");
	labels_node = scene_config.child("char_sel_scene").child("labels");
	textures_node = scene_config.child("char_sel_scene").child("textures");
	setRects();
	//By default, both player have Warrior selected
	player1.character = player2.character = WARRIOR;
}


characterSelScene::~characterSelScene()	{}

bool characterSelScene::start()	{
	character_potraits = App->textures->load(textures_node.child("portraits_tex").attribute("path").as_string());
	vs_tex = App->textures->load(textures_node.child("vs_tex").attribute("path").as_string());
	character_names = App->textures->load(textures_node.child("char_name_tex").attribute("path").as_string());
	items = App->textures->load(textures_node.child("items_tex").attribute("path").as_string());
	ready_tex = App->textures->load(textures_node.child("ready_tex").attribute("path").as_string());

	App->entities->players[0]->focus = App->entities->players[1]->focus = nullptr;

	loadSceneUi();
	assignFocus();
	
	return true;
}

bool characterSelScene::update(float dt)	{
	if (!App->entities->players[0]->focus || !App->entities->players[1]->focus)
		assignFocus();

	setCurrentCharDisplay();
	loadSceneTextures();
	checkSceneInput();
	App->gui->draw();

	if (transition_timer.isActive())
		startingTransition();

	return true;
}

bool characterSelScene::onEvent(Buttons* button)	{
	switch (button->button_type)
	{
	default:
		break;
	case SELECT_WARRIOR:
		if (button->focus_id == 0)
			player1.character = WARRIOR;
		else 
			player2.character = WARRIOR;
		break;
	case SELECT_MAGE:
		if (button->focus_id == 0)
			player1.character = MAGE;
		else 
			player2.character = MAGE;
		break;
	case SELECT_ROGUE:
		if (button->focus_id == 0)
			player1.character = ROGUE;
		else 
			player2.character = ROGUE;
		break;
	case SELECT_PALADIN:
		if (button->focus_id == 0)
			player1.character = PALADIN;
		else 
			player2.character = PALADIN;
		break;
	case SELECT_ITEM1:
		if (button->focus_id == 0) {
			player1.item = SPECIAL_ITEM_1;
			player1.has_selected_item = true;
			closeP1Window();
		}
		else  {
			player2.item = SPECIAL_ITEM_1;
			player2.has_selected_item = true;
			closeP2Window();
		}
		break;
	case SELECT_ITEM2:
		if (button->focus_id == 0) {
			player1.item = SPECIAL_ITEM_2;
			player1.has_selected_item = true;
			closeP1Window();
		}
		else {
			player2.item = SPECIAL_ITEM_2;
			player2.has_selected_item = true;
			closeP2Window();
		}
		break;
	}

	if (button->being_clicked && button->size == CHARACTER_SELECTION)
	{
		if (button->focus_id == 0)
		{
			if (App->entities->players[0]->getInput(LIGHT_ATTACK, KEY_DOWN))
				player1.skin = 0;
			if (App->entities->players[0]->getInput(HEAVY_ATTACK, KEY_DOWN))
				player1.skin = 1;
			if (App->entities->players[0]->getInput(SPECIAL_1, KEY_DOWN))
				player1.skin = 2;
			if (App->entities->players[0]->getInput(SPECIAL_2, KEY_DOWN))
				player1.skin = 3;

			player1.has_selected_character = true; 
			popUpP1Window();

		}
		else
		{
			if (App->entities->players[1]->getInput(LIGHT_ATTACK, KEY_DOWN))
				player2.skin = 0;
			if (App->entities->players[1]->getInput(HEAVY_ATTACK, KEY_DOWN))
				player2.skin = 1;
			if (App->entities->players[1]->getInput(SPECIAL_1, KEY_DOWN))
				player2.skin = 2;
			if (App->entities->players[1]->getInput(SPECIAL_2, KEY_DOWN))
				player2.skin = 3;

			player2.has_selected_character = true;
			popUpP2Window();
		}
	}

	return true;
}

void characterSelScene::loadSceneUi()	{
	//LABELS
	scene_title = (Labels*)App->gui->createLabel(labels_node.child("title").child("content").attribute("value").as_string(), { (Uint8)labels_node.child("title").child("color").attribute("r").as_int(),(Uint8)labels_node.child("title").child("color").attribute("g").as_int(),(Uint8)labels_node.child("title").child("color").attribute("b").as_int(),(Uint8)labels_node.child("title").child("color").attribute("a").as_int() },
	App->fonts->extra_large_size, { labels_node.child("title").child("pos").attribute("x").as_int(), labels_node.child("title").child("pos").attribute("y").as_int() }, this);

	p1_label = (Labels*)App->gui->createLabel(labels_node.child("player1_l").child("content").attribute("value").as_string(), { (Uint8)labels_node.child("player1_l").child("color").attribute("r").as_int(),(Uint8)labels_node.child("player1_l").child("color").attribute("g").as_int(),(Uint8)labels_node.child("player1_l").child("color").attribute("b").as_int(),(Uint8)labels_node.child("player1_l").child("color").attribute("a").as_int() },
	App->fonts->large_size, { labels_node.child("player1_l").child("pos").attribute("x").as_int(), labels_node.child("player1_l").child("pos").attribute("y").as_int() }, this);
	
	p2_label = (Labels*)App->gui->createLabel(labels_node.child("player2_l").child("content").attribute("value").as_string(), { (Uint8)labels_node.child("player2_l").child("color").attribute("r").as_int(),(Uint8)labels_node.child("player2_l").child("color").attribute("g").as_int(),(Uint8)labels_node.child("player2_l").child("color").attribute("b").as_int(),(Uint8)labels_node.child("player2_l").child("color").attribute("a").as_int() },
	App->fonts->large_size, { labels_node.child("player2_l").child("pos").attribute("x").as_int(), labels_node.child("player2_l").child("pos").attribute("y").as_int() }, this);

	//BUTTONS
	createP1CharButtons();
	createP2CharButtons();
}

void characterSelScene::createP1CharButtons()	{
	//Left
	b_warrior_l = (Buttons*)App->gui->createButton(SELECT_WARRIOR, CHARACTER_SELECTION, buttons_node.child("b_warrior_l").child("player_id").attribute("value").as_int(), { buttons_node.child("b_warrior_l").child("pos").attribute("x").as_int(), buttons_node.child("b_warrior_l").child("pos").attribute("y").as_int() }, this);
	b_mage_l = (Buttons*)App->gui->createButton(SELECT_MAGE, CHARACTER_SELECTION, buttons_node.child("b_mage_l").child("player_id").attribute("value").as_int(), { buttons_node.child("b_mage_l").child("pos").attribute("x").as_int(), buttons_node.child("b_mage_l").child("pos").attribute("y").as_int() }, this);
	b_rogue_l = (Buttons*)App->gui->createButton(SELECT_ROGUE, CHARACTER_SELECTION, buttons_node.child("b_rogue_l").child("player_id").attribute("value").as_int(), { buttons_node.child("b_rogue_l").child("pos").attribute("x").as_int(), buttons_node.child("b_rogue_l").child("pos").attribute("y").as_int() }, this);
	b_paladin_l = (Buttons*)App->gui->createButton(SELECT_PALADIN, CHARACTER_SELECTION, buttons_node.child("b_paladin_l").child("player_id").attribute("value").as_int(), { buttons_node.child("b_paladin_l").child("pos").attribute("x").as_int(), buttons_node.child("b_paladin_l").child("pos").attribute("y").as_int() }, this);

}

void characterSelScene::createP2CharButtons()	{
	//Right
	b_warrior_r = (Buttons*)App->gui->createButton(SELECT_WARRIOR, CHARACTER_SELECTION, buttons_node.child("b_warrior_r").child("player_id").attribute("value").as_int(), { buttons_node.child("b_warrior_r").child("pos").attribute("x").as_int(), buttons_node.child("b_warrior_r").child("pos").attribute("y").as_int() }, this);
	b_mage_r = (Buttons*)App->gui->createButton(SELECT_MAGE, CHARACTER_SELECTION, buttons_node.child("b_mage_r").child("player_id").attribute("value").as_int(), { buttons_node.child("b_mage_r").child("pos").attribute("x").as_int(), buttons_node.child("b_mage_r").child("pos").attribute("y").as_int() }, this);
	b_rogue_r = (Buttons*)App->gui->createButton(SELECT_ROGUE, CHARACTER_SELECTION, buttons_node.child("b_rogue_r").child("player_id").attribute("value").as_int(), { buttons_node.child("b_rogue_r").child("pos").attribute("x").as_int(), buttons_node.child("b_rogue_r").child("pos").attribute("y").as_int() }, this);
	b_paladin_r = (Buttons*)App->gui->createButton(SELECT_PALADIN, CHARACTER_SELECTION, buttons_node.child("b_paladin_r").child("player_id").attribute("value").as_int(), { buttons_node.child("b_paladin_r").child("pos").attribute("x").as_int(), buttons_node.child("b_paladin_r").child("pos").attribute("y").as_int() }, this);

}

void characterSelScene::loadSceneTextures()	{

	//Player 1 portrait (left)
	if (player1.portrait == &paladin_portrait || player1.portrait == &warrior_portrait)
		App->render->drawSprite(2, character_potraits, textures_node.child("left_portraits").attribute("x").as_int(), textures_node.child("left_portraits").attribute("y").as_int(), player1.portrait, textures_node.child("portraits").attribute("scale").as_int(), true, 1.0f, 0, 0, 0, false);
	else
		App->render->drawSprite(2, character_potraits, textures_node.child("left_portraits").attribute("x").as_int(), textures_node.child("left_portraits").attribute("y").as_int(), player1.portrait, textures_node.child("portraits").attribute("scale").as_int(), false, 1.0f, 0, 0, 0, false);

	//Player 2 portrait (right)
	if (player2.portrait == &rogue_portrait || player2.portrait == &mage_portrait)
		App->render->drawSprite(2, character_potraits, textures_node.child("right_portraits").attribute("x").as_int(), textures_node.child("right_portraits").attribute("y").as_int(), player2.portrait, textures_node.child("portraits").attribute("scale").as_int(), true, 1.0f, 0, 0, 0, false);
	else
		App->render->drawSprite(2, character_potraits, textures_node.child("right_portraits").attribute("x").as_int(), textures_node.child("right_portraits").attribute("y").as_int(), player2.portrait, textures_node.child("portraits").attribute("scale").as_int(), false, 1.0f, 0, 0, 0, false);

	//VS
//	App->render->drawSprite(4, vs_tex, textures_node.child("vs_tex").attribute("x").as_int(), textures_node.child("vs_tex").attribute("y").as_int(), 0, textures_node.child("vs_tex").attribute("scale").as_int(), false, 1.0f, 0, 0, 0, false);

	//Character Miniatures
	loadCharMiniaturesTex();
	
	//CHARACTER NAME TEXTURES
	//Left
	App->render->drawSprite(4, character_names, textures_node.child("left_names").attribute("x").as_int(), textures_node.child("left_names").attribute("y").as_int(), player1.name_tex, textures_node.child("names").attribute("scale").as_int(), false, 1.0f, 0, 0, 0, false);
	//Right
	App->render->drawSprite(4, character_names, textures_node.child("right_names").attribute("x").as_int(), textures_node.child("right_names").attribute("y").as_int(), player2.name_tex, textures_node.child("names").attribute("scale").as_int(), false, 1.0f, 0, 0, 0, false);

	if (player1.has_selected_character && player1.has_selected_item)
		App->render->drawSprite(4, ready_tex, textures_node.child("ready_p1").attribute("x").as_int(), textures_node.child("ready_p1").attribute("y").as_int(), 0, textures_node.child("ready_p1").attribute("scale").as_int(), false, 1.0f, 0, 0, 0, false);
	if (player2.has_selected_character && player2.has_selected_item)
		App->render->drawSprite(4, ready_tex, textures_node.child("ready_p2").attribute("x").as_int(), textures_node.child("ready_p2").attribute("y").as_int(), 0, textures_node.child("ready_p2").attribute("scale").as_int(), false, 1.0f, 0, 0, 0, false);

	//OBJECT TEXTUREs
	loadObjectTextures();

}

void characterSelScene::loadObjectTextures()	{
	pugi::xml_node obj1;
	pugi::xml_node obj2;

	if (player1.has_selected_character && !player1.has_selected_item)
	{
		obj1 = textures_node.child("p1_obj1_tex");
		obj2 = textures_node.child("p1_obj2_tex");
		switch (player1.character)
		{
		default:
			break;
		case WARRIOR:
			App->render->drawSprite(6, items, obj1.attribute("x").as_int(), obj1.attribute("y").as_int(), &warrior_item1, obj1.attribute("scale").as_int(), false, 1.0f, 0, 0, 0, false);
			App->render->drawSprite(6, items, obj2.attribute("x").as_int(), obj2.attribute("y").as_int(), &warrior_item2, obj2.attribute("scale").as_int(), false, 1.0f, 0, 0, 0, false);
			break;
		case MAGE:
			App->render->drawSprite(6, items, obj1.attribute("x").as_int(), obj1.attribute("y").as_int(), &mage_item1, obj1.attribute("scale").as_int(), false, 1.0f, 0, 0, 0, false);
			App->render->drawSprite(6, items, obj2.attribute("x").as_int(), obj2.attribute("y").as_int(), &mage_item2, obj2.attribute("scale").as_int(), false, 1.0f, 0, 0, 0, false);
			break;
		case ROGUE:
			App->render->drawSprite(6, items, obj1.attribute("x").as_int(), obj1.attribute("y").as_int(), &rogue_item1, obj1.attribute("scale").as_int(), false, 1.0f, 0, 0, 0, false);
			App->render->drawSprite(6, items, obj2.attribute("x").as_int(), obj2.attribute("y").as_int(), &rogue_item2, obj2.attribute("scale").as_int(), false, 1.0f, 0, 0, 0, false);
			break;
		case PALADIN:
			App->render->drawSprite(6, items, obj1.attribute("x").as_int(), obj1.attribute("y").as_int() - 30, &paladin_item1, obj1.attribute("scale").as_int(), false, 1.0f, 0, 0, 0, false);
			App->render->drawSprite(6, items, obj2.attribute("x").as_int(), obj2.attribute("y").as_int() - 30, &paladin_item2, obj2.attribute("scale").as_int(), false, 1.0f, 0, 0, 0, false);
			break;
		}
	}

	if (player2.has_selected_character && !player2.has_selected_item)
	{
		obj1 = textures_node.child("p2_obj1_tex");
		obj2 = textures_node.child("p2_obj2_tex");
		switch (player2.character)
		{
		default:
			break;
		case WARRIOR:
			App->render->drawSprite(6, items, obj1.attribute("x").as_int(), obj1.attribute("y").as_int(), &warrior_item1, obj1.attribute("scale").as_int(), false, 1.0f, 0, 0, 0, false);
			App->render->drawSprite(6, items, obj2.attribute("x").as_int(), obj2.attribute("y").as_int(), &warrior_item2, obj2.attribute("scale").as_int(), false, 1.0f, 0, 0, 0, false);
			break;
		case MAGE:
			App->render->drawSprite(6, items, obj1.attribute("x").as_int(), obj1.attribute("y").as_int(), &mage_item1, obj1.attribute("scale").as_int(), false, 1.0f, 0, 0, 0, false);
			App->render->drawSprite(6, items, obj2.attribute("x").as_int(), obj2.attribute("y").as_int(), &mage_item2, obj2.attribute("scale").as_int(), false, 1.0f, 0, 0, 0, false);
			break;
		case ROGUE:
			App->render->drawSprite(6, items, obj1.attribute("x").as_int(), obj1.attribute("y").as_int(), &rogue_item1, obj1.attribute("scale").as_int(), false, 1.0f, 0, 0, 0, false);
			App->render->drawSprite(6, items, obj2.attribute("x").as_int(), obj2.attribute("y").as_int(), &rogue_item2, obj2.attribute("scale").as_int(), false, 1.0f, 0, 0, 0, false);
			break;
		case PALADIN:
			App->render->drawSprite(6, items, obj1.attribute("x").as_int(), obj1.attribute("y").as_int() - 30, &paladin_item1, obj1.attribute("scale").as_int(), false, 1.0f, 0, 0, 0, false);
			App->render->drawSprite(6, items, obj2.attribute("x").as_int(), obj2.attribute("y").as_int() - 30, &paladin_item2, obj2.attribute("scale").as_int(), false, 1.0f, 0, 0, 0, false);
			break;
		}
	}
}

void characterSelScene::loadCharMiniaturesTex()	{

	pugi::xml_node min_pos = textures_node.child("miniature_pos");
	if (!player1.has_selected_character)
	{
		//CHARACTER MINIATURES
		//Left
		App->render->drawSprite(3, App->gui->atlas, min_pos.attribute("x1").as_int(), min_pos.attribute("y1").as_int(), &warrior_miniature, 3, false, 1.0f, 0, 0, 0, false);
		App->render->drawSprite(3, App->gui->atlas, min_pos.attribute("x1").as_int(), min_pos.attribute("y2").as_int(), &mage_miniature, 3, false, 1.0f, 0, 0, 0, false);
		App->render->drawSprite(3, App->gui->atlas, min_pos.attribute("x1").as_int(), min_pos.attribute("y3").as_int(), &rogue_miniature, 3, false, 1.0f, 0, 0, 0, false);
		App->render->drawSprite(3, App->gui->atlas, min_pos.attribute("x1").as_int(), min_pos.attribute("y4").as_int(), &paladin_miniature, 3, false, 1.0f, 0, 0, 0, false);
	}
	if (!player2.has_selected_character)
	{
		//Right
		App->render->drawSprite(3, App->gui->atlas, min_pos.attribute("x2").as_int(), min_pos.attribute("y1").as_int(), &warrior_miniature, 3, false, 1.0f, 0, 0, 0, false);
		App->render->drawSprite(3, App->gui->atlas, min_pos.attribute("x2").as_int(), min_pos.attribute("y2").as_int(), &mage_miniature, 3, false, 1.0f, 0, 0, 0, false);
		App->render->drawSprite(3, App->gui->atlas, min_pos.attribute("x2").as_int(), min_pos.attribute("y3").as_int(), &rogue_miniature, 3, false, 1.0f, 0, 0, 0, false);
		App->render->drawSprite(3, App->gui->atlas, min_pos.attribute("x2").as_int(), min_pos.attribute("y4").as_int(), &paladin_miniature, 3, false, 1.0f, 0, 0, 0, false);
	}
}

void characterSelScene::assignFocus()	{
	if (!App->gui->p1_focus_elements.empty() && !App->entities->players[0]->focus)
		App->entities->players[0]->focus = *App->gui->p1_focus_elements.begin();

	if (!App->gui->p2_focus_elements.empty() && !App->entities->players[1]->focus)
		App->entities->players[1]->focus = *App->gui->p2_focus_elements.begin();
	
}

void characterSelScene::checkSceneInput()	{
	if (App->entities->players[0]->getInput(GRAB, KEY_DOWN) && object_win_p1)
		closeP1Window();
	if (App->entities->players[1]->getInput(GRAB, KEY_DOWN) && object_win_p2)
		closeP2Window();

	if (player1.has_selected_character && player1.has_selected_item && player2.has_selected_character && player2.has_selected_item && !transition_timer.isActive())
		assignCharacterToPlayer();
}

void characterSelScene::assignCharacterToPlayer()	{
	//Creating players
	App->entities->players[0]->createAndAssignCharacter(player1.character, player1.item, false, player1.skin);
	App->entities->players[1]->createAndAssignCharacter(player2.character, player2.item, true, player2.skin);

	//Hidding them
	App->entities->paused = true;
	App->entities->show = false;
	
	transition_timer.start();
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
	warrior = textures_node.child("warrior_miniature");
	mage = textures_node.child("mage_miniature");
	rogue = textures_node.child("rogue_miniature");
	paladin = textures_node.child("paladin_miniature");

	warrior_miniature.x = warrior.attribute("x").as_int();
	warrior_miniature.y = warrior.attribute("y").as_int();
	warrior_miniature.w = warrior.attribute("w").as_int();
	warrior_miniature.h = warrior.attribute("h").as_int();

	mage_miniature.x = mage.attribute("x").as_int();
	mage_miniature.y = mage.attribute("y").as_int();
	mage_miniature.w = mage.attribute("w").as_int();
	mage_miniature.h = mage.attribute("h").as_int();

	rogue_miniature.x = rogue.attribute("x").as_int();
	rogue_miniature.y = rogue.attribute("y").as_int();
	rogue_miniature.w = rogue.attribute("w").as_int();
	rogue_miniature.h = rogue.attribute("h").as_int();

	paladin_miniature.x = paladin.attribute("x").as_int();
	paladin_miniature.y = paladin.attribute("y").as_int();
	paladin_miniature.w = paladin.attribute("w").as_int();
	paladin_miniature.h = paladin.attribute("h").as_int();

	//NAME TEXTURES
	warrior = textures_node.child("warrior_name");
	mage = textures_node.child("mage_name");
	rogue = textures_node.child("rogue_name");
	paladin = textures_node.child("paladin_name");

	warrior_name_tex.x = warrior.attribute("x").as_int();
	warrior_name_tex.y = warrior.attribute("y").as_int();
	warrior_name_tex.w = warrior.attribute("w").as_int();
	warrior_name_tex.h = warrior.attribute("h").as_int();

	mage_name_tex.x = mage.attribute("x").as_int();
	mage_name_tex.y = mage.attribute("y").as_int();
	mage_name_tex.w = mage.attribute("w").as_int();
	mage_name_tex.h = mage.attribute("h").as_int();

	rogue_name_tex.x = rogue.attribute("x").as_int();
	rogue_name_tex.y = rogue.attribute("y").as_int();
	rogue_name_tex.w = rogue.attribute("w").as_int();
	rogue_name_tex.h = rogue.attribute("h").as_int();

	paladin_name_tex.x = paladin.attribute("x").as_int();
	paladin_name_tex.y = paladin.attribute("y").as_int();
	paladin_name_tex.w = paladin.attribute("w").as_int();
	paladin_name_tex.h = paladin.attribute("h").as_int();

	//OBJECTS
	warrior_item2.x = textures_node.child("warrior_obj1").attribute("x").as_int();
	warrior_item2.y = textures_node.child("warrior_obj1").attribute("y").as_int();
	warrior_item2.w = textures_node.child("warrior_obj1").attribute("w").as_int();
	warrior_item2.h = textures_node.child("warrior_obj1").attribute("h").as_int();

	warrior_item1.x = textures_node.child("warrior_obj2").attribute("x").as_int();
	warrior_item1.y = textures_node.child("warrior_obj2").attribute("y").as_int();
	warrior_item1.w = textures_node.child("warrior_obj2").attribute("w").as_int();
	warrior_item1.h = textures_node.child("warrior_obj2").attribute("h").as_int();

	mage_item1.x = textures_node.child("mage_obj2").attribute("x").as_int();
	mage_item1.y = textures_node.child("mage_obj2").attribute("y").as_int();
	mage_item1.w = textures_node.child("mage_obj2").attribute("w").as_int();
	mage_item1.h = textures_node.child("mage_obj2").attribute("h").as_int();

	mage_item2.x = textures_node.child("mage_obj1").attribute("x").as_int();
	mage_item2.y = textures_node.child("mage_obj1").attribute("y").as_int();
	mage_item2.w = textures_node.child("mage_obj1").attribute("w").as_int();
	mage_item2.h = textures_node.child("mage_obj1").attribute("h").as_int();

	rogue_item1.x = textures_node.child("rogue_obj2").attribute("x").as_int();
	rogue_item1.y = textures_node.child("rogue_obj2").attribute("y").as_int();
	rogue_item1.w = textures_node.child("rogue_obj2").attribute("w").as_int();
	rogue_item1.h = textures_node.child("rogue_obj2").attribute("h").as_int();

	rogue_item2.x = textures_node.child("rogue_obj1").attribute("x").as_int();
	rogue_item2.y = textures_node.child("rogue_obj1").attribute("y").as_int();
	rogue_item2.w = textures_node.child("rogue_obj1").attribute("w").as_int();
	rogue_item2.h = textures_node.child("rogue_obj1").attribute("h").as_int();

	paladin_item1.x = textures_node.child("paladin_obj2").attribute("x").as_int();
	paladin_item1.y = textures_node.child("paladin_obj2").attribute("y").as_int();
	paladin_item1.w = textures_node.child("paladin_obj2").attribute("w").as_int();
	paladin_item1.h = textures_node.child("paladin_obj2").attribute("h").as_int();

	paladin_item2.x = textures_node.child("paladin_obj1").attribute("x").as_int();
	paladin_item2.y = textures_node.child("paladin_obj1").attribute("y").as_int();
	paladin_item2.w = textures_node.child("paladin_obj1").attribute("w").as_int();
	paladin_item2.h = textures_node.child("paladin_obj1").attribute("h").as_int();
}

void characterSelScene::setCurrentCharDisplay()	{
	//Function to set all current character's info to screen

	switch (player1.character)
	{
	default:
		break;
	case WARRIOR:
		player1.portrait = &warrior_portrait;
		player1.name_tex = &warrior_name_tex;
		break;
	case MAGE:
		player1.portrait = &mage_portrait;
		player1.name_tex = &mage_name_tex;
		break;
	case ROGUE:
		player1.portrait = &rogue_portrait;
		player1.name_tex = &rogue_name_tex;
		break;
	case PALADIN:
		player1.portrait = &paladin_portrait;
		player1.name_tex = &paladin_name_tex;
		break;
	}

	switch (player2.character)
	{
	default:
		break;
	case WARRIOR:
		player2.portrait = &warrior_portrait;
		player2.name_tex = &warrior_name_tex;
		break;
	case MAGE:
		player2.portrait = &mage_portrait;
		player2.name_tex = &mage_name_tex;
		break;
	case ROGUE:
		player2.portrait = &rogue_portrait;
		player2.name_tex = &rogue_name_tex;
		break;
	case PALADIN:
		player2.portrait = &paladin_portrait;
		player2.name_tex = &paladin_name_tex;
		break;
	}
}

void characterSelScene::closeWindow()	{
	//This function has to receive which player is actually closing the window
	//By calling createCharacterButtons always, the buttons of the player that has not selected character are created again

	object_win_p1->to_delete = true;
	player1.has_selected_character = false;

	assignFocus();
}

void characterSelScene::popUpP1Window() {
	//First we destroy the already created buttons and clean the corresponding list
	std::list<Widgets*>::reverse_iterator it = App->gui->p1_focus_elements.rbegin();
	Widgets* object = nullptr;

	if (!object_win_p1)
	{
		for (it; it != App->gui->p1_focus_elements.rend(); it++)
		{
			object = *it;
			object->to_delete = true;
		}
		App->gui->p1_focus_elements.clear();
		App->entities->players[0]->focus = nullptr;

		object_win_p1 = (UiWindow*)App->gui->createWindow(OBJ_SELECTION, { 150, 310 }, this);
		select_object_p1 = (Labels*)App->gui->createLabel("Choose your Item", { 255,255,255,255 }, App->fonts->large_size, { 270, 330 }, this);
		affects_special1_p1 = (Labels*)App->gui->createLabel("Affects to:", { 40, 39, 39 }, App->fonts->medium_size, { 390, 480 }, this);
		affects_special2_p1 = (Labels*)App->gui->createLabel("Affects to:", { 40, 39, 39 }, App->fonts->medium_size, { 390,705 }, this);
		p1_select_item1 = (Buttons*)App->gui->createButton(SELECT_ITEM1, OBJECT_SELECTION, 0, { 170, 390 }, this);
		p1_select_item2 = (Buttons*)App->gui->createButton(SELECT_ITEM2, OBJECT_SELECTION, 0, { 170, 610 }, this);

		pugi::xml_node obj1_name;
		pugi::xml_node obj2_name;
		pugi::xml_node obj1_desc;
		pugi::xml_node obj2_desc;

		switch (player1.character)
		{
		default:
			break;
		case WARRIOR:
			obj1_name = labels_node.child("player_1").child("warrior_obj1");
			obj2_name = labels_node.child("player_1").child("warrior_obj2");
			obj1_desc = labels_node.child("player_1").child("warrior_obj1_desc");
			obj2_desc = labels_node.child("player_1").child("warrior_obj2_desc");
			obj1_name_p1 = (Labels*)App->gui->createLabel(obj1_name.child("content").attribute("value").as_string(), { (Uint8)obj1_name.child("color").attribute("r").as_int(),(Uint8)obj1_name.child("color").attribute("g").as_int(),(Uint8)obj1_name.child("color").attribute("b").as_int(),(Uint8)obj1_name.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj1_name.child("pos").attribute("x").as_int(), obj1_name.child("pos").attribute("y").as_int() }, this);

			obj2_name_p1 = (Labels*)App->gui->createLabel(obj2_name.child("content").attribute("value").as_string(), { (Uint8)obj2_name.child("color").attribute("r").as_int(),(Uint8)obj2_name.child("color").attribute("g").as_int(),(Uint8)obj2_name.child("color").attribute("b").as_int(),(Uint8)obj2_name.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj2_name.child("pos").attribute("x").as_int(), obj2_name.child("pos").attribute("y").as_int() }, this);

			obj1_desc_p1 = (Labels*)App->gui->createLabel(obj1_desc.child("content").attribute("value").as_string(), { (Uint8)obj1_desc.child("color").attribute("r").as_int(),(Uint8)obj1_desc.child("color").attribute("g").as_int(),(Uint8)obj1_desc.child("color").attribute("b").as_int(),(Uint8)obj1_desc.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj1_desc.child("pos").attribute("x").as_int(), obj1_desc.child("pos").attribute("y").as_int() }, this);

			obj2_desc_p1 = (Labels*)App->gui->createLabel(obj2_desc.child("content").attribute("value").as_string(), { (Uint8)obj2_desc.child("color").attribute("r").as_int(),(Uint8)obj2_desc.child("color").attribute("g").as_int(),(Uint8)obj2_desc.child("color").attribute("b").as_int(),(Uint8)obj2_desc.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj2_desc.child("pos").attribute("x").as_int(), obj2_desc.child("pos").attribute("y").as_int() }, this);
			break;
		case MAGE:
			obj1_name = labels_node.child("player_1").child("mage_obj1");
			obj2_name = labels_node.child("player_1").child("mage_obj2");
			obj1_desc = labels_node.child("player_1").child("mage_obj1_desc");
			obj2_desc = labels_node.child("player_1").child("mage_obj2_desc");
			obj1_name_p1 = (Labels*)App->gui->createLabel(obj1_name.child("content").attribute("value").as_string(), { (Uint8)obj1_name.child("color").attribute("r").as_int(),(Uint8)obj1_name.child("color").attribute("g").as_int(),(Uint8)obj1_name.child("color").attribute("b").as_int(),(Uint8)obj1_name.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj1_name.child("pos").attribute("x").as_int(), obj1_name.child("pos").attribute("y").as_int() }, this);

			obj2_name_p1 = (Labels*)App->gui->createLabel(obj2_name.child("content").attribute("value").as_string(), { (Uint8)obj2_name.child("color").attribute("r").as_int(),(Uint8)obj2_name.child("color").attribute("g").as_int(),(Uint8)obj2_name.child("color").attribute("b").as_int(),(Uint8)obj2_name.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj2_name.child("pos").attribute("x").as_int(), obj2_name.child("pos").attribute("y").as_int() }, this);

			obj1_desc_p1 = (Labels*)App->gui->createLabel(obj1_desc.child("content").attribute("value").as_string(), { (Uint8)obj1_desc.child("color").attribute("r").as_int(),(Uint8)obj1_desc.child("color").attribute("g").as_int(),(Uint8)obj1_desc.child("color").attribute("b").as_int(),(Uint8)obj1_desc.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj1_desc.child("pos").attribute("x").as_int(), obj1_desc.child("pos").attribute("y").as_int() }, this);

			obj2_desc_p1 = (Labels*)App->gui->createLabel(obj2_desc.child("content").attribute("value").as_string(), { (Uint8)obj2_desc.child("color").attribute("r").as_int(),(Uint8)obj2_desc.child("color").attribute("g").as_int(),(Uint8)obj2_desc.child("color").attribute("b").as_int(),(Uint8)obj2_desc.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj2_desc.child("pos").attribute("x").as_int(), obj2_desc.child("pos").attribute("y").as_int() }, this);
			break;
		case ROGUE:
			obj1_name = labels_node.child("player_1").child("rogue_obj1");
			obj2_name = labels_node.child("player_1").child("rogue_obj2");
			obj1_desc = labels_node.child("player_1").child("rogue_obj1_desc");
			obj2_desc = labels_node.child("player_1").child("rogue_obj2_desc");
			obj1_name_p1 = (Labels*)App->gui->createLabel(obj1_name.child("content").attribute("value").as_string(), { (Uint8)obj1_name.child("color").attribute("r").as_int(),(Uint8)obj1_name.child("color").attribute("g").as_int(),(Uint8)obj1_name.child("color").attribute("b").as_int(),(Uint8)obj1_name.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj1_name.child("pos").attribute("x").as_int(), obj1_name.child("pos").attribute("y").as_int() }, this);

			obj2_name_p1 = (Labels*)App->gui->createLabel(obj2_name.child("content").attribute("value").as_string(), { (Uint8)obj2_name.child("color").attribute("r").as_int(),(Uint8)obj2_name.child("color").attribute("g").as_int(),(Uint8)obj2_name.child("color").attribute("b").as_int(),(Uint8)obj2_name.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj2_name.child("pos").attribute("x").as_int(), obj2_name.child("pos").attribute("y").as_int() }, this);

			obj1_desc_p1 = (Labels*)App->gui->createLabel(obj1_desc.child("content").attribute("value").as_string(), { (Uint8)obj1_desc.child("color").attribute("r").as_int(),(Uint8)obj1_desc.child("color").attribute("g").as_int(),(Uint8)obj1_desc.child("color").attribute("b").as_int(),(Uint8)obj1_desc.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj1_desc.child("pos").attribute("x").as_int(), obj1_desc.child("pos").attribute("y").as_int() }, this);

			obj2_desc_p1 = (Labels*)App->gui->createLabel(obj2_desc.child("content").attribute("value").as_string(), { (Uint8)obj2_desc.child("color").attribute("r").as_int(),(Uint8)obj2_desc.child("color").attribute("g").as_int(),(Uint8)obj2_desc.child("color").attribute("b").as_int(),(Uint8)obj2_desc.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj2_desc.child("pos").attribute("x").as_int(), obj2_desc.child("pos").attribute("y").as_int() }, this);
			break;
		case PALADIN:
			obj1_name = labels_node.child("player_1").child("paladin_obj1");
			obj2_name = labels_node.child("player_1").child("paladin_obj2");
			obj1_desc = labels_node.child("player_1").child("paladin_obj1_desc");
			obj2_desc = labels_node.child("player_1").child("paladin_obj2_desc");
			obj1_name_p1 = (Labels*)App->gui->createLabel(obj1_name.child("content").attribute("value").as_string(), { (Uint8)obj1_name.child("color").attribute("r").as_int(),(Uint8)obj1_name.child("color").attribute("g").as_int(),(Uint8)obj1_name.child("color").attribute("b").as_int(),(Uint8)obj1_name.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj1_name.child("pos").attribute("x").as_int(), obj1_name.child("pos").attribute("y").as_int() }, this);

			obj2_name_p1 = (Labels*)App->gui->createLabel(obj2_name.child("content").attribute("value").as_string(), { (Uint8)obj2_name.child("color").attribute("r").as_int(),(Uint8)obj2_name.child("color").attribute("g").as_int(),(Uint8)obj2_name.child("color").attribute("b").as_int(),(Uint8)obj2_name.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj2_name.child("pos").attribute("x").as_int(), obj2_name.child("pos").attribute("y").as_int() }, this);

			obj1_desc_p1 = (Labels*)App->gui->createLabel(obj1_desc.child("content").attribute("value").as_string(), { (Uint8)obj1_desc.child("color").attribute("r").as_int(),(Uint8)obj1_desc.child("color").attribute("g").as_int(),(Uint8)obj1_desc.child("color").attribute("b").as_int(),(Uint8)obj1_desc.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj1_desc.child("pos").attribute("x").as_int(), obj1_desc.child("pos").attribute("y").as_int() }, this);

			obj2_desc_p1 = (Labels*)App->gui->createLabel(obj2_desc.child("content").attribute("value").as_string(), { (Uint8)obj2_desc.child("color").attribute("r").as_int(),(Uint8)obj2_desc.child("color").attribute("g").as_int(),(Uint8)obj2_desc.child("color").attribute("b").as_int(),(Uint8)obj2_desc.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj2_desc.child("pos").attribute("x").as_int(), obj2_desc.child("pos").attribute("y").as_int() }, this);
			break;
		}
	}
}

void characterSelScene::popUpP2Window()	{
	std::list<Widgets*>::reverse_iterator it = App->gui->p2_focus_elements.rbegin();
	Widgets* object = nullptr;

	if (!object_win_p2 && player2.has_selected_character)
	{
		for (it; it != App->gui->p2_focus_elements.rend(); it++)
		{
			object = *it;
			object->to_delete = true;
		}
		App->gui->p2_focus_elements.clear();
		App->entities->players[1]->focus = nullptr;

		object_win_p2 = (UiWindow*)App->gui->createWindow(OBJ_SELECTION, { 1140, 310 }, this);
		select_object_p2 = (Labels*)App->gui->createLabel("Choose your Item", { 255,255,255,255 }, App->fonts->large_size, { 1260, 330 }, this);
		affects_special1_p2 = (Labels*)App->gui->createLabel("Affects to:", { 40, 39, 39 }, App->fonts->medium_size, { 1380, 480 }, this);
		affects_special2_p2 = (Labels*)App->gui->createLabel("Affects to:", { 40, 39, 39 }, App->fonts->medium_size, { 1380,705 }, this);
		p2_select_item1 = (Buttons*)App->gui->createButton(SELECT_ITEM1, OBJECT_SELECTION, 1, { 1160, 390 }, this);
		p2_select_item2 = (Buttons*)App->gui->createButton(SELECT_ITEM2, OBJECT_SELECTION, 1, { 1160, 610 }, this);

		pugi::xml_node obj1_name;
		pugi::xml_node obj2_name;
		pugi::xml_node obj1_desc;
		pugi::xml_node obj2_desc;

		switch (player2.character)
		{
		default:
			break;
		case WARRIOR:
			obj1_name = labels_node.child("player_2").child("warrior_obj1");
			obj2_name = labels_node.child("player_2").child("warrior_obj2");
			obj1_desc = labels_node.child("player_2").child("warrior_obj1_desc");
			obj2_desc = labels_node.child("player_2").child("warrior_obj2_desc");
			obj1_name_p2 = (Labels*)App->gui->createLabel(obj1_name.child("content").attribute("value").as_string(), { (Uint8)obj1_name.child("color").attribute("r").as_int(),(Uint8)obj1_name.child("color").attribute("g").as_int(),(Uint8)obj1_name.child("color").attribute("b").as_int(),(Uint8)obj1_name.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj1_name.child("pos").attribute("x").as_int(), obj1_name.child("pos").attribute("y").as_int() }, this);

			obj2_name_p2 = (Labels*)App->gui->createLabel(obj2_name.child("content").attribute("value").as_string(), { (Uint8)obj2_name.child("color").attribute("r").as_int(),(Uint8)obj2_name.child("color").attribute("g").as_int(),(Uint8)obj2_name.child("color").attribute("b").as_int(),(Uint8)obj2_name.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj2_name.child("pos").attribute("x").as_int(), obj2_name.child("pos").attribute("y").as_int() }, this);

			obj1_desc_p2 = (Labels*)App->gui->createLabel(obj1_desc.child("content").attribute("value").as_string(), { (Uint8)obj1_desc.child("color").attribute("r").as_int(),(Uint8)obj1_desc.child("color").attribute("g").as_int(),(Uint8)obj1_desc.child("color").attribute("b").as_int(),(Uint8)obj1_desc.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj1_desc.child("pos").attribute("x").as_int(), obj1_desc.child("pos").attribute("y").as_int() }, this);

			obj2_desc_p2 = (Labels*)App->gui->createLabel(obj2_desc.child("content").attribute("value").as_string(), { (Uint8)obj2_desc.child("color").attribute("r").as_int(),(Uint8)obj2_desc.child("color").attribute("g").as_int(),(Uint8)obj2_desc.child("color").attribute("b").as_int(),(Uint8)obj2_desc.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj2_desc.child("pos").attribute("x").as_int(), obj2_desc.child("pos").attribute("y").as_int() }, this);
			break;
		case MAGE:
			obj1_name = labels_node.child("player_2").child("mage_obj1");
			obj2_name = labels_node.child("player_2").child("mage_obj2");
			obj1_desc = labels_node.child("player_2").child("mage_obj1_desc");
			obj2_desc = labels_node.child("player_2").child("mage_obj2_desc");
			obj1_name_p2 = (Labels*)App->gui->createLabel(obj1_name.child("content").attribute("value").as_string(), { (Uint8)obj1_name.child("color").attribute("r").as_int(),(Uint8)obj1_name.child("color").attribute("g").as_int(),(Uint8)obj1_name.child("color").attribute("b").as_int(),(Uint8)obj1_name.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj1_name.child("pos").attribute("x").as_int(), obj1_name.child("pos").attribute("y").as_int() }, this);

			obj2_name_p2 = (Labels*)App->gui->createLabel(obj2_name.child("content").attribute("value").as_string(), { (Uint8)obj2_name.child("color").attribute("r").as_int(),(Uint8)obj2_name.child("color").attribute("g").as_int(),(Uint8)obj2_name.child("color").attribute("b").as_int(),(Uint8)obj2_name.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj2_name.child("pos").attribute("x").as_int(), obj2_name.child("pos").attribute("y").as_int() }, this);

			obj1_desc_p2 = (Labels*)App->gui->createLabel(obj1_desc.child("content").attribute("value").as_string(), { (Uint8)obj1_desc.child("color").attribute("r").as_int(),(Uint8)obj1_desc.child("color").attribute("g").as_int(),(Uint8)obj1_desc.child("color").attribute("b").as_int(),(Uint8)obj1_desc.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj1_desc.child("pos").attribute("x").as_int(), obj1_desc.child("pos").attribute("y").as_int() }, this);

			obj2_desc_p2 = (Labels*)App->gui->createLabel(obj2_desc.child("content").attribute("value").as_string(), { (Uint8)obj2_desc.child("color").attribute("r").as_int(),(Uint8)obj2_desc.child("color").attribute("g").as_int(),(Uint8)obj2_desc.child("color").attribute("b").as_int(),(Uint8)obj2_desc.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj2_desc.child("pos").attribute("x").as_int(), obj2_desc.child("pos").attribute("y").as_int() }, this);
			break;
		case ROGUE:
			obj1_name = labels_node.child("player_2").child("rogue_obj1");
			obj2_name = labels_node.child("player_2").child("rogue_obj2");
			obj1_desc = labels_node.child("player_2").child("rogue_obj1_desc");
			obj2_desc = labels_node.child("player_2").child("rogue_obj2_desc");
			obj1_name_p2 = (Labels*)App->gui->createLabel(obj1_name.child("content").attribute("value").as_string(), { (Uint8)obj1_name.child("color").attribute("r").as_int(),(Uint8)obj1_name.child("color").attribute("g").as_int(),(Uint8)obj1_name.child("color").attribute("b").as_int(),(Uint8)obj1_name.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj1_name.child("pos").attribute("x").as_int(), obj1_name.child("pos").attribute("y").as_int() }, this);

			obj2_name_p2 = (Labels*)App->gui->createLabel(obj2_name.child("content").attribute("value").as_string(), { (Uint8)obj2_name.child("color").attribute("r").as_int(),(Uint8)obj2_name.child("color").attribute("g").as_int(),(Uint8)obj2_name.child("color").attribute("b").as_int(),(Uint8)obj2_name.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj2_name.child("pos").attribute("x").as_int(), obj2_name.child("pos").attribute("y").as_int() }, this);

			obj1_desc_p2 = (Labels*)App->gui->createLabel(obj1_desc.child("content").attribute("value").as_string(), { (Uint8)obj1_desc.child("color").attribute("r").as_int(),(Uint8)obj1_desc.child("color").attribute("g").as_int(),(Uint8)obj1_desc.child("color").attribute("b").as_int(),(Uint8)obj1_desc.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj1_desc.child("pos").attribute("x").as_int(), obj1_desc.child("pos").attribute("y").as_int() }, this);

			obj2_desc_p2 = (Labels*)App->gui->createLabel(obj2_desc.child("content").attribute("value").as_string(), { (Uint8)obj2_desc.child("color").attribute("r").as_int(),(Uint8)obj2_desc.child("color").attribute("g").as_int(),(Uint8)obj2_desc.child("color").attribute("b").as_int(),(Uint8)obj2_desc.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj2_desc.child("pos").attribute("x").as_int(), obj2_desc.child("pos").attribute("y").as_int() }, this);
			break;
		case PALADIN:
			obj1_name = labels_node.child("player_2").child("paladin_obj1");
			obj2_name = labels_node.child("player_2").child("paladin_obj2");
			obj1_desc = labels_node.child("player_2").child("paladin_obj1_desc");
			obj2_desc = labels_node.child("player_2").child("paladin_obj2_desc");
			obj1_name_p2 = (Labels*)App->gui->createLabel(obj1_name.child("content").attribute("value").as_string(), { (Uint8)obj1_name.child("color").attribute("r").as_int(),(Uint8)obj1_name.child("color").attribute("g").as_int(),(Uint8)obj1_name.child("color").attribute("b").as_int(),(Uint8)obj1_name.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj1_name.child("pos").attribute("x").as_int(), obj1_name.child("pos").attribute("y").as_int() }, this);

			obj2_name_p2 = (Labels*)App->gui->createLabel(obj2_name.child("content").attribute("value").as_string(), { (Uint8)obj2_name.child("color").attribute("r").as_int(),(Uint8)obj2_name.child("color").attribute("g").as_int(),(Uint8)obj2_name.child("color").attribute("b").as_int(),(Uint8)obj2_name.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj2_name.child("pos").attribute("x").as_int(), obj2_name.child("pos").attribute("y").as_int() }, this);

			obj1_desc_p2 = (Labels*)App->gui->createLabel(obj1_desc.child("content").attribute("value").as_string(), { (Uint8)obj1_desc.child("color").attribute("r").as_int(),(Uint8)obj1_desc.child("color").attribute("g").as_int(),(Uint8)obj1_desc.child("color").attribute("b").as_int(),(Uint8)obj1_desc.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj1_desc.child("pos").attribute("x").as_int(), obj1_desc.child("pos").attribute("y").as_int() }, this);

			obj2_desc_p2 = (Labels*)App->gui->createLabel(obj2_desc.child("content").attribute("value").as_string(), { (Uint8)obj2_desc.child("color").attribute("r").as_int(),(Uint8)obj2_desc.child("color").attribute("g").as_int(),(Uint8)obj2_desc.child("color").attribute("b").as_int(),(Uint8)obj2_desc.child("color").attribute("a").as_int() },
				App->fonts->medium_size, { obj2_desc.child("pos").attribute("x").as_int(), obj2_desc.child("pos").attribute("y").as_int() }, this);
			break;
		}
	}
}

void characterSelScene::closeP1Window()	{
	std::list<Widgets*>::reverse_iterator it = App->gui->p1_focus_elements.rbegin();
	Widgets* object = nullptr;

	for (it; it != App->gui->p1_focus_elements.rend(); it++)
	{
		object = *it;
		object->to_delete = true;
	}
	App->gui->p1_focus_elements.clear();
	App->entities->players[0]->focus = nullptr;

	object_win_p1->to_delete = true;
	object_win_p1 = nullptr;
	select_object_p1->to_delete = true;
	affects_special1_p1->to_delete = true;
	affects_special2_p1->to_delete = true;
	obj1_name_p1->to_delete = true;
	obj2_name_p1->to_delete = true;
	obj1_desc_p1->to_delete = true;
	obj2_desc_p1->to_delete = true;
	
	if (!player1.has_selected_item)
	{
		player1.has_selected_character = false;
		createP1CharButtons();
		assignFocus();
	}
	
}

void characterSelScene::closeP2Window()	{
	std::list<Widgets*>::reverse_iterator it = App->gui->p2_focus_elements.rbegin();
	Widgets* object = nullptr;

	for (it; it != App->gui->p2_focus_elements.rend(); it++)
	{
		object = *it;
		object->to_delete = true;
	}
	App->gui->p2_focus_elements.clear();
	App->entities->players[1]->focus = nullptr;

	object_win_p2->to_delete = true;
	object_win_p2 = nullptr;
	select_object_p2->to_delete = true;
	affects_special1_p2->to_delete = true;
	affects_special2_p2->to_delete = true;
	obj1_name_p2->to_delete = true;
	obj2_name_p2->to_delete = true;
	obj1_desc_p2->to_delete = true;
	obj2_desc_p2->to_delete = true;
	
	if (!player2.has_selected_item)
	{
		player2.has_selected_character = false;
		createP2CharButtons();
		assignFocus();
	}
}

void characterSelScene::startingTransition()	{

	if (transition_timer.readSec() >= 2)
		App->scene_manager->changeScene(App->scene_manager->combat_scene, this);

}

