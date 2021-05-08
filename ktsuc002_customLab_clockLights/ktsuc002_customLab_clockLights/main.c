/*
 * ktsuc002_customLab_clockLights.c
 *
 * Created: 5/26/2019 10:15:14 PM
 * Author : Kris Tsuchiyama
 */ 

#include <avr/io.h>
#include "timer.h"
#include "bit.h"
typedef struct _task {
	unsigned char state;
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
	unsigned long int times; //num of times task happened
} task;
void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR0B &= 0x08; } //stops timer/counter
		else { TCCR0B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR0A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR0A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}
void PWM_on() {
	TCCR0A = (1 << WGM02) | (1 << WGM00) | (1 << COM0A0);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0); 
}
void PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}
char ClockLightBar(unsigned char time){
	unsigned char count = time;
	switch(count){
		case 10:
			PORTC = 0xFF;
			PORTD = 0xFF;
			--count;
			break;
		case 9:
			PORTC = 0xFF;
			PORTD = 0x80;
			--count;
			break;
		case 8:
			PORTC = 0xFF;
			PORTD = 0x00;
			--count;
			break;
		case 7:
			PORTC = 0xFE;
			--count;
			break;
		case 6:
			PORTC = 0xFC;
			--count;
			break;
		case 5:
			PORTC = 0xF8;
			--count;
			break;
		case 4:
			PORTC = 0xF0;
			--count;
			break;
		case 3:
			PORTC = 0xE0;
			--count;
			break;
		case 2:
			PORTC = 0xC0;
			--count;
			break;
		case 1:
			PORTC = 0x80;
			count = 10;
			break;
		default:
			count = 10;
		}
		return count;
}
char ClockLightNum(unsigned char min){
	unsigned char num = min;
	switch(num){
		case 3:
			PORTA = 0xA4;
			break;
		case 2:
			PORTA = 0xB0;
			break;
		case 1:
			PORTA = 0xED;
			break;
		default:
			PORTA = 0x3F;
	}
	if(num != 0){
		--num;
	}
	return num;
}
char makeSound(){
	while(!TimerFlag){
		set_PWM(3520.00);
	}
	TimerFlag = 0;
	while(!TimerFlag){
		set_PWM(2217.00);
	}
	TimerFlag = 0;
	while (!TimerFlag){
		set_PWM(0);
	}
	TimerFlag = 0;
	return 1;
}
int main(void)
{
	static task taskSound;
	static task taskMin; 
	static task taskBar;
	task *tasks[] = {&taskMin, &taskBar, &taskSound};
	unsigned char listSize = 3;
    DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFD; PORTB = 0x02;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	unsigned long count = 0;
	unsigned char stopSignal = PINB & 0x04;
	PWM_on();
	TimerOn();
	TimerSet(200);
	while(!TimerFlag){}
	TimerFlag = 0;
	while(1){
		unsigned char startButton = ~PINB & 0x02;
		if(startButton){
			break;
		}
	}
	//Set Up Tasks
		taskSound.state = 0;
		taskSound.period = 15;
		taskSound.elapsedTime = 15;
		taskSound.TickFct = &makeSound;
		taskSound.times = 0;
		
		taskMin.state = 3;
		taskMin.period = 300;
		taskMin.elapsedTime = 300;
		taskMin.TickFct = &ClockLightNum;
		taskMin.times = 0;
	
		taskBar.state = 10;
		taskBar.period = 30;
		taskBar.elapsedTime = 30;
		taskBar.TickFct = &ClockLightBar;
		taskBar.times = 0;
		
    while(!stopSignal) 
    {
		if(count == 520){
			taskSound.period = 5;
			taskSound.elapsedTime = 5;
		}
		if(count == 630){
			taskSound.period = 1;
			taskSound.elapsedTime = 1;
		}
		for(unsigned char i = 0; i < listSize; ++i){
			if(tasks[i]->elapsedTime >= tasks[i]->period){
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
				if(tasks[i]->TickFct == &makeSound){
					tasks[i-1]->elapsedTime += 3;
					tasks[i-2]->elapsedTime += 3;
				} 
				tasks[i]->times += 1;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag){	}
		TimerFlag = 0;		
		++count;
		if(taskBar.times == 31){
			PORTB = SetBit(2,0,1);
			PORTC = 0x00;
			PORTD = 0x00;
			break;
		}
		stopSignal = PINB & 0x04;
    }
}

