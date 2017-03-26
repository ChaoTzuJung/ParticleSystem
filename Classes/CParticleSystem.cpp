#include "CParticleSystem.h"

#define NUMBER_PARTICLES 1000 // 預設一次取得 1000 個 Particles

USING_NS_CC;

CParticleSystem::CParticleSystem()
{
	_fGravity = 0;
	_bEmitterOn = false;
	_fWind = 0;
	
}

void CParticleSystem::setEmitter(bool bEm)
{
	_bEmitterOn = bEm;
}

void CParticleSystem::init(cocos2d::Layer &inlayer)//元素的加入 
{

	_iFree = NUMBER_PARTICLES;//_iFree有1000個
	_iInUsed = 0;
	_pParticles = new CParticle[NUMBER_PARTICLES]; // 取得所需要的 particle 空間，new一個正列
	//A.   int *_pParticles
	//B.   _pParticles = new int[1000]  
	// A+B = int _pParticles[1000]
	 // 讀入儲存多張圖片的 plist 檔

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Pic/particletexture.plist");
	for (int i = 0; i < NUMBER_PARTICLES; i++) {
		_pParticles[i].setParticle("flare.png", inlayer);//對_pParticles[i](每一個分子)，跟setParticle()說用這張圖flare.png並放到這個layer場景(inlayer)內
		_FreeList.push_front(&_pParticles[i]);
	}    //&_pParticles[0]  放的是門牌號碼不是數值所以是左值要加&//push_front讓門牌號碼加進串列FreeList內


}		
void CParticleSystem::ChangeSprite(int Case, cocos2d::Layer &inlayer) {
	switch (Case) {
	case 1:
		for (int i = 0; i < NUMBER_PARTICLES; i++) {
			_pParticles[i].setParticle("bubble.png", inlayer);
			_FreeList.push_front(&_pParticles[i]);
		}
	break;
	case 2:
		for (int i = 0; i < NUMBER_PARTICLES; i++) {
			_pParticles[i].setParticle("circle.png", inlayer);
			_FreeList.push_front(&_pParticles[i]);
		}
	break;
	case 3:
		for (int i = 0; i < NUMBER_PARTICLES; i++) {
			_pParticles[i].setParticle("cloud.png", inlayer);
			_FreeList.push_front(&_pParticles[i]);
		}
	break;
	case 4:
		for (int i = 0; i < NUMBER_PARTICLES; i++) {
			_pParticles[i].setParticle("comet.png", inlayer);
			_FreeList.push_front(&_pParticles[i]);
		}
	break;
	case 5:
		for (int i = 0; i < NUMBER_PARTICLES; i++) {
			_pParticles[i].setParticle("raindrop.png", inlayer);
			_FreeList.push_front(&_pParticles[i]);
		}
	break;
	case 6:
		for (int i = 0; i < NUMBER_PARTICLES; i++) {
			_pParticles[i].setParticle("spark.png", inlayer);
			_FreeList.push_front(&_pParticles[i]);
		}
	break;
	}
}























