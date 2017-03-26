#include "ParticleSystemScene.h"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"


USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;
using namespace CocosDenshion;

Scene* ParticleSystemScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ParticleSystemScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

ParticleSystemScene::~ParticleSystemScene()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("res/particletexture.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("particletexture.plist");
	this->removeAllChildren();
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool ParticleSystemScene::init()
{
	Size size;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    if ( !Layer::init() )
    {
        return false;
    }

    auto rootNode = CSLoader::createNode("MainScene.csb");
    addChild(rootNode);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("particletexture.plist");
	Sprite *Fire_On = Sprite::createWithSpriteFrameName("emittertd.png");


	// For Emitter
	_EmitterSprite = CDraggableSprite::create();
	_EmitterSprite->setSpriteInfo("emittericon.png", Point(-125.0f + visibleSize.width / 2.0f, visibleSize.height / 2.0f));
	_EmitterSprite->setVisible(false);
	_bEmitterOn = false;
	this->addChild(_EmitterSprite, 5);

	auto emiterpos = (Sprite *)(rootNode->getChildByName("emitterpos"));
	Point loc = emiterpos->getPosition();//emitterpos的位置 = loc
	emiterpos->setVisible(false);
	_emitterSwitchBtn = CSwitchButton::create();
	_emitterSwitchBtn->setButtonInfo("emitteroff.png", "emitteron.png", "emittertd.png", loc);
	this->addChild(_emitterSwitchBtn, 2);

	// Particle Control System
	// 最好的方式是，以下的 Slider 根據這裡的設定值，顯示出正確的數值與位置
	_ParticleControl.init(*this);//CParticleSystem _ParticleControl; 的初始化，產生一系列的分子加在場景中，this是指ParticleSystemScene的場景
	_ParticleControl._emitterPt = _EmitterSprite->getLoc();//取得mitter 目前的位置
	_ParticleControl._fGravity = 0;//重力 0 = 50%
	_ParticleControl._fSpread = 180.0f;//噴出來範圍多大 180.0 = 100%
	_ParticleControl._fDir = 90.0;//預設方向90度，25% = 90度
	_ParticleControl._fSpin = 0;//旋轉 0 = 0%
	_ParticleControl._fOpacity = 255;//不透明度100%，預設值255
	_ParticleControl._fParticles = 100;//分子數量50%，預設值100
	_ParticleControl._fVelocity = 2;	 // 分子的離開速度(2.5改成2，為了配合cocos2d預設) 5%= 2
	_ParticleControl._fLifeTime = 3.5f;	 // 分子的存活時間，50% = 3.5f
	_ParticleControl._fRed = 255;
	_ParticleControl._fGreen = 255;
	_ParticleControl._fBlue = 255;
	_ParticleControl._iNumParticles = 100;//預設有100個
	_ParticleControl._iGenParticles = 0;// 目前時間內已經產生的分子個數，當使用Emitter時他會切換數量會增加，目前沒開啟Emitter
	_ParticleControl._fElpasedTime = 0;//目前過了多少時間
	_ParticleControl.setType(STAY_FOR_TWOSECONDS); // 目前分子的運動型態，預設為 0，STAY_FOR_TWOSECONDS = 按一下出現，停留兩秒鐘消失不見
	_ParticleControl._windDir = Point(0, 0); // 本範例沒有實作此項功能(風力風向)
	_ParticleControl._fWind = 0;
	// 分子的可調整參數的捲動軸
	// Slider of Gravity
	auto *GravitySlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Gravity"));
	GravitySlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::GravityEvent, this));
	GravitySlider->setMaxPercent(100); 	// 將 0 到 100 對應到 -10 到 10 之間
	_GravityBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("GravityBMFont");

	// Slider of Spread
	auto *SpreadSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Spread"));
	SpreadSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::SpreadEvent, this));
	SpreadSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 180 之間
	_SpreadBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("SpreadBMFont");

	// Slider of Direction
	auto *DirectionSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Direction"));
	DirectionSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::DirectionEvent, this));
	DirectionSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 360 之間
	_DirectionBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("DirectionBMFont");
	
	// Slider of Spin
	auto *SpinSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Spin"));
	SpinSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::SpinEvent, this));
	SpinSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 360 之間
	_SpinBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("SpinBMFont");

	// Slider of Opacity
	auto *OpacitySlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Opacity"));
	OpacitySlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::OpacityEvent, this));
	OpacitySlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 255 之間
	_OpacityBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("OpacityBMFont");

	// Slider of Particles
	auto *ParticlesSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Particles"));
	ParticlesSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::ParticlesEvent, this));
	ParticlesSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 200 之間
	_ParticlesBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("ParticlesBMFont");

	// Slider of Speed
	auto *SpeedSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Speed"));
	SpeedSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::SpeedEvent, this));
	SpeedSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 40 之間
	_SpeedBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("SpeedBMFont");

	// Slider of Lifetime
	auto *LifetimeSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Lifetime"));
	LifetimeSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::LifetimeEvent, this));
	LifetimeSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 7 之間
	_LifetimeBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("LifetimeBMFont");

	// Slider of Red
	auto *RedSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Red"));
	RedSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::RedEvent, this));
	RedSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 255  之間
	_RedBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("RedBMFont");

	// Slider of Green
	auto *GreenSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Green"));
	GreenSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::GreenEvent, this));
	GreenSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 255  之間
	_GreenBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("GreenBMFont");

	// Slider of Blue
	auto *BlueSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Blue"));
	BlueSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::BlueEvent, this));
	BlueSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 255  之間
	_BlueBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("BlueBMFont");

	// Slider of Wind
	auto *WindSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Wind"));
	WindSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::WindEvent, this));
	WindSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 255  之間
	_WindBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("WindBMFont");

	// Slider of Type
	auto *TypeSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Type"));
	TypeSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::TypeEvent, this));
	TypeSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 5 之間
	_TypeBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("TypeBMFont");
	// Slider of Emitter_Type
	auto *Emitter_TypeSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Emitter_Type"));
	Emitter_TypeSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::Emitter_TypeEvent, this));
	Emitter_TypeSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 3 之間
	_Emitter_TypeBMFont = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("Emitter_TypeBMFont");
	//案件BUBBLE
	
	Button *BUBBLEbtn = (cocos2d::ui::Button *)(rootNode->getChildByName("BUBBLEbtn"));
	BUBBLEbtn->addTouchEventListener(CC_CALLBACK_2(ParticleSystemScene::BUBBLEbtnTouchEvent, this));
	//案件CIRCLE
	
	Button *CIRCLEbtn = (cocos2d::ui::Button *)(rootNode->getChildByName("CIRCLEbtn"));
	CIRCLEbtn->addTouchEventListener(CC_CALLBACK_2(ParticleSystemScene::CIRCLEbtnTouchEvent, this));
	//案件BUBBLE
	
	Button *CLOUDbtn = (cocos2d::ui::Button *)(rootNode->getChildByName("CLOUDbtn"));
	CLOUDbtn->addTouchEventListener(CC_CALLBACK_2(ParticleSystemScene::CLOUDbtnTouchEvent, this));
	//案件BUBBLE

	Button *COMETbtn = (cocos2d::ui::Button *)(rootNode->getChildByName("COMETbtn"));
	COMETbtn->addTouchEventListener(CC_CALLBACK_2(ParticleSystemScene::COMETbtnTouchEvent, this));
	//案件BUBBLE
	
	Button *RAINDROPbtn = (cocos2d::ui::Button *)(rootNode->getChildByName("RAINDROPbtn"));
	RAINDROPbtn->addTouchEventListener(CC_CALLBACK_2(ParticleSystemScene::RAINDROPbtnTouchEvent, this));
	//案件SPARK
	
	Button *SPARKbtn = (cocos2d::ui::Button *)(rootNode->getChildByName("SPARKbtn"));
	SPARKbtn->addTouchEventListener(CC_CALLBACK_2(ParticleSystemScene::SPARKbtnTouchEvent, this));


	_listener1 = EventListenerTouchOneByOne::create();	//創建一個一對一的事件聆聽器
	_listener1->onTouchBegan = CC_CALLBACK_2(ParticleSystemScene::onTouchBegan, this);		//加入觸碰開始事件
	_listener1->onTouchMoved = CC_CALLBACK_2(ParticleSystemScene::onTouchMoved, this);		//加入觸碰移動事件
	_listener1->onTouchEnded = CC_CALLBACK_2(ParticleSystemScene::onTouchEnded, this);		//加入觸碰離開事件

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//加入剛創建的事件聆聽器
	this->schedule(CC_SCHEDULE_SELECTOR(ParticleSystemScene::doStep));//註冊doStep

    return true;
}

