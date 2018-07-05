int Imageprocessor::crossedge(int index, std::vector<Imageprocessor::sobel> sobel)
{
	int cn; //teller
	
	if ((sobel[index].magnitude)) //check voor witte ranged
		cn++;

	if (cn&1)
		return true;

	return false;
}

void extent(int opp, std::vector<Imageprocessor::sobel> sobel)
{
	int prevhighestX, currentX, prevhighestY, currentY;

	for(int index; index <= sobel.size(); index++)
	{
		if(sobel[index].magnitude == 0)
		{
			index++;

			int indX = index;

			while(sobel[indX].magnitude != 0)
			{
				currentX++;
				
				while(sobel[(indX + (currentYfactor * width))].magnitude != 0)
				{
					currentY++;
					currentYfactor++;
				}
				if(currentY > prevhighestY)
				{
					prevhighestY = currentY;
				}
				
				indX++;
			}
			if(currentX > prevhighestX)
			{
				prevhighestX = currentX;
			}
		}
	}

	double area = ((prevhighestX * prevhighestY)/opp);

	if(prevhighestX == prevhighestY)
	{
		printf("Het is een vierkant.");
	}
	else if(area >= 0.17 && area <= 0.58)
	{
		printf("Het is een driehoek.");
	}
	else if(area >= 0.7 && area <= 0.86)
	{
		printf("Het is een cirkel.");
	}
	else if(area >= 0.92 && area <= 1)
	{
		printf("Dit object is niet bekend.");
	}
	else
	{
		printf("Het is een rechthoek.");
	}
}
int main()
{	
	int opp;

	for(int i = 0; i <= width*height; i++)
	{
		if(crossedge(i, sobel))
		{
			//doe die ding kompiler!!
			opp++;
			printf("Er is een puntje binnen een polygoon gevonden. Counter is %i\n", opp);
		}
	}

	printf("\n Opp = %i\n", opp);

	extent(opp, sobel);

	return 0;
}