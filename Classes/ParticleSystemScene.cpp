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
	Point loc = emiterpos->getPosition();//emitterpos����m = loc
	emiterpos->setVisible(false);
	_emitterSwitchBtn = CSwitchButton::create();
	_emitterSwitchBtn->setButtonInfo("emitteroff.png", "emitteron.png", "emittertd.png", loc);
	this->addChild(_emitterSwitchBtn, 2);

	// Particle Control System
	// �̦n���覡�O�A�H�U�� Slider �ھڳo�̪��]�w�ȡA��ܥX���T���ƭȻP��m
	_ParticleControl.init(*this);//CParticleSystem _ParticleControl; ����l�ơA���ͤ@�t�C�����l�[�b�������Athis�O��ParticleSystemScene������
	_ParticleControl._emitterPt = _EmitterSprite->getLoc();//���omitter �ثe����m
	_ParticleControl._fGravity = 0;//���O 0 = 50%
	_ParticleControl._fSpread = 180.0f;//�Q�X�ӽd��h�j 180.0 = 100%
	_ParticleControl._fDir = 90.0;//�w�]��V90�סA25% = 90��
	_ParticleControl._fSpin = 0;//���� 0 = 0%
	_ParticleControl._fOpacity = 255;//���z����100%�A�w�]��255
	_ParticleControl._fParticles = 100;//���l�ƶq50%�A�w�]��100
	_ParticleControl._fVelocity = 2;	 // ���l�����}�t��(2.5�令2�A���F�t�Xcocos2d�w�]) 5%= 2
	_ParticleControl._fLifeTime = 3.5f;	 // ���l���s���ɶ��A50% = 3.5f
	_ParticleControl._fRed = 255;
	_ParticleControl._fGreen = 255;
	_ParticleControl._fBlue = 255;
	_ParticleControl._iNumParticles = 100;//�w�]��100��
	_ParticleControl._iGenParticles = 0;// �ثe�ɶ����w�g���ͪ����l�ӼơA��ϥ�Emitter�ɥL�|�����ƶq�|�W�[�A�ثe�S�}��Emitter
	_ParticleControl._fElpasedTime = 0;//�ثe�L�F�h�֮ɶ�
	_ParticleControl.setType(STAY_FOR_TWOSECONDS); // �ثe���l���B�ʫ��A�A�w�]�� 0�ASTAY_FOR_TWOSECONDS = ���@�U�X�{�A���d�������������
	_ParticleControl._windDir = Point(0, 0); // ���d�ҨS����@�����\��(���O���V)
	_ParticleControl._fWind = 0;
	// ���l���i�վ�Ѽƪ����ʶb
	// Slider of Gravity
	auto *GravitySlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Gravity"));
	GravitySlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::GravityEvent, this));
	GravitySlider->setMaxPercent(100); 	// �N 0 �� 100 ������ -10 �� 10 ����
	_GravityBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("GravityBMFont");

	// Slider of Spread
	auto *SpreadSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Spread"));
	SpreadSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::SpreadEvent, this));
	SpreadSlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 180 ����
	_SpreadBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("SpreadBMFont");

	// Slider of Direction
	auto *DirectionSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Direction"));
	DirectionSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::DirectionEvent, this));
	DirectionSlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 360 ����
	_DirectionBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("DirectionBMFont");
	
	// Slider of Spin
	auto *SpinSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Spin"));
	SpinSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::SpinEvent, this));
	SpinSlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 360 ����
	_SpinBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("SpinBMFont");

	// Slider of Opacity
	auto *OpacitySlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Opacity"));
	OpacitySlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::OpacityEvent, this));
	OpacitySlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 255 ����
	_OpacityBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("OpacityBMFont");

	// Slider of Particles
	auto *ParticlesSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Particles"));
	ParticlesSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::ParticlesEvent, this));
	ParticlesSlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 200 ����
	_ParticlesBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("ParticlesBMFont");

	// Slider of Speed
	auto *SpeedSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Speed"));
	SpeedSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::SpeedEvent, this));
	SpeedSlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 40 ����
	_SpeedBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("SpeedBMFont");

	// Slider of Lifetime
	auto *LifetimeSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Lifetime"));
	LifetimeSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::LifetimeEvent, this));
	LifetimeSlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 7 ����
	_LifetimeBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("LifetimeBMFont");

	// Slider of Red
	auto *RedSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Red"));
	RedSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::RedEvent, this));
	RedSlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 255  ����
	_RedBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("RedBMFont");

	// Slider of Green
	auto *GreenSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Green"));
	GreenSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::GreenEvent, this));
	GreenSlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 255  ����
	_GreenBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("GreenBMFont");

	// Slider of Blue
	auto *BlueSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Blue"));
	BlueSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::BlueEvent, this));
	BlueSlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 255  ����
	_BlueBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("BlueBMFont");

	// Slider of Wind
	auto *WindSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Wind"));
	WindSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::WindEvent, this));
	WindSlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 255  ����
	_WindBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("WindBMFont");

	// Slider of Type
	auto *TypeSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Type"));
	TypeSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::TypeEvent, this));
	TypeSlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 5 ����
	_TypeBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("TypeBMFont");
	// Slider of Emitter_Type
	auto *Emitter_TypeSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Emitter_Type"));
	Emitter_TypeSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::Emitter_TypeEvent, this));
	Emitter_TypeSlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 3 ����
	_Emitter_TypeBMFont = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("Emitter_TypeBMFont");
	//�ץ�BUBBLE
	
	Button *BUBBLEbtn = (cocos2d::ui::Button *)(rootNode->getChildByName("BUBBLEbtn"));
	BUBBLEbtn->addTouchEventListener(CC_CALLBACK_2(ParticleSystemScene::BUBBLEbtnTouchEvent, this));
	//�ץ�CIRCLE
	
	Button *CIRCLEbtn = (cocos2d::ui::Button *)(rootNode->getChildByName("CIRCLEbtn"));
	CIRCLEbtn->addTouchEventListener(CC_CALLBACK_2(ParticleSystemScene::CIRCLEbtnTouchEvent, this));
	//�ץ�BUBBLE
	
	Button *CLOUDbtn = (cocos2d::ui::Button *)(rootNode->getChildByName("CLOUDbtn"));
	CLOUDbtn->addTouchEventListener(CC_CALLBACK_2(ParticleSystemScene::CLOUDbtnTouchEvent, this));
	//�ץ�BUBBLE

	Button *COMETbtn = (cocos2d::ui::Button *)(rootNode->getChildByName("COMETbtn"));
	COMETbtn->addTouchEventListener(CC_CALLBACK_2(ParticleSystemScene::COMETbtnTouchEvent, this));
	//�ץ�BUBBLE
	
	Button *RAINDROPbtn = (cocos2d::ui::Button *)(rootNode->getChildByName("RAINDROPbtn"));
	RAINDROPbtn->addTouchEventListener(CC_CALLBACK_2(ParticleSystemScene::RAINDROPbtnTouchEvent, this));
	//�ץ�SPARK
	
	Button *SPARKbtn = (cocos2d::ui::Button *)(rootNode->getChildByName("SPARKbtn"));
	SPARKbtn->addTouchEventListener(CC_CALLBACK_2(ParticleSystemScene::SPARKbtnTouchEvent, this));


	_listener1 = EventListenerTouchOneByOne::create();	//�Ыؤ@�Ӥ@��@���ƥ��ť��
	_listener1->onTouchBegan = CC_CALLBACK_2(ParticleSystemScene::onTouchBegan, this);		//�[�JĲ�I�}�l�ƥ�
	_listener1->onTouchMoved = CC_CALLBACK_2(ParticleSystemScene::onTouchMoved, this);		//�[�JĲ�I���ʨƥ�
	_listener1->onTouchEnded = CC_CALLBACK_2(ParticleSystemScene::onTouchEnded, this);		//�[�JĲ�I���}�ƥ�

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//�[�J��Ыت��ƥ��ť��
	this->schedule(CC_SCHEDULE_SELECTOR(ParticleSystemScene::doStep));//���UdoStep

    return true;
}

