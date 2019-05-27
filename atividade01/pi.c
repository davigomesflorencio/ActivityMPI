#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double monteCarlos(int n){
	srand(time(NULL));  
	int acertos=0,i=0;
	double x,y,z;
	for(i=0;i<=n;i++){
		x=(double) rand()/RAND_MAX;
		y=(double) rand()/RAND_MAX;
		//printf("%f ---%f \n",x,y);
		z = x * x + y * y;
		if(z<1) acertos++;
	}
	return (double)acertos*4/n;
}

int main(int argc, char *argv[]){
	int n=atoi(argv[1]);
	printf("pi = %.5f \n",monteCarlos(n));
	return 0;
}
