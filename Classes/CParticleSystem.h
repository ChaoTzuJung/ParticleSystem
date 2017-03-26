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
	CParticle* _pParticles;//初始一個Particles
	list<CParticle*> _FreeList;//建立一個CParticle型別的雙向鏈結串列，其容器每一格可放的人都是CParticle型別的指標物件
	list<CParticle*> _InUsedList;
	int _iFree;	// 可用的  Particle 個數
	int _iInUsed;  // 正在使用的 Particle 個數
	int _iType;  // 目前控制的分子運動模式是哪一種型態
	bool _bFireOn;
	bool _bEmitterOn; // Emitter 是否顯示

public:
	// Emitter 設定的相關參數
	cocos2d::Point _emitterPt;// Emitter 的位置	
	//cocos2d::Point _emitterPt1 = _emitterPt = 100;
	float   _fGravity;
	float	_fSpread;	 // 0 到 180 度，預設為 180
	float    _fDir; // Emitter 的噴射方向，0 到 360度，逆時針方向為正
	float	_fSpin;		// 分子的旋轉量 degree/sec
	float   _fOpacity;
	float   _fParticles;
	float   _fVelocity;	 // 分子的離開速度
	float   _fLifeTime;	 // 分子的存活時間
	float   _fRed;
	float   _fGreen;
	float   _fBlue;
	int		_iNumParticles;	// 每秒應該產生的分子數
	int     _iGenParticles; // 到目前為止所產生的分子個數(以每秒為單位)
	float   _fElpasedTime;	// 到目前為止經過的時間(每一秒會重設一次)
	float Timer;
	int _iEmitter_Type;
	//cocos2d::Color3B _color;	// 分子的顏色
	int _a;//CParticlesystem 168行
	float   _fWind;
	float degree;
	cocos2d::Point _windDir;  // 風的方向，本範例沒有實作



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