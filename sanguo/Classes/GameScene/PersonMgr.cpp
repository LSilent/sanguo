﻿#include "PersonMgr.h"
#include "../BaseCSV.h"

USING_NS_CC;

PersonMgr::PersonMgr()
{
    initActionInfo("actioninfo.csv", false);
    initSoldierInfo("soldierinfo.csv", false);
}

PersonMgr::~PersonMgr()
{
    
}

PersonMgr& PersonMgr::getSingleton()
{
    static PersonMgr s_mgr;
    
    return s_mgr;
}

void PersonMgr::release()
{
}

//! 初始化动作信息表
void PersonMgr::initActionInfo(const char* filename, bool refresh)
{
	if(!m_mapActionInfo.empty() && !refresh)
		return ;

	BaseCSV csvdata;
	csvdata.load(filename);

	m_mapActionInfo.clear();

	//! 第一行是表头
	for(int i = 1; i < csvdata.m_iHeight; ++i)
	{
		std::pair<int, PersonActionInfo> p;

		p.first = csvdata.getAsInt("gameobjid", i);

		p.second.oid = p.first;
		p.second.img0 = csvdata.get("img0", i);
		p.second.img1 = csvdata.get("img1", i);
		p.second.resname = csvdata.get("resname", i);
		p.second.stype = csvdata.getAsInt("stype", i);

		char tmp[32];

		for(int j = 0; j < _PERSON_ACTION_NUMS; ++j)
		{
			sprintf(tmp, "bindex%d", j);
			p.second.bindex[j] = csvdata.getAsInt(tmp, i);

			sprintf(tmp, "eindex%d", j);
			p.second.eindex[j] = csvdata.getAsInt(tmp, i);

			sprintf(tmp, "time%d", j);
			p.second.atime[j] = csvdata.getAsInt(tmp, i);
		}

		m_mapActionInfo.insert(p);
	}
}

//! 取动作信息
PersonActionInfo* PersonMgr::getActionInfo(int id)
{
	std::unordered_map<int, PersonActionInfo>::iterator it = m_mapActionInfo.find(id);

	if(it != m_mapActionInfo.end())
		return &(it->second);

	return NULL;
}

//! 初始化士兵信息表
void PersonMgr::initSoldierInfo(const char* filename, bool refresh)
{
    if(!m_mapSoldierInfo.empty() && !refresh)
        return ;
    
    BaseCSV csvdata;
    csvdata.load(filename);
    
    m_mapSoldierInfo.clear();
    
    //! 第一行是表头
    for(int i = 1; i < csvdata.m_iHeight; ++i)
    {
        std::pair<int, SoldierInfo> p;
        
        p.first = csvdata.getAsInt("gameobjid", i);
        
        p.second.oid = p.first;
        
        p.second.idActionInfo = csvdata.getAsInt("actioninfo", i);
        p.second.minRadius = csvdata.getAsInt("minradius", i);
        p.second.maxRadius = csvdata.getAsInt("maxradius", i);
        p.second.speed = csvdata.getAsInt("speed", i);
        
        m_mapSoldierInfo.insert(p);
    }
}

//! 取士兵信息
SoldierInfo* PersonMgr::getSoldierInfo(GameObjID oid)
{
    std::unordered_map<int, SoldierInfo>::iterator it = m_mapSoldierInfo.find(oid);
    
    if(it != m_mapSoldierInfo.end())
        return &(it->second);
    
    return NULL;
}

