#include <stdio.h>


static float inputVoltage = 0;

static float outputVoltage = 0;

static float R1 = 0;

static float R2 = 0;

static float impedance = 0;

int resistorDivider(void);

int main(int argc, char *argv[]){
	
	while(resistorDivider()){}

	return 0;
}

int resistorDivider(){
	printf("welcome to the resistor divider program\nPlease enter you input voltage:");
	scanf("%f", &inputVoltage);
	printf("\nPlease enter your desired output voltage:");
	scanf("%f", &outputVoltage);
	printf("\nPlease enter your desired overall impedance:");
	scanf("%f", &impedance);
	printf("\ncalculating...");
	R2 = outputVoltage*impedance/inputVoltage;
	R1 = impedance - R2;
	printf("\n\nResults: R1 = %f, R2 = %f\n\n", R1, R2);
	printf("press q to quit or r to run again:");
	while(getchar() != '\n'){}
	char nextAction = 0;
	scanf("%c", &nextAction);
	if(nextAction == 'r'){
		return 1;
	} else {
		return 0;
	}

}
