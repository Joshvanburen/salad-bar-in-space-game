void main()
{
	
}
int update(Enemy &enemy)
{
	PrintString("In shooter ai Script");

	Enemy@ enemy1 = enemy;

	if (!enemy1.moveToDest()) {
		enemy1.genRandomDest();
	}

	enemy1.shootPlayer();

	return 1;
}

