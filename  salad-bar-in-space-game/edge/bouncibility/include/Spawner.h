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


	void setFrequency(int frequency){
		m_Frequency = frequency;
	}

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
 	
	WorldEntity* clone(); 

	int m_Frequency;

	int m_SimulSpawn;

	irr::u32 m_AccumulatedTime;

	std::string currentState;



	hat<WorldEntity* > entities;
};