void ParticleSystemScene::doStep(float dt)
{
	_ParticleControl.doStep(dt);//呼叫控制系統跟他說過了多少時間，然後要幹嘛....
}


bool ParticleSystemScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//觸碰開始事件
{
	Point touchLoc = pTouch->getLocation();

	
	//顯示 Emitter 時，可拖曳該圖式
	if( _bEmitterOn ) _EmitterSprite->touchesBegan(touchLoc);////當按下那一瞬間，先確定目前Emitter是開還是關，如果是，表示你目前可能會點在EmitterSprite(紅圈發射器圖示)身上，或你可能會拖曳它
	// 沒有顯示 Emitter，而且沒有按在 Emitter 切換按鈕上，才讓 touch 可以點選顯示分子(Particle)
	if ( !_emitterSwitchBtn->touchesBegan(touchLoc) && !_bEmitterOn ) _ParticleControl.onTouchesBegan(touchLoc);//沒點在Emitter按鈕上&Emitter之前也沒開啟，點哪就在哪產生 _Particle
	return true;
}

void  ParticleSystemScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰移動事件
{
	Point touchLoc = pTouch->getLocation();	
	if (_bEmitterOn) {//當按下那一瞬間，先確定目前Emitter是開還是關
		if (_EmitterSprite->touchesMoved(touchLoc)) //  移動並更新 Emitter 的位置
			_ParticleControl._emitterPt = _EmitterSprite->getLoc();//隨著發射器移動，ParticleControl隨時定位新的位子，並設定發射器的位置
	}
	// 沒有顯示 Emitter，而且沒有按在 Emitter 切換按鈕上，才讓 touch 可以點選顯示分子
	if ( !_emitterSwitchBtn->touchesMoved(touchLoc) && !_bEmitterOn ) _ParticleControl.onTouchesMoved(touchLoc);//_ParticleControl持續控制onTouchesMoved，改定位就改分子的產聲地方
}

