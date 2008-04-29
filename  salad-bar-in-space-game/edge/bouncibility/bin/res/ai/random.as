void main()
{
	
}
int update(Enemy &enemy)
{
	PrintString("In random ai Script");

	Enemy@ enemy1 = enemy;

	if (!enemy1.moveToDest()) {
		enemy1.genRandomLoc();
	}


	return 1;
}