void CParticleSystem::doStep(float dt){
 	CParticle *get;
	list <CParticle *>::iterator it;

	
//------------------------------------------------------------------------------------------
	if (_bEmitterOn) { 
		int n = (int)(_fElpasedTime * _iNumParticles);
//---------------------------------------------------------------------------------------------
	  switch(_iEmitter_Type){
	  case 0:
		  if (n > _iGenParticles) {
			  for (int i = 0; i < n - _iGenParticles; i++) {
				  if (_iFree != 0) {
					  get = _FreeList.front();
					  get->setBehavior(EMITTER_DEFAULT);
					  get->setVelocity(_fVelocity);
					  get->setLifetime(_fLifeTime);
					  get->setGravity(_fGravity);
					  get->setPosition(_emitterPt);
					  get->setSize(0.125f);
					  get->setColor(Color3B(_fRed, _fGreen, _fBlue));
					  get->setOpacity(_fOpacity);
					  get->setWind(_fWind);

					  float t = (rand() % 1001) / 1000.0f;
					  t = _fSpread - t * _fSpread * 2;
					  t = (_fDir + t)* M_PI / 180.0f;
					  Vec2 vdir(cosf(t), sinf(t));
					  get->setDirection(vdir);
					  _FreeList.pop_front();
					  _InUsedList.push_front(get);
					  _iFree--; _iInUsed++;
				  }
			  }
			  _iGenParticles = n;

		  }
		  if (_fElpasedTime >= 1.0f) {
			  _fElpasedTime -= 1.0f;
			  if (_iGenParticles >= _iNumParticles) _iGenParticles -= _iNumParticles;
			  else _iGenParticles = 0;
		  }
		  _fElpasedTime += dt;
	  

		  break;
//---------------------------------------------------------------------------------------------
	  case 1:
		  if (n > _iGenParticles) {
			  for (int i = 0; i < n - _iGenParticles; i++) {
				  if (_iFree != 0) {
					  get = _FreeList.front();
					  get->setBehavior(EMITTER_DEFAULT);
					  get->setVelocity(_fVelocity);
					  get->setLifetime(_fLifeTime);
					  get->setGravity(8.2);
					  get->setPosition(_emitterPt);
					  get->setSize(0.125f);
					  get->setColor(Color3B(0, 100, 100));
					  get->setOpacity(_fOpacity);
					  get->setWind(-10);
					 
					  float t = (rand() % 1001) / 1000.0f;
					  t = 18 - t * 18 * 2;
					  t = (290 + t)* M_PI / 180.0f;
					  Vec2 vdir(cosf(t), sinf(t));
					  get->setDirection(vdir);
					  _FreeList.pop_front();
					  _InUsedList.push_front(get);
					  _iFree--; _iInUsed++;
				  }
			  }
			  _iGenParticles = n;

		  }
		  if (_fElpasedTime >= 1.0f) {
			  _fElpasedTime -= 1.0f;
			  if (_iGenParticles >= _iNumParticles) _iGenParticles -= _iNumParticles;
			  else _iGenParticles = 0;
		  }
		  _fElpasedTime += dt;
	  
		  break;
	  case 2:
		  if (n > _iGenParticles) {
			  for (int i = 0; i < n - _iGenParticles; i++) {
				  if (_iFree != 0) {
					  get = _FreeList.front();
					  get->setBehavior(EMITTER_DEFAULT);
					  get->setVelocity(30);
					  get->setLifetime(2);
					  get->setGravity(_fGravity);
					  get->setPosition(_emitterPt);
					  get->setSize(0.5f);
					  get->setColor(Color3B(100, 0, 100));
					  get->setOpacity(150);
					  get->setWind(_fWind);

					  float t = (rand() % 1001) / 1000.0f;
					  t = _fSpread - t * _fSpread * 2;
					  t = (_fDir + t)* M_PI / 180.0f;
					  Vec2 vdir(cosf(t), sinf(t));
					  get->setDirection(vdir);
					  _FreeList.pop_front();
					  _InUsedList.push_front(get);
					  _iFree--; _iInUsed++;
				  }
			  }
			  _iGenParticles = n;

		  }
		  if (_fElpasedTime >= 1.0f) {
			  _fElpasedTime -= 1.0f;
			  if (_iGenParticles >= _iNumParticles) _iGenParticles -= _iNumParticles;
			  else _iGenParticles = 0;
		  }
		  _fElpasedTime += dt;
	  
		  break;
//------------------------------------------------------------------------------------------
		 
	  case 4:
		  
			  if (Timer == 0) {
				  garbage = Vec2(100, 0);
				  _NowPos = garbage;
				  _Vector = _emitterPt - garbage;
				  _Move = _emitterPt - garbage;
				  bFire = true;
				  _bBoom = false;
			  }
			  else if (Timer == 0.0f && Timer <= 2.0f) {
				 
			  }
			  else if (Timer >=1.0f && Timer <= 2.5f) {
				  bFire = false;
				 _bBoom = true;
				
			  }
			  else if (Timer >= 2.5f && Timer <= 3.0f) {
				  
				 
			  }
			  else if (Timer >= 3.0f && Timer <= 5.0f) {
				  _bBoom = false;
				
			  }
			  else if (Timer > 5.0f) {
				  Timer = 0.0f;
				  cocos2d::Point garbage = Vec2(100, 0);
				  _NowPos = garbage;
				  _Vector = _emitterPt - garbage;
				  _Move = _emitterPt - garbage;
				  bFire = true;
				  _bBoom = false;

			  }
			  Timer = Timer + dt;

	/*		  if (_iFree != 0 && _bBoom = true) {
				  get = _FreeList.front();
				  get->setBehavior(EMITTER_DEFAULT);
				 
				  get->setSize(0.5f); 
				  get->setOpacity(255);
				  _FreeList.pop_front();
				  _InUsedList.push_front(get);
				  _iFree--; _iInUsed++;
				 }
			  if (_iFree != 0 && bFire = false;) {
				  get = _FreeList.front();
				  get->setBehavior(EMITTER_DEFAULT);
				  get->setPosition(Vec2(100, 0));
				  get->setSize(0.5f);
				  get->setOpacity(0);
				  _FreeList.pop_front();
				  _InUsedList.push_front(get);
				  _iFree--; _iInUsed++;
			  }*/
			  
			  if (n > _iGenParticles){
				  for (int i = 0; i < n - _iGenParticles; i++) {
					  if (_iFree != 0) {
						  get = _FreeList.front();
						  get->setBehavior(EMITTER_DEFAULT);
						  get->setVelocity(_fVelocity);
						  get->setLifetime(_fLifeTime);
						  get->setGravity(_fGravity);
						  get->setPosition(_NowPos);
						  get->setSize(0.125f);
						  get->setColor(Color3B(_fRed, _fGreen, _fBlue));
						  if (bFire == true)
							  get->setOpacity(_fOpacity);
						  else if (bFire==false)
							  get->setOpacity(0.0f);
						  get->setWind(_fWind);

						  float t = (rand() % 1001) / 1000.0f;
						  t = _fSpread - t * _fSpread * 2;
						  t = (_fDir + t)* M_PI / 180.0f;
						  Vec2 vdir(cosf(t), sinf(t));
						  get->setDirection(vdir);
						  _FreeList.pop_front();
						  _InUsedList.push_front(get);
						  _iFree--; _iInUsed++;
					  }
				  }
				  if (_bBoom) {
					  if (_iFree > 5) {
						  for (int i = 0; i < 5; i++) {
							  get = _FreeList.front();
							  get->setBehavior(EXPLOSION);
							  get->setPosition(_emitterPt);
							  get->setGravity(_fGravity);
							  get->setWind(_fWind);
							  _FreeList.pop_front();
							  _InUsedList.push_front(get);
							  _iFree--; _iInUsed++;
						  }
					  }
					  else return;// 沒有分子, 所以就不提供
				  
				  }
				  _iGenParticles = n;

			  }
			  if (_fElpasedTime >= 1.0f) {
				  _fElpasedTime -= 1.0f;
				  if (_iGenParticles >= _iNumParticles) _iGenParticles -= _iNumParticles;
				  else _iGenParticles = 0;
			  }
			  _fElpasedTime += dt;
			  _NowPos = _NowPos + _Move*dt;

			  break;
		  //--_NowPos ----------------------------------------------------------------------------------------
	  case 3:
		  if (n > _iGenParticles) {
			  for (int i = 0; i < n - _iGenParticles; i++) {
				  if (_iFree != 0) {
					  get = _FreeList.front();
					  get->setBehavior(EMITTER_DEFAULT);
					  get->setVelocity(8);
					  get->setLifetime(_fLifeTime);
					  get->setGravity(-5.4);
					  get->setPosition(_emitterPt);
					  get->setSize(0.125f);
					  get->setColor(Color3B(100, 100, 0));
					  get->setOpacity(_fOpacity);
					  get->setWind(_fWind);
					  get->setDir(90);
					  get->setSpread(49);

					  float t = (rand() % 1001) / 1000.0f;
					  t = 49 - t * 49 * 2;
					  t = (90 + t)* M_PI / 180.0f;
					  Vec2 vdir(cosf(t), sinf(t));
					  get->setDirection(vdir);
					  _FreeList.pop_front();
					  _InUsedList.push_front(get);
					  _iFree--; _iInUsed++;
				  }
			  }
			  _iGenParticles = n;

		  }
		  if (_fElpasedTime >= 1.0f) {
			  _fElpasedTime -= 1.0f;
			  if (_iGenParticles >= _iNumParticles) _iGenParticles -= _iNumParticles;
			  else _iGenParticles = 0;
		  }
		  _fElpasedTime += dt;

		  break;
		}//switch
	  }//Emitter_on
//--------------------------------------------------------------------------
	  if (_iInUsed != 0) {
		  for (it = _InUsedList.begin(); it != _InUsedList.end(); ) {
			  if ((*it)->doStep(dt)) {

				  _FreeList.push_front((*it));
				  it = _InUsedList.erase(it);
				  _iFree++; _iInUsed--;
			  }
			  else it++;
		  }
	}//_iInUsed != 0
 //-------------------------------------------------------------------------	
}//doStep