void  ParticleSystemScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰結束事件 
{
	Point touchLoc = pTouch->getLocation();

	if (_bEmitterOn) {//確定目前是開還是開
		if( _EmitterSprite->touchesEnded(touchLoc) ) 
			_ParticleControl._emitterPt = _EmitterSprite->getLoc();
	}

	// 點在 Emitter 切換的圖示上，進行必要的狀態改變
	if (_emitterSwitchBtn->touchesEnded(touchLoc))//當放掉的一瞬間要檢查滑鼠是否點在EmitterSwitchBtn上面
	{
		_bEmitterOn = _emitterSwitchBtn->getStatus();//取得狀態並告訴bEmitterOn當"按下之後"的emitterSwitchBtn現在是開還是關
		if ( _bEmitterOn ) { // 顯示 Emitter 圖示
			_EmitterSprite->setVisible(true);			
		}
		else { // 關閉 Emitter 圖示
			_EmitterSprite->setVisible(false);
		}
		_ParticleControl.setEmitter(_bEmitterOn); // 更新控制系統中的 Emitter 狀態
	}
}

void ParticleSystemScene::GravityEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)//在拖圓點的過程中被呼叫近來，檢查目前是否在改變slider
	{
		Slider* slider = dynamic_cast<Slider*>(sender);//用這行取得slider
		int percent = slider->getPercent();//取得目前拖到的比例
		float fGravity = (-50.0f + percent) / 5.0f;//-10~10之間
		_GravityBMValue->setString(StringUtils::format("%2.1f", fGravity));//透過 _GravityBMValue將目前改變結果螢幕上直接顯示出來，setString()=把值轉成字串，StringUtils::format() = printf()
		_ParticleControl.setGravity(fGravity);//呼叫 _ParticleControl 的 setGravity 函式，以反映目前重力的改變
	}
}

void ParticleSystemScene::SpreadEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fSpread = percent*1.8f; // 0 到 180 之間
		_SpreadBMValue->setString(StringUtils::format("%2.1f", fSpread));
		_ParticleControl._fSpread = fSpread;
	}
}

