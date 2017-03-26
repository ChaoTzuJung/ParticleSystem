#ifndef __CPARTICLE_SYSTEM_H__
#define __CPARTICLE_SYSTEM_H__

#include "cocos2d.h"
#include "CParticle.h"
#include <list>
#include "cocostudio/CocoStudio.h"


using namespace std;



class CParticleSystem
{
private:
	//CDraggableSprite _EmitterSprite;
	CParticle* _pParticles;//��l�@��Particles
	list<CParticle*> _FreeList;//�إߤ@��CParticle���O�����V�쵲��C�A��e���C�@��i�񪺤H���OCParticle���O�����Ъ���
	list<CParticle*> _InUsedList;
	int _iFree;	// �i�Ϊ�  Particle �Ӽ�
	int _iInUsed;  // ���b�ϥΪ� Particle �Ӽ�
	int _iType;  // �ثe������l�B�ʼҦ��O���@�ث��A
	bool _bFireOn;
	bool _bEmitterOn; // Emitter �O�_���

public:
	// Emitter �]�w�������Ѽ�
	cocos2d::Point _emitterPt;// Emitter ����m	
	//cocos2d::Point _emitterPt1 = _emitterPt = 100;
	float   _fGravity;
	float	_fSpread;	 // 0 �� 180 �סA�w�]�� 180
	float    _fDir; // Emitter ���Q�g��V�A0 �� 360�סA�f�ɰw��V����
	float	_fSpin;		// ���l������q degree/sec
	float   _fOpacity;
	float   _fParticles;
	float   _fVelocity;	 // ���l�����}�t��
	float   _fLifeTime;	 // ���l���s���ɶ�
	float   _fRed;
	float   _fGreen;
	float   _fBlue;
	int		_iNumParticles;	// �C�����Ӳ��ͪ����l��
	int     _iGenParticles; // ��ثe����Ҳ��ͪ����l�Ӽ�(�H�C�����)
	float   _fElpasedTime;	// ��ثe����g�L���ɶ�(�C�@��|���]�@��)
	float Timer;
	int _iEmitter_Type;
	//cocos2d::Color3B _color;	// ���l���C��
	int _a;//CParticlesystem 168��
	float   _fWind;
	float degree;
	cocos2d::Point _windDir;  // ������V�A���d�ҨS����@



	CParticleSystem();
	~CParticleSystem();
	void init(cocos2d::Layer &inlayer);
	void doStep(float dt);
	void setEmitter(bool bEm);
	void setType(int type) { _iType = type; }
	void setEmitter_Type(int type) { _iEmitter_Type = type; }

 //   void onTouchesEnded(const cocos2d::CCPoint &touchPoint);
    void onTouchesBegan(const cocos2d::CCPoint &touchPoint);
    void onTouchesMoved(const cocos2d::CCPoint &touchPoint);

	void setGravity(float fGravity);
	void setSpread(float fSpread);
	void setDir(float fDir);
	void setSpin(float fSpin);
	void setOpacity(float fOpacity);
	void setParticles(float fParticles);
	void setSpeed(float fSpeed);
	void setLifetime(float fLifetime);
	void setRed(float fRed);
	void setGreen(float fGreen);
	void setBlue(float fBlue);
	void setWind(float fWind);

	void CParticleSystem::ChangeSprite(int Case, cocos2d::Layer &inlayer);

	cocos2d::Point garbage;
	cocos2d::Vec2 _NowPos;
	cocos2d::Point _Vector;
	cocos2d::Point _Move;
	bool bFire = true;
	bool _bBoom = false;

		

	
};

#endif