void ParticleSystemScene::doStep(float dt)
{
	_ParticleControl.doStep(dt);//�I�s����t�θ�L���L�F�h�֮ɶ��A�M��n�F��....
}


bool ParticleSystemScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//Ĳ�I�}�l�ƥ�
{
	Point touchLoc = pTouch->getLocation();

	
	//��� Emitter �ɡA�i�즲�ӹϦ�
	if( _bEmitterOn ) _EmitterSprite->touchesBegan(touchLoc);////����U���@�����A���T�w�ثeEmitter�O�}�٬O���A�p�G�O�A��ܧA�ثe�i��|�I�bEmitterSprite(����o�g���ϥ�)���W�A�ΧA�i��|�즲��
	// �S����� Emitter�A�ӥB�S�����b Emitter �������s�W�A�~�� touch �i�H�I����ܤ��l(Particle)
	if ( !_emitterSwitchBtn->touchesBegan(touchLoc) && !_bEmitterOn ) _ParticleControl.onTouchesBegan(touchLoc);//�S�I�bEmitter���s�W&Emitter���e�]�S�}�ҡA�I���N�b������ _Particle
	return true;
}

void  ParticleSystemScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //Ĳ�I���ʨƥ�
{
	Point touchLoc = pTouch->getLocation();	
	if (_bEmitterOn) {//����U���@�����A���T�w�ثeEmitter�O�}�٬O��
		if (_EmitterSprite->touchesMoved(touchLoc)) //  ���ʨç�s Emitter ����m
			_ParticleControl._emitterPt = _EmitterSprite->getLoc();//�H�۵o�g�����ʡAParticleControl�H�ɩw��s����l�A�ó]�w�o�g������m
	}
	// �S����� Emitter�A�ӥB�S�����b Emitter �������s�W�A�~�� touch �i�H�I����ܤ��l
	if ( !_emitterSwitchBtn->touchesMoved(touchLoc) && !_bEmitterOn ) _ParticleControl.onTouchesMoved(touchLoc);//_ParticleControl���򱱨�onTouchesMoved�A��w��N����l�����n�a��
}

