// (C) 2015 Turnro.com

#include "GameScene.h"
#include "DDConfig.h"
#include "ScatOrcScene.h"
#include "DDMapData.h"


USING_NS_CC;

bool GameScene::init()
{
    assert(TRBaseScene::init());


    if (DDMapData::s()->hasSavedGame()) {
        CCLOG("has saved game");
        DDMapData::s()->loadMinmapTemplates();
        DDMapData::s()->loadPresentingMinmapsFromSavedGame();
    } else {
        CCLOG("no saved game, init game");
        DDMapData::s()->randomChooseTemplates();
        DDMapData::s()->loadMinmapTemplates();
        DDMapData::s()->loadPresentingMinmapsForNew();
    }

    initBaseUI();
    initBigmap();
    initBattleField();
    initBuildingField();
    initPropertyField();

    _bigmap.configProtocals(&_battleField);
    DDMapData::s()->configProtocals(&_battleField);

    DDMapData::s()->isBattleFieldObserving = true;

    /*
    addCommonBtn({0.5,0.1}, "battle dlg", [this](){
        this->battleDialog("this is message", "action", [](){
            CCLOG("test dlg func");
        });
    });
     */
    addCommonBtn({0.5,0.1}, "reset", [this](){
        Director::getInstance()->replaceScene(GameScene::create());
    });

    addCommonBtn({0.25,0.1}, "refresh", [this](){
        _battleField.configAnimationTexture("/Users/chenbingfeng/Documents/NormalMapPreviewer/0/animation.png");
    });


    _defaultLayer->setZOrder(1000);

    scheduleUpdate();


    return true;
}


void GameScene::initBigmap()
{
    _bigmapLayer = Layer::create();
    this->addChild(_bigmapLayer);
    _bigmapLayer->setPosition(DDConfig::bigmapAreaCenter());
    _bigmapLayer->setZOrder(LZ_BIGMAP);
    _bigmap.init(_bigmapLayer, _generalLayer);
    _bigmapLayer->setVisible(false);
}


void GameScene::initBaseUI()
{
    _generalLayer = Layer::create();
    this->addChild(_generalLayer);
    _generalLayer->setZOrder(LZ_GENERAL);


    auto spBaseCover = Sprite::create("images/cover.png");
    spBaseCover->setPosition(genPos({0.5,0.5}));
    _generalLayer->addChild(spBaseCover);
}

void GameScene::initBuildingField()
{
    _buildingLayer = Layer::create();
    this->addChild(_buildingLayer);
    _buildingLayer->setZOrder(LZ_BUILDING);
    _buildingField.init(_buildingLayer);
    _buildingLayer->setVisible(false);
}

void GameScene::initPropertyField()
{
    _propertyLayer = Layer::create();
    this->addChild(_propertyLayer);
    _propertyLayer->setZOrder(LZ_PROPERTY);
    _propertyField.init(_propertyLayer);
    _propertyLayer->setVisible(false);
}

void GameScene::update(float dt)
{
    static int CNT = 0;
    _battleField.step(dt);
    CNT++;
    if (CNT%10 == 0) {
        _bigmap.step(dt);
    }
    if (CNT%3 == 0) {
        DDMapData::s()->gameTick();
    }
}