void CParticleSystem::setGravity(float fGravity)
{
	// 設定目前 particle 是在 inused 的 gravity
	_fGravity = fGravity;
	list <CParticle *>::iterator it;//宣告List的	iterator(迭代器) it
	if (_iInUsed != 0) { // 有分子需要更新時
		for (it = _InUsedList.begin(); it != _InUsedList.end(); it++) {//it++  = it指向下一個容器
			(*it)->setGravity(_fGravity);//更新個別分子的重力

		}
	}//it一定要加上(* )，才能轉換成所儲存裡面的指標物件，此時才能透過->呼叫或是設定成員函式與變數
	

}

void CParticleSystem::setSpread(float fSpread)
{
	_fSpread = fSpread;

}
void CParticleSystem::setDir(float fDir)
{
	_fDir = fDir;
	
}
void CParticleSystem::setSpin(float fSpin)
{
	_fSpin = fSpin;
	list <CParticle *>::iterator it; 
	if (_iInUsed != 0) { 
		for (it = _InUsedList.begin(); it != _InUsedList.end(); it++) {
			(*it)->setSpin1(_fSpin);
		}
	}

}

void CParticleSystem::setOpacity(float fOpacity)
{
	_fOpacity = fOpacity;

}

void CParticleSystem::setParticles(float fParticles)
{
	_fParticles = fParticles;

}

