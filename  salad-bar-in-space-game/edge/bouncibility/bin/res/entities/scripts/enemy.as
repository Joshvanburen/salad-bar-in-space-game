void main()
{
	
}
int collide(WorldEntity &entity1, WorldEntity &entity2)
{
	PrintString("In Enemy Script");
	Enemy@ enemy = EntityToEnemy(entity1);
	Bullet@ bullet = EntityToBullet(entity2);
	if (enemy == null){
		Bullet@ bullet = EntityToBullet(entity1);
		Enemy@ enemy = EntityToEnemy(entity2
);
		if (bullet == null){
			PrintString("unknown collider");
			return 1;
		}
		
		soundManager.getSound("bullet_hit").play(false);
		entityManager.remove(entity1.getID());
		entityManager.remove(entity2.getID());
		//explosion
	}
	else{
		if (enemy == null){
		PrintString("unknown collider");
		return 1;
		}
	
		
		soundManager.getSound("bullet_hit").play(false);
		
	entityManager.remove(entity2.getID());
	entityManager.remove(entity1.getID());
	//explosion
	}
	
	
	return 1;
}