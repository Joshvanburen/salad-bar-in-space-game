/* 
 * 
 * Spawner.h
 * 
 *
 */

#pragma once
#include "boost/random.hpp"
#include "Common.h"
#include "hat.h"
#include "WorldEntity.h"

namespace Entity{
class SpawnerFactory;
};


class Spawner : public WorldEntity {
	friend class Entity::SpawnerFactory;
public:


	// Constructor and destructor
	Spawner();
	~Spawner();

	void load();

	void pause(bool enabled){
		m_Paused = enabled;
	}
	void setFrequency(int frequency);

	int getFrequency(){
		return m_Frequency;
	}

	int getSimulSpawn(){
		return m_SimulSpawn;
	}

	void setSimulSpawn(int simulSpawn){
		m_SimulSpawn = simulSpawn;
	}

	void addEntity(const std::string& entityName, float probability);

	void changeState(const std::string name);

	static Spawner* EntityToSpawner(WorldEntity*);


	void update();

	void draw();



private: 
	void spawnEntity();
 	
	WorldEntity* clone(); 

	int m_Frequency;

	int m_SimulSpawn;

	int m_EntitiesLeft;

	irr::u32 m_AccumulatedTime;

	std::string currentState;

	typedef std::list<int> TimeList;

	typedef boost::mt19937 RNGEngine;

	typedef boost::uniform_int<> UniformDistribution;
	
	TimeList m_SpawnTimes;

	RNGEngine m_RandomEngine;

	boost::variate_generator<RNGEngine&, UniformDistribution>* m_RNG;

	hat<WorldEntity* > m_Entities;

	bool m_Paused;
};
