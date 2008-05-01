void main()
{
	
}
int update(Enemy &enemy)
{
	PrintString("In coward ai Script");

	Enemy@ enemy1 = enemy;


	if (!enemy1.moveAwayFromPlayer(400)) {
		enemy1.moveToDest();
	}
	else {
		enemy1.genRandomDest();
	}


	return 1;
}

