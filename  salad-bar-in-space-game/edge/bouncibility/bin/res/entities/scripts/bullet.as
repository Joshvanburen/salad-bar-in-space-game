void main()
{
	
}
int collide(WorldEntity &entity1, WorldEntity &entity2)
{
	PrintString("In Bullet Script");
	Gravship@ gravship = EntityToGravship(entity1);
	Bullet@ bullet = EntityToBullet(entity2);
	if (gravship == null){
		Bullet@ bullet = EntityToBullet(entity1);
		Gravship@ gravship = EntityToGravship(entity2
);
		if (bullet == null){
			PrintString("unknown collider");
			return 1;
		}
		gravship.takeDamage(10);
		
		soundManager.getSound("bullet_hit").play(false);
		entityManager.remove(entity1.getID());
	}
	else{
		if (bullet == null){
		PrintString("unknown collider");
		return 1;
		}
		gravship.takeDamage(10);
		
		soundManager.getSound("bullet_hit").play(false);
	entityManager.remove(entity2.getID());
	}

	return 1;
}