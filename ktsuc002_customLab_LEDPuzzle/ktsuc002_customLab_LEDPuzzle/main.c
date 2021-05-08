/*
 * ktsuc002_customLab_LEDPuzzle.c
 *
 * Created: 5/23/2019 5:48:11 PM
 * Author : Kris Tsuchiyama
 */ 

#include <avr/io.h>
#include "bit.h"
#include "timer.h"
#include "io.c"
#include "io.h"
#include "avr/eeprom.h"
//variables and column values for password puzzle
unsigned char cursorPos = 0x01;
unsigned char posA = 0x00;
unsigned char posB = 0x00;
unsigned char posC = 0x00;
unsigned char posD = 0x00;
unsigned char posE = 0x00;
unsigned char colA[] = {'Y','Z','O','K','S','Q','H'};
unsigned char colB[] = {'B','S','U','H','P','O','A'};
unsigned char colC[] = {'E','O','A','P','I','U','N'};
unsigned char colD[] = {'N','T','H','A','I','D','L'};
unsigned char colE[] = {'B','O','L','E','Y','H','N'};
unsigned char GetKeypadKey() {

	PORTC = 0x7F; // Enable col 4 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,3)==0) { return('1'); }
	if (GetBit(PINC,2)==0) { return('2'); }
	if (GetBit(PINC,1)==0) { return('3'); }
	if (GetBit(PINC,0)==0) { return('A'); }

	// Check keys in col 2
	PORTC = 0xBF; // Enable col 5 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,3)==0) { return('4'); }
	if (GetBit(PINC,2)==0) { return('5'); }
	if (GetBit(PINC,1)==0) { return('6'); }
	if (GetBit(PINC,0)==0) { return('B'); }
	// ... *****FINISH*****

	// Check keys in col 3
	PORTC = 0xDF; // Enable col 6 with 0, disable others with 1’s
	asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC,3)==0) { return('7'); }
	if (GetBit(PINC,2)==0) { return('8'); }
	if (GetBit(PINC,1)==0) { return('9'); }
	if (GetBit(PINC,0)==0) { return('C'); }
	// ... *****FINISH*****

	// Check keys in col 4
	PORTC = 0xEF;
	asm("nop");
	if (GetBit(PINC,3)==0) { return('*'); }
	if (GetBit(PINC,2)==0) { return('0'); }
	if (GetBit(PINC,1)==0) { return('#'); }
	if (GetBit(PINC,0)==0) { return('D'); }
	// ... *****FINISH*****

	return('\0'); // default value

}
void changeColumnA(){
	switch(cursorPos){
		case 1:
			if(posA == 6){
				posA = 0;
			}
			else{ ++posA; }
			break;
		case 2:
			if(posB == 6){
				posB = 0;
			}
			else{ ++posB; }
			break;
		case 3:
			if(posC == 6){
				posC = 0;
			}
			else{ ++posC; }
			break;
		case 4:
			if(posD == 6){
				posD = 0;
			}
			else{ ++posD; }
			break;
		case 5:
			if(posE == 6){
				posE = 0;
			}
			else{ ++posE; }
			break;
		default:
			LCD_Cursor(6);
			// do nothing
	}
	LCD_Cursor(cursorPos);
	switch(cursorPos){
		case 1:
			LCD_WriteData(colA[posA]);
			break;
		case 2:
			LCD_WriteData(colB[posB]);
			break;
		case 3:
			LCD_WriteData(colC[posC]);
			break;
		case 4:
			LCD_WriteData(colD[posD]);
			break;
		case 5:
			LCD_WriteData(colE[posE]);
			break;
		default:
			LCD_WriteData(colA[posA]);
	}
	LCD_Cursor(6);
}
void changeColumnB(){
	switch(cursorPos){
		case 1:
			if(posA == 0){
				posA = 6;
			}
			else{ --posA; }
			break;
		case 2:
			if(posB == 0){
				posB = 6;
			}
			else{ --posB; }
			break;
		case 3:
			if(posC == 0){
				posC = 6;
			}
			else{ --posC; }
			break;
		case 4:
			if(posD == 0){
				posD = 6;
			}
			else{ --posD; }
			break;
		case 5:
			if(posE == 0){
				posE = 6;
			}
			else{ --posE; }
			break;
		default:
			LCD_Cursor(6);
		// do nothing
	}
	LCD_Cursor(cursorPos);
	switch(cursorPos){
		case 1:
			LCD_WriteData(colA[posA]);
			break;
		case 2:
			LCD_WriteData(colB[posB]);
			break;
		case 3:
			LCD_WriteData(colC[posC]);
			break;
		case 4:
			LCD_WriteData(colD[posD]);
			break;
		case 5:
			LCD_WriteData(colE[posE]);
			break;
		default:
			LCD_WriteData(colA[posA]);
	}
	LCD_Cursor(6);
}
void changePosRight(){
	if(cursorPos == 5){
		cursorPos = 1;
	}
	else{ ++cursorPos;}
}
void changePosLeft(){
	if(cursorPos == 1){
		cursorPos = 5;
	}
	else{ --cursorPos;}
}
void generatePuzzleOneCharacters(){
	unsigned char OneCharacter1[8] = {0x04, 0x0E, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned char OneCharacter2[8] = {0x1F, 0x1B, 0x13, 0x1B, 0x1B, 0x11, 0x1F, 0x00};
	unsigned char OneCharacter3[8] = {0x00, 0x0E, 0x02, 0x0E, 0x08, 0x0E, 0x00, 0x00};
	unsigned char OneCharacter4[8] = {0x00, 0x0E, 0x02, 0x0E, 0x02, 0x0E, 0x00, 0x00};
	unsigned char OneCharacter5[8] = {0x00, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x00, 0x00};
	 
	LCD_BuildChar(1, OneCharacter1);
	LCD_BuildChar(2, OneCharacter2);
	LCD_BuildChar(3, OneCharacter3);
	LCD_BuildChar(4, OneCharacter4);
	LCD_BuildChar(5, OneCharacter5);
}
void generatePuzzleTwoCharacters(){
	unsigned char Character1[8] = {0x00, 0x04, 0x0C, 0x04, 0x04, 0x0E, 0x00, 0x00};
	unsigned char Character2[8] = {0x1F, 0x11, 0x1D, 0x11, 0x17, 0x11, 0x1F, 0x00};
	unsigned char Character3[8] = {0x00, 0x0E, 0x02, 0x0E, 0x02, 0x0E, 0x00, 0x00};
	unsigned char Character4[8] = {0x00, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x00, 0x00};
	unsigned char Symbol1[8] = {0x00, 0x04, 0x0A, 0x15, 0x0A, 0x04, 0x00, 0x00};
	unsigned char Symbol2[8] = {0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00};
	unsigned char Symbol3[8] = {0x00, 0x1B, 0x1B, 0x04, 0x1B, 0x1B, 0x00, 0x00};
	unsigned char Symbol4[8] = {0x00, 0x04, 0x0E, 0x1F, 0x1F, 0x04, 0x00, 0x00};
	LCD_BuildChar(0, Character1);
	LCD_BuildChar(1, Character2);
	LCD_BuildChar(2, Character3);
	LCD_BuildChar(3, Character4);
	LCD_BuildChar(4, Symbol1);
	LCD_BuildChar(5, Symbol2);
	LCD_BuildChar(6, Symbol3);
	LCD_BuildChar(7, Symbol4);
}
void generatePuzzleThreeCharacters(){
	unsigned char ThreeCharacter1[8] = {0x00, 0x04, 0x0C, 0x04, 0x04, 0x0E, 0x00, 0x00};
	unsigned char ThreeCharacter2[8] = {0x00, 0x0E, 0x02, 0x0E, 0x08, 0x0E, 0x00, 0x00};
	unsigned char ThreeCharacter3[8] = {0x1F, 0x11, 0x1D, 0x11, 0x1D, 0x11, 0x1F, 0x00};
	unsigned char ThreeCharacter4[8] = {0x00, 0x0A, 0x0A, 0x0E, 0x02, 0x02, 0x00, 0x00};
	unsigned char ThreeSymbol1[8] = {0x04, 0x04, 0x04, 0x04, 0x0A, 0x11, 0x15, 0x11};
	unsigned char ThreeSymbol2[8] = {0x00, 0x11, 0x0E, 0x0E, 0x04, 0x1F, 0x0E, 0x04};
	unsigned char ThreeSymbol3[8] = {0x0E, 0x04, 0x15, 0x1F, 0x1F, 0x15, 0x04, 0x0E};
	unsigned char ThreeSymbol4[8] = {0x02, 0x05, 0x0A, 0x14, 0x05, 0x0A, 0x14, 0x08};	
		
	LCD_BuildChar(0, ThreeCharacter1);
	LCD_BuildChar(1, ThreeCharacter2);
	LCD_BuildChar(2, ThreeCharacter3);
	LCD_BuildChar(3, ThreeCharacter4);
	LCD_BuildChar(4, ThreeSymbol1);
	LCD_BuildChar(5, ThreeSymbol2);
	LCD_BuildChar(6, ThreeSymbol3);
	LCD_BuildChar(7, ThreeSymbol4);
	
}
void generatePuzzleFourCharacters(){
	unsigned char FourCharacter1[8] = {0x00, 0x04, 0x0C, 0x04, 0x04, 0x0E, 0x00, 0x00};
	unsigned char FourCharacter2[8] = {0x00, 0x0E, 0x02, 0x0E, 0x08, 0x0E, 0x00, 0x00};
	unsigned char FourCharacter3[8] = {0x00, 0x0E, 0x02, 0x0E, 0x02, 0x0E, 0x00, 0x00};
	unsigned char FourCharacter4[8] = {0x1F, 0x15, 0x15, 0x11, 0x1D, 0x1D, 0x1F, 0x00};
	unsigned char playerChar[8] = {0x00, 0x04, 0x0E, 0x1F, 0x0E, 0x04, 0x00, 0x00};
	
	LCD_BuildChar(0, FourCharacter1);
	LCD_BuildChar(1, FourCharacter2);
	LCD_BuildChar(2, FourCharacter3);
	LCD_BuildChar(3, FourCharacter4);
	LCD_BuildChar(4, playerChar);
}
void generateMenuCharacters(){
	unsigned char DSchar1[8] = {0x0F, 0x1B, 0x17, 0x17, 0x1B, 0x1D, 0x0F, 0x07};
	unsigned char DSchar2[8] = {0x1E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1E, 0x1C};
	unsigned char DSchar3[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07};
	unsigned char DSchar4[8] = {0x00, 0x01, 0x00, 0x00, 0x0C, 0x13, 0x10, 0x1C};
	unsigned char DSchar5[8] = {0x01, 0x12, 0x01, 0x0C, 0x11, 0x00, 0x0A, 0x11};
	LCD_BuildChar(1, DSchar1);
	LCD_BuildChar(2, DSchar2);
	LCD_BuildChar(3, DSchar3);
	LCD_BuildChar(4, DSchar4);
	LCD_BuildChar(5, DSchar5);
}
void displayStartScreen(){
	generateMenuCharacters();
	LCD_DisplayString(1, "Press to         Defuse");
	LCD_Cursor(12);
	LCD_WriteData(3);
	LCD_Cursor(13);
	LCD_WriteData(4);
	LCD_Cursor(14);
	LCD_WriteData(5);
	LCD_Cursor(28);
	LCD_WriteData(1);
	LCD_Cursor(29);
	LCD_WriteData(2);
}
char wordLockPuzzle(){
	LCD_ClearScreen();
	unsigned char keypad = 0x00;
	unsigned char timeUp = 0x00;
	generatePuzzleOneCharacters();
	LCD_DisplayString(1, "ABENE");
	LCD_Cursor(29);
	LCD_WriteData(2);
	LCD_Cursor(30);
	LCD_WriteData(3);
	LCD_Cursor(31);
	LCD_WriteData(4);
	LCD_Cursor(32);
	LCD_WriteData(5);
	//LCD_WriteCommand(0x0C);
	while(1)
	{
		timeUp = PINB & 0x02;
		if(timeUp){
			return 0;
		}
		keypad = GetKeypadKey();
		switch(keypad){
			case 'A':
				changeColumnA();
				break;
			case 'B':
				changeColumnB();
				break;
			case 'C':
				changePosRight();
				break;
			case 'D':
				changePosLeft();
				break;
			default:
				LCD_Cursor(6);
		}
		while(!TimerFlag){
			LCD_Cursor(cursorPos+16); //
			LCD_WriteData(1); //Generates arrow under current column
		}
		LCD_Cursor(cursorPos+16);
		LCD_WriteData(' ');
		TimerFlag = 0;
		if(posA == 4 && posB == 4 && posC == 0 && posD == 6 && posE == 2){
								//SEND SIGNAL THAT PUZZLE IS CORRECT
			return 1;
		}
	}
	return 0;
}
char simonSaysPuzzle(){
	LCD_ClearScreen();
	generatePuzzleTwoCharacters();
	unsigned char userInput = 0x00;
	unsigned char errors = 0x00; //counts errors, if 3 -> lose
	unsigned char timeUp = 0x00;
	LCD_Cursor(29);    //
	LCD_WriteData(0);
	LCD_Cursor(30);
	LCD_WriteData(1);  // Prints out Puzzle #s
	LCD_Cursor(31);
	LCD_WriteData(2);
	LCD_Cursor(32);
	LCD_WriteData(3);  //
	
	while(1){
		timeUp = PINB & 0x02;
		if(timeUp){
			return 0;
		}
		LCD_Cursor(29);    //
		LCD_WriteData(0);
		LCD_Cursor(30);
		LCD_WriteData(1);  // Prints out Puzzle #s
		LCD_Cursor(31);
		LCD_WriteData(2);
		LCD_Cursor(32);
		LCD_WriteData(3);  //
		LCD_Cursor(1);	   // Simon Says #1, Heart -> Spade
		LCD_WriteData(5);
		userInput = GetKeypadKey();
		switch(userInput){
			case '1':
				LCD_Cursor(17);
				LCD_WriteData(4); // Diamond
				break;
			case '2':
				LCD_Cursor(17);
				LCD_WriteData(5); // Heart
				break;
			case '3':
				LCD_Cursor(17);
				LCD_WriteData(6); // Club
				break;
			case '4':
				LCD_Cursor(17);
				LCD_WriteData(7); // Spade
				break;
			default:
				LCD_Cursor(1);
				userInput = 0;
		}
		if(userInput != 0){      //Check for correct answer
			if(userInput == '4'){
				for(unsigned char i = 0; i < 4; ++i){
					while(!TimerFlag) {}
					TimerFlag = 0;
				}
				break;
			}
			else{
				LCD_DisplayString(1, "ERROR");
				++errors;
				for(unsigned char i = 0; i < 4; ++i){
					while(!TimerFlag) {}
					TimerFlag = 0;
				}
				LCD_ClearScreen();
			}
		}
		if(errors == 3){
			return 0;
		}
	}
	for(unsigned char a = 0; a < 4; a++){ //1 sec break in between commands
		while(!TimerFlag){}	
	}
	TimerFlag = 0;
	while(1){
		timeUp = PINB & 0x02;
		if(timeUp){
			return 0;
		}
		LCD_Cursor(29);    //
		LCD_WriteData(0);
		LCD_Cursor(30);
		LCD_WriteData(1);  // Prints out Puzzle #s
		LCD_Cursor(31);
		LCD_WriteData(2);
		LCD_Cursor(32);
		LCD_WriteData(3);  //
		LCD_Cursor(1);	   // Simon Says #1, Heart -> Spade
		LCD_WriteData(5);
		LCD_Cursor(17);
		LCD_WriteData(7);
		LCD_Cursor(2);	   // Simon Says #2, Diamond -> Heart
		LCD_WriteData(4);
		userInput = GetKeypadKey();
		switch(userInput){
			case '1':
			LCD_Cursor(18);
			LCD_WriteData(4); // Diamond
			break;
			case '2':
			LCD_Cursor(18);
			LCD_WriteData(5); // Heart
			break;
			case '3':
			LCD_Cursor(18);
			LCD_WriteData(6); // Club
			break;
			case '4':
			LCD_Cursor(18);
			LCD_WriteData(7); // Spade
			break;
			default:
			LCD_Cursor(1);
			userInput = 0;
		}
		if(userInput != 0){      //Check for correct answer
			if(userInput == '2'){
				for(unsigned char i = 0; i < 4; ++i){
					while(!TimerFlag) {}
					TimerFlag = 0;
				}
				break;
			}
			else{
				LCD_DisplayString(1, "ERROR");
				++errors;
				for(unsigned char i = 0; i < 4; ++i){
					while(!TimerFlag) {}
					TimerFlag = 0;
				}
				LCD_ClearScreen();
			}
		}
		if(errors == 3){
			return 0;
		}
	}
	for(unsigned char a = 0; a < 4; a++){ //1 sec break in between commands
		while(!TimerFlag){}
	}
	TimerFlag = 0;
	while(1){
		timeUp = PINB & 0x02;
		if(timeUp){
			return 0;
		}
		LCD_Cursor(29);    //
		LCD_WriteData(0);
		LCD_Cursor(30);
		LCD_WriteData(1);  // Prints out Puzzle #s
		LCD_Cursor(31);
		LCD_WriteData(2);
		LCD_Cursor(32);
		LCD_WriteData(3);  //
		LCD_Cursor(1);	   // Simon Says #1, Heart -> Spade
		LCD_WriteData(5);
		LCD_Cursor(17);
		LCD_WriteData(7);
		LCD_Cursor(2);	   // Simon Says #2, Diamond -> Heart
		LCD_WriteData(4);
		LCD_Cursor(18);
		LCD_WriteData(5);
		LCD_Cursor(3);     // Simon Says #3, Spade -> Club
		LCD_WriteData(7);
		userInput = GetKeypadKey();
		switch(userInput){
			case '1':
			LCD_Cursor(19);
			LCD_WriteData(4); // Diamond
			break;
			case '2':
			LCD_Cursor(19);
			LCD_WriteData(5); // Heart
			break;
			case '3':
			LCD_Cursor(19);
			LCD_WriteData(6); // Club
			break;
			case '4':
			LCD_Cursor(19);
			LCD_WriteData(7); // Spade
			break;
			default:
			LCD_Cursor(1);
			userInput = 0;
		}
		if(userInput != 0){      //Check for correct answer
			if(userInput == '3'){
				for(unsigned char i = 0; i < 4; ++i){
					while(!TimerFlag) {}
					TimerFlag = 0;
				}
				break;
			}
			else{
				LCD_DisplayString(1, "ERROR");
				++errors;
				for(unsigned char i = 0; i < 4; ++i){
					while(!TimerFlag) {}
					TimerFlag = 0;
				}
				LCD_ClearScreen();
			}
		}
		if(errors == 3){
			return 0;
		}
	}
	for(unsigned char a = 0; a < 4; a++){ //1 sec break in between commands
		while(!TimerFlag){}
	}
	TimerFlag = 0;
	while(1){
		timeUp = PINB & 0x02;
		if(timeUp){
			return 0;
		}
		LCD_Cursor(29);    //
		LCD_WriteData(0);
		LCD_Cursor(30);
		LCD_WriteData(1);  // Prints out Puzzle #s
		LCD_Cursor(31);
		LCD_WriteData(2);
		LCD_Cursor(32);
		LCD_WriteData(3);  //
		LCD_Cursor(1);	   // Simon Says #1, Heart -> Spade
		LCD_WriteData(5);
		LCD_Cursor(17);
		LCD_WriteData(7);
		LCD_Cursor(2);	   // Simon Says #2, Diamond -> Heart
		LCD_WriteData(4);
		LCD_Cursor(18);
		LCD_WriteData(5);
		LCD_Cursor(3);     // Simon Says #3, Spade -> Club
		LCD_WriteData(7);
		LCD_Cursor(19);
		LCD_WriteData(6);
		LCD_Cursor(4);
		LCD_WriteData(6);  // Simon Says #4, Club -> Diamond
		userInput = GetKeypadKey();
		switch(userInput){
			case '1':
			LCD_Cursor(20);
			LCD_WriteData(4); // Diamond
			break;
			case '2':
			LCD_Cursor(20);
			LCD_WriteData(5); // Heart
			break;
			case '3':
			LCD_Cursor(20);
			LCD_WriteData(6); // Club
			break;
			case '4':
			LCD_Cursor(20);
			LCD_WriteData(7); // Spade
			break;
			default:
			LCD_Cursor(1);
			userInput = 0;
		}
		if(userInput != 0){      //Check for correct answer
			if(userInput == '1'){
				for(unsigned char i = 0; i < 4; ++i){
					while(!TimerFlag) {}
					TimerFlag = 0;
				}
				return 1;
			}
			else{
				LCD_DisplayString(1, "ERROR");
				++errors;
				for(unsigned char i = 0; i < 4; ++i){
					while(!TimerFlag) {}
					TimerFlag = 0;
				}
				LCD_ClearScreen();
			}
		}
		if(errors == 3){
			return 0;
		}
	}
	return 0;
}
char matchSymbolsPuzzle(){
	LCD_ClearScreen();
	generatePuzzleThreeCharacters();
	unsigned char userInput = 0x00;
	unsigned char timeUp = 0x00;
	LCD_Cursor(29);
		LCD_WriteData(0);
	LCD_Cursor(30);
		LCD_WriteData(1);
	LCD_Cursor(31);
		LCD_WriteData(2);
	LCD_Cursor(32);
		LCD_WriteData(3);
		
	LCD_Cursor(1);
		LCD_WriteData(4);
	LCD_Cursor(2);
		LCD_WriteData(5);
	LCD_Cursor(3);
		LCD_WriteData(6);
	LCD_Cursor(4);
		LCD_WriteData(7);
	while(1){
		timeUp = PINB & 0x02;
		if(timeUp){
			return 0;
		}
		userInput = GetKeypadKey();
		switch(userInput){
			case '3' : 
				userInput = 3;
				break;
			case '\0':
				userInput = 0;
				break;
			default:
				userInput = 1;
		}
		while(!TimerFlag){}
		TimerFlag = 0;
		if(userInput != 0){
			if(userInput == 3){
				return 1;
			}
			else{
				return 0;
			}
		}
	}
	return 0;
}
char mazePuzzle(){
	unsigned char correctMove = 0;
	unsigned char userMove = 0x00;
	unsigned char userNum = 0;
	unsigned short currentPosition = 16;
	unsigned char answerKey[22] = {8,4,4,2,4,4,8,4,4,2,4,4,8,4,4,4,2,4,4,4,8,4};
	LCD_ClearScreen();
	generatePuzzleFourCharacters();
	LCD_Cursor(16);
		LCD_WriteData(4); //Puts player char at right end of top LCD row
	LCD_Cursor(1);
		LCD_WriteData('O');
	LCD_Cursor(29);
		LCD_WriteData('O'); //Establish maze 
	while(1){
		userMove = GetKeypadKey();
		switch(userMove){
			case '2':
				userNum = 2;
				break;
			case '4':
				userNum = 4;
				break;
			case '6':
				userNum = 6;
				break;
			case '8':
				userNum = 8;
				break;
			default:
				userNum = 0;
		} 
		if(userNum != 0){		//Wait for input, check if right
			if(userNum != answerKey[correctMove]){
				return 0;
			}
			switch(answerKey[correctMove]){
				case 2:
					LCD_DisplayString(1,"                ");
					currentPosition -= 16;
					LCD_Cursor(currentPosition);
						LCD_WriteData(4);
					LCD_Cursor(1);
						LCD_WriteData('O');
					LCD_Cursor(29);
						LCD_WriteData('O');
					correctMove++;
					break;
				case 4:
					LCD_DisplayString(1,"                ");
					currentPosition -= 1;
					LCD_Cursor(currentPosition);
						LCD_WriteData(4);
					LCD_Cursor(1);
						LCD_WriteData('O');
					LCD_Cursor(29);
						LCD_WriteData('O');
					correctMove++;
					break;
				case 6:
					LCD_DisplayString(1,"                ");
					currentPosition += 1;
					LCD_Cursor(currentPosition);
						LCD_WriteData(4);
					LCD_Cursor(1);
						LCD_WriteData('O');
					LCD_Cursor(29);
						LCD_WriteData('O');
					correctMove++;
					break;
				case 8:
					LCD_DisplayString(1,"                ");
					currentPosition += 16;
					LCD_Cursor(currentPosition);
						LCD_WriteData(4);
					LCD_Cursor(1);
						LCD_WriteData('O');
					LCD_Cursor(29);
						LCD_WriteData('O');
					correctMove++;
					break;
				default:
					return 0;
			}
		}
		while(!TimerFlag){}
			TimerFlag = 0;
		if(correctMove == 22){
			return 1;
		}
	}
	return 0;
}
void resetEEPROM(){
	uint8_t zero = 0;
	eeprom_update_byte(0,zero);
}
void updateEEPROM(){
	uint8_t DataValue;
	DataValue = eeprom_read_byte(0);

	//DataValue = DataValue + 1;
	
	eeprom_update_byte(0,DataValue + 1);
}
void getDefuses(){
	uint8_t Data;
	Data = eeprom_read_byte(0);
	
	switch(Data){
		case 0:
			LCD_Cursor(16);
			LCD_WriteData('0');
			break;
		case 1:
			LCD_Cursor(16);
			LCD_WriteData('1');
			break;
		case 2:
			LCD_Cursor(16);
			LCD_WriteData('2');
			break;
		case 3:
			LCD_Cursor(16);
			LCD_WriteData('3');
			break;
		case 4:
			LCD_Cursor(16);
			LCD_WriteData('4');
			break;
		case 5:
			LCD_Cursor(16);
			LCD_WriteData('5');
			break;
		case 6:
			LCD_Cursor(16);
			LCD_WriteData('6');
			break;
		case 7:
			LCD_Cursor(16);
			LCD_WriteData('7');
			break;
		case 8:
			LCD_Cursor(16);
			LCD_WriteData('8');
			break;
		case 9:
			LCD_Cursor(16);
			LCD_WriteData('9');
			break;
		case 10:
			LCD_Cursor(15);
			LCD_WriteData('10');
			break;
		case 11:
			LCD_Cursor(15);
			LCD_WriteData('11');
			break;
		case 12:
			LCD_Cursor(15);
			LCD_WriteData('12');
			break;
		case 13:
			LCD_Cursor(15);
			LCD_WriteData('13');
			break;
		case 14:
			LCD_Cursor(15);
			LCD_WriteData('14');
			break;
		case 15:
			LCD_Cursor(15);
			LCD_WriteData('15');
			break;
		case 16:
			LCD_Cursor(15);
			LCD_WriteData('16');
			break;
		case 17:
			LCD_Cursor(15);
			LCD_WriteData('17');
			break;
		case 18:
			LCD_Cursor(15);
			LCD_WriteData('18');
			break;
		case 19:
			LCD_Cursor(15);
			LCD_WriteData('19');
			break;
		case 20:
			LCD_Cursor(15);
			LCD_WriteData('20');
			break;
		default:
			eeprom_update_byte(0,0);
			LCD_Cursor(16);
			LCD_WriteData('0');
	}
}
int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0x0C; PORTB = 0xF3;
	DDRC = 0xF0; PORTC = 0x0F;
	DDRD = 0xFF; PORTD = 0x00;
	unsigned char checkPuzzle = 0x00;
	TimerOn();
	TimerSet(300);
	LCD_init();
	LCD_WriteCommand(0x0C);
	displayStartScreen();
	while(1){ //START GAME
		unsigned char startButton = ~PINB & 0x01;
		if(startButton){
			break;
		}
	}
	while (1) 
    {
		LCD_ClearScreen();
		checkPuzzle = wordLockPuzzle();
			if(checkPuzzle == 1){
				LCD_DisplayString(1, "STEP 1 COMPLETE Press to Proceed");
				while(1){
					unsigned char startButton = ~PINB & 0x01;
					if(startButton){
						break;
					}
				}
			}
			if(checkPuzzle == 0){
				LCD_DisplayString(1, "    *KABOOM*        YOU LOSE");
				for(unsigned char i = 0; i < 8; ++i){
					while(!TimerFlag){	}
					TimerFlag = 0;
				}
				break;
			}
		if(checkPuzzle == 1){
			LCD_ClearScreen();
			checkPuzzle = simonSaysPuzzle();
		}
			if(checkPuzzle == 1){
				LCD_DisplayString(1, "STEP 2 COMPLETE Press to Proceed");
				while(1){
					unsigned char startButton = ~PINB & 0x01;
					if(startButton){
						break;
					}
				}
			}
			if(checkPuzzle == 0){
				LCD_DisplayString(1, "    *KABOOM*        YOU LOSE");
				for(unsigned char i = 0; i < 8; ++i){
					while(!TimerFlag){	}
					TimerFlag = 0;
				}
				break;
			} 
		if(checkPuzzle == 1){
			LCD_ClearScreen();
			checkPuzzle = matchSymbolsPuzzle();
		}
			if(checkPuzzle == 1){
				LCD_DisplayString(1, "STEP 3 COMPLETE Press to Proceed");
				while(1){
					unsigned char startButton = ~PINB & 0x01;
					if(startButton){
						break;
					}
				}
			}
			if(checkPuzzle == 0){
				LCD_DisplayString(1, "    *KABOOM*        YOU LOSE");
				for(unsigned char i = 0; i < 8; ++i){
					while(!TimerFlag){	}
					TimerFlag = 0;
				}
				break;
			}
		if(checkPuzzle == 1){
			LCD_ClearScreen();
			checkPuzzle = mazePuzzle();
		}
			if(checkPuzzle == 1){
				LCD_DisplayString(1, "MODULES COMPLETE  BOMB DEFUSED");
				for(unsigned char i = 0; i < 8; ++i){
					while(!TimerFlag){	}
					TimerFlag = 0;
				}
				updateEEPROM();
				break;
			}
			if(checkPuzzle == 0){
				LCD_DisplayString(1, "    *KABOOM*        YOU LOSE");
				for(unsigned char i = 0; i < 8; ++i){
					while(!TimerFlag){	}
					TimerFlag = 0;
				}
				break;
			} 
    }
	LCD_ClearScreen();
	//LCD_DisplayString(1, "                                ");
	PORTA = 0xFF;
	LCD_DisplayString(1, "BOMBS DEFUSED:");
	/*resetEEPROM();
	updateEEPROM(); */
	getDefuses();
}