void  ParticleSystemScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //Ĳ�I�����ƥ� 
{
	Point touchLoc = pTouch->getLocation();

	if (_bEmitterOn) {//�T�w�ثe�O�}�٬O�}
		if( _EmitterSprite->touchesEnded(touchLoc) ) 
			_ParticleControl._emitterPt = _EmitterSprite->getLoc();
	}

	// �I�b Emitter �������ϥܤW�A�i�楲�n�����A����
	if (_emitterSwitchBtn->touchesEnded(touchLoc))//��񱼪��@�����n�ˬd�ƹ��O�_�I�bEmitterSwitchBtn�W��
	{
		_bEmitterOn = _emitterSwitchBtn->getStatus();//���o���A�çi�DbEmitterOn��"���U����"��emitterSwitchBtn�{�b�O�}�٬O��
		if ( _bEmitterOn ) { // ��� Emitter �ϥ�
			_EmitterSprite->setVisible(true);			
		}
		else { // ���� Emitter �ϥ�
			_EmitterSprite->setVisible(false);
		}
		_ParticleControl.setEmitter(_bEmitterOn); // ��s����t�Τ��� Emitter ���A
	}
}

void ParticleSystemScene::GravityEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)//�b����I���L�{���Q�I�s��ӡA�ˬd�ثe�O�_�b����slider
	{
		Slider* slider = dynamic_cast<Slider*>(sender);//�γo����oslider
		int percent = slider->getPercent();//���o�ثe��쪺���
		float fGravity = (-50.0f + percent) / 5.0f;//-10~10����
		_GravityBMValue->setString(StringUtils::format("%2.1f", fGravity));//�z�L _GravityBMValue�N�ثe���ܵ��G�ù��W������ܥX�ӡAsetString()=����ন�r��AStringUtils::format() = printf()
		_ParticleControl.setGravity(fGravity);//�I�s _ParticleControl �� setGravity �禡�A�H�ϬM�ثe���O������
	}
}

void ParticleSystemScene::SpreadEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fSpread = percent*1.8f; // 0 �� 180 ����
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
		float fDir = percent*3.6f; // 0 �� 360 ����
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
		float fSpin= percent*3.6f; // 0 �� 360 ���� 
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
		float fOpacity = percent*2.55f; // 0 �� 255 ����
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
		float fParticles = percent*2; // 0 �� 200 ����
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
		float fSpeed = percent*0.4f; // 0 �� 40 ����
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
		float fLifetime = percent*0.07f; // 0 �� 7 ����
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
		float fRed = percent*2.55f; // 0 �� 255 ����
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
		float fGreen = percent*2.55f; // 0 �� 255 ����
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
		float fBlue = percent*2.55f; // 0 �� 255 ����
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
		int percent = slider->getPercent();//���o�ثe��쪺���
		float fWind = (-50.0f + percent) / 5.0f;//-10~10����
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
		int iType = percent/12; // 0 �� 9 ����
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
		int iEmitter_Type = percent / 25; // 0 �� 3 ����
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