void CParticleSystem::setSpeed(float fSpeed)
{
	_fVelocity = fSpeed;

}
void CParticleSystem::setLifetime(float fLifetime)
{
	_fLifeTime = fLifetime;

}
void CParticleSystem::setRed(float fRed)
{
	_fRed = fRed;
	list <CParticle *>::iterator it;
	if (_iInUsed != 0) {
		for (it = _InUsedList.begin(); it != _InUsedList.end(); it++) {
			(*it)->setRed(_fRed);
		}
	}
	

}
void CParticleSystem::setGreen(float fGreen)
{
	
	_fGreen = fGreen;
	list <CParticle *>::iterator it;
	if (_iInUsed != 0) {
		for (it = _InUsedList.begin(); it != _InUsedList.end(); it++) {
			(*it)->setGreen(_fGreen);
		}
	}
	
}
void CParticleSystem::setBlue(float fBlue)
{

	_fBlue = fBlue;
	list <CParticle *>::iterator it;
	if (_iInUsed != 0) {
		for (it = _InUsedList.begin(); it != _InUsedList.end(); it++) {
			(*it)->setBlue( _fBlue);
		}
	}
	
}
void CParticleSystem::setWind(float fWind)
{

	_fWind = fWind;
	list <CParticle *>::iterator it;
	if (_iInUsed != 0) {
		for (it = _InUsedList.begin(); it != _InUsedList.end(); it++) {
			(*it)->setWind(_fWind);
		}
	}


}

CParticleSystem::~CParticleSystem()
{
	if (_iInUsed != 0) _InUsedList.clear(); // 移除所有的 NODE
	if (_iFree != 0) _FreeList.clear();//是把鏈結串列的所有的節點跟容器清掉，但不影響鏈結串列的內容
	delete[] _pParticles; // 釋放所有取得資源
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("particletexture.plist");
}

