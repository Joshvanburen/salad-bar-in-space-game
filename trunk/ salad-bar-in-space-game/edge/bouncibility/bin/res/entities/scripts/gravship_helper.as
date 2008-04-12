void main()
{
	
}
int collide(WorldEntity &entity1, WorldEntity &entity2)
{
	PrintString("In Gravship Script");
	Ball@ ball1 = EntityToBall(entity1);
	Ball@ ball2 = EntityToBall(entity2);
	if (ball1 == null || ball2 == null){
		//collision between something that isn't two balls
	}
	else{
		PrintString("yay, they are both balls");
	}
	entity2.move();
	return 1;
}