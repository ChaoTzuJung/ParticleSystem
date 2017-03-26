#include "CParticleSystem.h"

#define NUMBER_PARTICLES 1000 // �w�]�@�����o 1000 �� Particles

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

void CParticleSystem::init(cocos2d::Layer &inlayer)//�������[�J 
{

	_iFree = NUMBER_PARTICLES;//_iFree��1000��
	_iInUsed = 0;
	_pParticles = new CParticle[NUMBER_PARTICLES]; // ���o�һݭn�� particle �Ŷ��Anew�@�ӥ��C
	//A.   int *_pParticles
	//B.   _pParticles = new int[1000]  
	// A+B = int _pParticles[1000]
	 // Ū�J�x�s�h�i�Ϥ��� plist ��

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Pic/particletexture.plist");
	for (int i = 0; i < NUMBER_PARTICLES; i++) {
		_pParticles[i].setParticle("flare.png", inlayer);//��_pParticles[i](�C�@�Ӥ��l)�A��setParticle()���γo�i��flare.png�é��o��layer����(inlayer)��
		_FreeList.push_front(&_pParticles[i]);
	}    //&_pParticles[0]  �񪺬O���P���X���O�ƭȩҥH�O���ȭn�[&//push_front�����P���X�[�i��CFreeList��


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
					  else return;// �S�����l, �ҥH�N������
				  
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
	// �]�w�ثe particle �O�b inused �� gravity
	_fGravity = fGravity;
	list <CParticle *>::iterator it;//�ŧiList��	iterator(���N��) it
	if (_iInUsed != 0) { // �����l�ݭn��s��
		for (it = _InUsedList.begin(); it != _InUsedList.end(); it++) {//it++  = it���V�U�@�Ӯe��
			(*it)->setGravity(_fGravity);//��s�ӧO���l�����O

		}
	}//it�@�w�n�[�W(* )�A�~���ഫ�����x�s�̭������Ъ���A���ɤ~��z�L->�I�s�άO�]�w�����禡�P�ܼ�
	

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
	if (_iInUsed != 0) _InUsedList.clear(); // �����Ҧ��� NODE
	if (_iFree != 0) _FreeList.clear();//�O���쵲��C���Ҧ����`�I��e���M���A�����v�T�쵲��C�����e
	delete[] _pParticles; // ����Ҧ����o�귽
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("particletexture.plist");
}

void CParticleSystem::onTouchesBegan(const cocos2d::CCPoint &touchPoint)
{
	CParticle *get;//A
	switch (_iType)
	{
	case STAY_FOR_TWOSECONDS:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
		if (_iFree != 0) {//���ˬd _iFree �O�_����
			get = _FreeList.front();//�qList����Ĥ@�Ӧ�m(�e��)��(��)�X�ӡA�M��q_FreeList���o�@��Particle�X�ӥ� B
			get->setBehavior(STAY_FOR_TWOSECONDS);//�]�w�欰�b�e���W���d�����
			get->setPosition(touchPoint);//��ثeUSE���I���m���A
			get->setGravity(_fGravity);//�⭫�O���A
			get->setWind(_fWind);
			//������쵲��CFreeList&InUsedList�A�@�}�l�j�a(1000���l)���bFreeList���A���I�ù��@�U�ͦ��@�Ӥ��l2�����b�e���W(_iFree--; _iInUsed++;)�A�ۤ�2���
			_FreeList.pop_front();//���o��m���e������
			_InUsedList.push_front(get);//����P���X���
			_iFree--; _iInUsed++;
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case RANDOMS_FALLING :
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(RANDOMS_FALLING);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			get->setWind(_fWind);
			_FreeList.pop_front();//�NList�Ĥ@�Ӧ�m(�e��)�����A���e�����Ҹ˪�"�x�s��m"���|�Q����  C
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case FREE_FLY:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
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
		else return;// �S�����l, �ҥH�N������
		break;
	case EXPLOSION:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
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
		else return;// �S�����l, �ҥH�N������
		break;
	case HEARTSHAPE:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
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
		else return;// �S�����l, �ҥH�N������
		break;
	case BUTTERFLYSHAPE:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
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
		else return;// �S�����l, �ҥH�N������
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
		else return;// �S�����l, �ҥH�N������
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
		else return;// �S�����l, �ҥH�N������
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
		else return;// �S�����l, �ҥH�N������
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
		else return;// �S�����l, �ҥH�N������
		break;
	}
}

void CParticleSystem::onTouchesMoved(const cocos2d::CCPoint &touchPoint)
{
	CParticle *get;
	switch (_iType)
	{
	case STAY_FOR_TWOSECONDS:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
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
		else return;// �S�����l, �ҥH�N������
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
		else return;// �S�����l, �ҥH�N������
		break;
	case FREE_FLY:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
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
		else return;// �S�����l, �ҥH�N������
		break;
	}
}