void CParticleSystem::onTouchesBegan(const cocos2d::CCPoint &touchPoint)
{
	CParticle *get;//A
	switch (_iType)
	{
	case STAY_FOR_TWOSECONDS:
		// 從 _FreeList 取得一個分子給放到 _InUsed
		if (_iFree != 0) {//先檢查 _iFree 是否足夠
			get = _FreeList.front();//從List中把第一個位置(容器)抓(拿)出來，然後從_FreeList取得一個Particle出來用 B
			get->setBehavior(STAY_FOR_TWOSECONDS);//設定行為在畫面上停留兩秒鐘
			get->setPosition(touchPoint);//把目前USE的點跟位置給你
			get->setGravity(_fGravity);//把重力給你
			get->setWind(_fWind);
			//有兩個鏈結串列FreeList&InUsedList，一開始大家(1000分子)都在FreeList內，當點螢幕一下生成一個分子2秒鐘在畫面上(_iFree--; _iInUsed++;)，相反2秒後
			_FreeList.pop_front();//取得位置後把容器拿掉
			_InUsedList.push_front(get);//把門牌號碼放到
			_iFree--; _iInUsed++;
		}
		else return;// 沒有分子, 所以就不提供
		break;
	case RANDOMS_FALLING :
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(RANDOMS_FALLING);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			get->setWind(_fWind);
			_FreeList.pop_front();//將List第一個位置(容器)移除，但容器內所裝的"儲存位置"不會被釋放  C
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// 沒有分子, 所以就不提供
		break;
	case FREE_FLY:
		// 從 _FreeList 取得一個分子給放到 _InUsed
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(FREE_FLY);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			get->setWind(_fWind);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// 沒有分子, 所以就不提供
		break;
	case EXPLOSION:
		// 從 _FreeList 取得一個分子給放到 _InUsed
		if (_iFree > 100) {
			for (int i = 0; i < 100; i++) {
				get = _FreeList.front();
				get->setBehavior(EXPLOSION);
				get->setPosition(touchPoint);
				get->setGravity(_fGravity);
				get->setWind(_fWind);
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
			}
		}
		else return;// 沒有分子, 所以就不提供
		break;
	case HEARTSHAPE:
		// 從 _FreeList 取得一個分子給放到 _InUsed
		if (_iFree > 150) {
			for (int i = 0; i < 150; i++) {
				get = _FreeList.front();
				get->setBehavior(HEARTSHAPE);
				get->setPosition(touchPoint);
				get->setGravity(_fGravity);
				get->setWind(_fWind);
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
			}
		}
		else return;// 沒有分子, 所以就不提供
		break;
	case BUTTERFLYSHAPE:
		// 從 _FreeList 取得一個分子給放到 _InUsed
		if (_iFree > 150) {
			for (int i = 0; i < 150; i++) {
				get = _FreeList.front();
				get->setBehavior(BUTTERFLYSHAPE);
				get->setPosition(touchPoint);
				get->setGravity(_fGravity);
				get->setWind(_fWind);
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
			}
		}
		else return;// 沒有分子, 所以就不提供
		break;
	case ROSE:
		if (_iFree > 150) {
			for (int i = 0; i < 150; i++) {
				get = _FreeList.front();
				get->setBehavior(ROSE);
				get->setPosition(touchPoint);
				get->setGravity(_fGravity);
				get->setWind(_fWind);
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
			}
		}
		else return;// 沒有分子, 所以就不提供
		break;
	case ROSE2:
		if (_iFree > 150) {
			for (int i = 0; i < 150; i++) {
				get = _FreeList.front();
				get->setBehavior(ROSE2);
				get->setPosition(touchPoint);
				get->setGravity(_fGravity);
				get->setWind(_fWind);
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
			}
		}
		else return;// 沒有分子, 所以就不提供
		break;
	case ROSE3:
		if (_iFree > 150) {
			for (int i = 0; i < 150; i++) {
				get = _FreeList.front();
				get->setBehavior(ROSE3);
				get->setPosition(touchPoint);
				get->setGravity(_fGravity);
				get->setWind(_fWind);
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
			}
		}
		else return;// 沒有分子, 所以就不提供
		break;
	case FIREWORK:
		if (_iFree > 150) {
			for (int i = 0; i < 150; i++) {
				get = _FreeList.front();
				get->setBehavior(FIREWORK);
				get->setPosition(touchPoint);
				get->setGravity(_fGravity);
				get->setWind(_fWind);
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
			}
		}
		else return;// 沒有分子, 所以就不提供
		break;
	}
}

void CParticleSystem::onTouchesMoved(const cocos2d::CCPoint &touchPoint)
{
	CParticle *get;
	switch (_iType)
	{
	case STAY_FOR_TWOSECONDS:
		// 從 _FreeList 取得一個分子給放到 _InUsed
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(STAY_FOR_TWOSECONDS);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			get->setWind(_fWind);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// 沒有分子, 所以就不提供
		break;
	case RANDOMS_FALLING:
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(RANDOMS_FALLING);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			get->setWind(_fWind);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// 沒有分子, 所以就不提供
		break;
	case FREE_FLY:
		// 從 _FreeList 取得一個分子給放到 _InUsed
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(FREE_FLY);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			get->setWind(_fWind);
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// 沒有分子, 所以就不提供
		break;
	}
}