void ParticleSystemScene::DirectionEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fDir = percent*3.6f; // 0 到 360 之間
		_DirectionBMValue->setString(StringUtils::format("%2.1f", fDir));
		_ParticleControl.setDir(fDir);
	}
}

void ParticleSystemScene::SpinEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fSpin= percent*3.6f; // 0 到 360 之間 
		_SpinBMValue->setString(StringUtils::format("%2.1f", fSpin));
		_ParticleControl.setSpin(fSpin);
		
	}
}

void ParticleSystemScene::OpacityEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fOpacity = percent*2.55f; // 0 到 255 之間
		_OpacityBMValue->setString(StringUtils::format("%2.1f", fOpacity));
		_ParticleControl.setOpacity(fOpacity);
	}
}

void ParticleSystemScene::ParticlesEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fParticles = percent*2; // 0 到 200 之間
		_ParticlesBMValue->setString(StringUtils::format("%2.1f", fParticles));
		_ParticleControl._iNumParticles = fParticles;
	}
}

void ParticleSystemScene::SpeedEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fSpeed = percent*0.4f; // 0 到 40 之間
		_SpeedBMValue->setString(StringUtils::format("%2.1f", fSpeed));
		_ParticleControl.setSpeed(fSpeed);
	}
}
void ParticleSystemScene::LifetimeEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fLifetime = percent*0.07f; // 0 到 7 之間
		_LifetimeBMValue->setString(StringUtils::format("%2.1f", fLifetime));
		_ParticleControl.setLifetime(fLifetime);
	}
}

void ParticleSystemScene::RedEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fRed = percent*2.55f; // 0 到 255 之間
		_RedBMValue->setString(StringUtils::format("%2.1f", fRed));
		_ParticleControl._fRed = fRed;
	}
}

void ParticleSystemScene::GreenEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fGreen = percent*2.55f; // 0 到 255 之間
		_GreenBMValue->setString(StringUtils::format("%2.1f", fGreen));
		_ParticleControl._fGreen = fGreen;
	}
}

void ParticleSystemScene::BlueEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fBlue = percent*2.55f; // 0 到 255 之間
		_BlueBMValue->setString(StringUtils::format("%2.1f", fBlue));
		_ParticleControl._fBlue = fBlue;
	}
}
void ParticleSystemScene::WindEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int maxPercent = slider->getMaxPercent(); 
		int percent = slider->getPercent();//取得目前拖到的比例
		float fWind = (-50.0f + percent) / 5.0f;//-10~10之間
		_WindBMValue->setString(StringUtils::format("%2.1f", fWind));
		_ParticleControl._fWind = fWind;
	}
}


void ParticleSystemScene::TypeEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		int iType = percent/12; // 0 到 9 之間
		_TypeBMValue->setString(StringUtils::format("%2d", iType));
		_ParticleControl.setType(iType);
	}
}
void ParticleSystemScene::Emitter_TypeEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		int iEmitter_Type = percent / 25; // 0 到 3 之間
		_Emitter_TypeBMFont->setString(StringUtils::format("%2d", iEmitter_Type));
		_ParticleControl.setEmitter_Type(iEmitter_Type);
	}
}
void ParticleSystemScene::BUBBLEbtnTouchEvent(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		_ParticleControl.ChangeSprite(1, *this);
		log("Touch Down"); break;
	
		
	default: break;
	}
}
void ParticleSystemScene::CIRCLEbtnTouchEvent(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		_ParticleControl.ChangeSprite(2, *this);
		log("Touch Down"); break;
	
	default: break;
	}
}
void ParticleSystemScene::CLOUDbtnTouchEvent(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		_ParticleControl.ChangeSprite(3, *this);
		log("Touch Down"); break;
	
	default: break;
	}
}
void ParticleSystemScene::COMETbtnTouchEvent(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		_ParticleControl.ChangeSprite(4, *this);
		log("Touch Down"); break;
	
	default: break;
	}
}
void ParticleSystemScene::RAINDROPbtnTouchEvent(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		_ParticleControl.ChangeSprite(5, *this);
		log("Touch Down"); break;
	
	default: break;
	}
}
void ParticleSystemScene::SPARKbtnTouchEvent(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		_ParticleControl.ChangeSprite(6, *this);
		log("Touch Down"); break;
	
	default: break;
	}
}