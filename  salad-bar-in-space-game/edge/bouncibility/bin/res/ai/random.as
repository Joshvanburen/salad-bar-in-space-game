void main()
{
	
}
int update(Enemy &enemy)
{
	PrintString("In random ai Script");

	Enemy@ enemy1 = enemy;

	if (!enemy1.moveNowhere()) {
		enemy1.genRandomNW();
	}


	return 1;
}

