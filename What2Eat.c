/******************************************************************************

    This file is part of What2Eat.

    What2Eat is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    What2Eat is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with What2Eat.  If not, see <https://www.gnu.org/licenses/>.

*******************************************************************************/
#include <stdio.h>

struct FoodRating
{
  int foodNumber;
  int rating;
};

struct FoodRating SortedRatings[5];



int pointsForFood[5];

void ApplyRating (struct FoodRating arr[5],  int foodNumber_, int pointsGiven)
{


  for (int i = 0; i < 5; i++)
    {
        
        if (arr[i].foodNumber == foodNumber_)
        {
            //printf("i: %d foodnum: %d , foodrating: %d; ratinggiven: %d \n", i ,arr[i].foodNumber,arr[i].rating, pointsGiven);
	        arr[i].rating += pointsGiven;
	    }

    }
}

void
swap (struct FoodRating * xp,struct  FoodRating * yp)
{
  struct FoodRating temp = *xp;
  *xp = *yp;
  *yp = temp;
}

// Function to perform Selection Sort
void
selectionSort (struct FoodRating arr[5], int n)
{
  int i, j, max_idx;
  struct FoodRating currentFoodRating;
  int currentPoints;
  struct FoodRating maxFoodRating;

  // One by one move boundary of unsorted subarray
  for (i = 0; i < n; i++)
    {
  
      currentFoodRating = arr[i];	//Current rating that i is on
      // Find the maximum element in unsorted array
      max_idx = i;		// we assume this one is the max for commparison
      for (j = i + 1; j < n; j++){
          //printf("i: %d j: %d, n: %d \n",i,j,n);
           if (arr[j].rating > arr[max_idx].rating)
	       max_idx = j;
      }
	   

      // Swap the found minimum element
      // with the first element
      
      swap (&arr[max_idx], &arr[i]);	//here swap() should swap places of two food ratings
    }
}

void
SayFoods (int foodNumber)
{
  switch (foodNumber)
    {
    case 1:
      // statements
      printf (" 1->kebab \n");
      break;

    case 2:
      // statements
      printf (" 2->Burger\n");
      break;
    case 3:
      // statements
      printf (" 3->Ramen \n");
      break;
    case 4:
      // statements
      printf (" 4->Lahmacun \n");
      break;
    case 5:
      // statements
      printf (" 5->Fried Rice \n");
      break;

    default:
      break;
    }

};

void PrintRatings(struct FoodRating arr[5]){
    
    for (int i = 0; i <5 ; ++i){
        printf(" Food rank:%d ,FoodNumber:%d ,FoodPoint: %d \n", i,arr[i].foodNumber, arr[i].rating);
    }
    printf("\n");
}

int main ()
{

  SortedRatings[0].foodNumber = 0;
  struct FoodRating SortedRatings[5];
  for(int i = 0; i<5; i++){
  	
  		SortedRatings[i].foodNumber = i+1;
  		SortedRatings[i].rating = 0;
  }

  int x;			// num of people

  printf ("What to eat, what to eat?\n");
  printf ("Your choises are:/n 1->kebab, 2->Burger, 3->Ramen, 4->Lahmacun,5->Fried Rice \n");
  printf ("How many people want food? ->");

  
  scanf("%d",&x);
  printf ("\n Number of people who want food is:");
  putchar (x);
  printf ("\n");

  for (int i = 0; i < x; ++i)
    {
      printf ("I am asking a new person their choises.Enter their numbers.\n");
      
      for(int k =0; k<5;k++){
           printf ("\n Your #%d food: ",k+1);
           int omega;
           scanf("%d",&omega);
          
           ApplyRating (SortedRatings, omega, (5-k));
           printf ("\n");
      }
    }
  PrintRatings(SortedRatings);
  
  selectionSort (SortedRatings,5);
  
  PrintRatings(SortedRatings);
  printf ("\n I am now looking if they pass 10 points \n ");

  int notEligibleCount=0;
  for (int i = 0; i < 5; i++)
    {
      if (SortedRatings[i].rating < 10)
	  {
	     SortedRatings[i].rating = 0;
	     notEligibleCount++;
	      
	  }
      else
	  {
	  SayFoods (SortedRatings[i].foodNumber);  
	  }
    }

  if (notEligibleCount == 5)
    {
      printf ("\nYou're gonna starve today sorry :///\n");
      return 0;
    }
  int remaining = 5- notEligibleCount; 
  selectionSort (SortedRatings, remaining);// I am sorting after filtering with 10
  
  //for (int i = 0; i <remaining; i++)
  //{
  //  printf ("Your # %d food : ", i+1);
//    int theta;
//    scanf("%d",&theta);
//    ApplyRating (SortedRatings, theta, i);
 //   printf ("\n");
//  }
 
 for (int i = 0; i < x; i++)
    {
      printf ("I am asking a new person their choises from this list.Enter their numbers.\n");
      
      for(int k =0; k<remaining;k++){
           printf ("\n Your #%d food: ",k+1);
           int omega;
           scanf("%d",&omega);
           ApplyRating (SortedRatings, omega, (remaining-k));
           printf ("\n");
      }
    }
    
    selectionSort (SortedRatings, remaining);
  printf ("YOU ARE EATING:");
  SayFoods (SortedRatings[0].foodNumber);

return 0;

}
