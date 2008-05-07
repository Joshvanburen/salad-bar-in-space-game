void main()
{
	
}
int collide(WorldEntity &entity1, WorldEntity &entity2)
{
	PrintString("In Gravship Script");
	Gravship@ gravship = EntityToGravship(entity1);
	Enemy@ enemy = EntityToEnemy(entity2);
	if (gravship == null){
		Enemy@ enemy = EntityToEnemy(entity1);
		Gravship@ gravship = EntityToGravship(entity2
);
		if (enemy == null){
			PrintString("unknown collider");
			return 1;
		}
		gravship.takeDamage(5);
		
		soundManager.getSound("splat").play(false);
		enemy.splat();
		entityManager.remove(entity1.getID());
	}
	else{
		if (enemy == null){
		PrintString("unknown collider");
		return 1;
		}
		gravship.takeDamage(5);
		
		soundManager.getSound("splat").play(false);
		enemy.splat();
	entityManager.remove(entity2.getID());
	}

	return 1;
}