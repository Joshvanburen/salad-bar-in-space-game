void main()
{
	
}
int update(Enemy &enemy)
{
	PrintString("In shooter ai Script");

	Enemy@ enemy1 = enemy;

	if (!enemy1.moveNowhere()) {
		enemy1.genRandomNW();
	}

	enemy1.shootPlayer();

	return 1;
}

