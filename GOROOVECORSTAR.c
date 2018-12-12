//���ׂĕW���֐��ł�
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>

int tunesRead(char tunesList[1000][50]);
int songSelectionScreen(char tunesList[1000][50], int musicNumber);
void outFrame(void);
void outTunes(char tunesList[1000][50], int selectedSong, int musicNumber);
void strcln(char *s1, char *s2);
char* streqlcpy(char *s1, int size);
void mainScreenOut(int tempo, int musicLength);
int readNotes(char *fileName, int *bpm, int *notesMax);
void newNoteArrey(int arrayLength);
void initializeNote(int arrayLength, int timing, int length, int kind);
void initializeY(int arrayLength);
void setY(int arrayLength, int y);
int getY(int arrayLength);
int getOldY(int arrayLength);
int getTiming(int arrayLength);
int getLength(int arrayLength);
int getKind(int arrayLength);
void downY(int arrayLength);

clock_t start;

//�����X���b�h�֐�
DWORD WINAPI startMusic(LPVOID arg)
{
	//�Ȃ̍Đ�
	PlaySound(arg, NULL, (SND_ASYNC|SND_FILENAME) );
	
	start = clock();
	
	return 0;
}

int main(void){
	
	//�Ȗڂ��i�[����ϐ��B1000�ȁA1�Ȃ�����50�����܂őΉ��B
	char tunesList[1000][50];
	
	//�ǂݍ��ދȐ��̍ő�l���i�[����ϐ��B
	int musicNumber;
	
	//���݂̋Ȕԍ����i�[����ϐ��B
	int serectMusicNumber;
	
	//�X���b�h�����p�̕ϐ��B
	HANDLE hThread;
	DWORD dwThreadId;
	
	//�ȃt�@�C���̖��O���i�[����ϐ��B
	char* musicFileName;
	
	int musicLength;
	
	int notesMax;
	int bpm;
	int tempo;
	
	Sleep(3000);
	
	//��ʂ̏������B
	system("cls");
	
	//�Ȃ̍ő吔���i�[�B
	musicNumber = tunesRead(tunesList);
	
	//�ȑI����ʂ�\�����A�I�����ꂽ�Ȕԍ����i�[�B
	serectMusicNumber = songSelectionScreen(tunesList, musicNumber);
	
	//���݂̋Ȗ���.wav�g���q��t���܂��B(�Ȗ�.wav�����̋Ȃ̃t�@�C�����ł��B)
	strcat(tunesList[serectMusicNumber], ".wav");
	
	musicFileName = streqlcpy(tunesList[serectMusicNumber], sizeof(tunesList[serectMusicNumber]) / sizeof(char));
	
	//�X���b�h�̐���
	hThread = CreateThread(
		NULL, //�Z�L�����e�B����
		0, //�X�^�b�N�T�C�Y
		startMusic, //�X���b�h�֐�
		musicFileName, //�X���b�h�֐��ɓn������
		0, //�쐬�I�v�V����(0�܂���CREATE_SUSPENDED)
		&dwThreadId//�X���b�hID
	);
	
	//��قǕ�����ɑ}������.wav�������܂��B
	strcln(tunesList[serectMusicNumber], ".wav");
	
	musicLength = readNotes(strcat(tunesList[serectMusicNumber], ".txt"), &notesMax, &bpm);
	
	tempo = 60 * 1000 / bpm;
	
	mainScreenOut(tempo, musicLength);
	
	//��قǕ�����ɑ}������.txt�������܂��B
	strcln(tunesList[serectMusicNumber], ".txt");
	
	//�X���b�h�̏I����҂�
	WaitForSingleObject(hThread, INFINITE);

	//�X���b�h�̃n���h�������
	CloseHandle(hThread);
	
	Sleep(1);
	
	return 0;
	
}

int tunesRead(char tunesList[1000][50]){
	
	//�Ȗڃ��X�g��ǂݍ��ނ��߂̃t�@�C���|�C���^�ϐ��B
	FILE *tunes;
	
	//���[�v�ϐ�
	int i;
	
	/*�����f�B���N�g����tunes.txt�ɋȖڃ��X�g�������Ă��܂��B
	���S�̂��߁A�ǂݍ��ݐ�p�ŊJ���Ă��������B*/
	tunes = fopen("tunes.txt", "r");
	
	for(i = 0; fscanf(tunes, "%s", tunesList[i]) != EOF; i++);
	
	//�t�@�C������܂��B
	fclose(tunes);
	
	//�Ȃ̍ő吔��Ԃ��܂��B
	return i;
	
}

int songSelectionScreen(char tunesList[1000][50], int musicNumber){
	
	//���ݑI�𒆂̋Ȕԍ����i�[����ϐ��B
	int selectedSong = 0;
	
	//���͂��ꂽ�L�[�̒l���i�[����ϐ��B
	char key = 0;
	
	//�g��\���B
	outFrame();
	//�Ȗڂ�\���B
	outTunes(tunesList, selectedSong, musicNumber);
	
	//Enter�L�[���������܂ŁB
	do{
		
		if(kbhit()){
			
			switch(key = getch()){
				
				/*w�܂���i�܂��́��������ꂽ�烊�X�g��1��ɁB
				�����̏����I�ɂ͓Y������1���Ȃ��B
				*/
				case 0x48:
				case 119:
				case 105:
				  if(selectedSong > 0)
				  	  selectedSong--;
				  //�I�𒆂̋Ȃ��ŏ��̋ȂȂ�A1��̋Ȃ͍Ō�̋ȂƂȂ�B
				  else
				      selectedSong = musicNumber - 1;
				  break;
				
				
				/*x�܂���,�܂��́��������ꂽ�烊�X�g��1���ɁB
				�����̏����I�ɂ͓Y������1�����B
				*/
				case 0x50:
				case 120:
				case 44:
				  if(selectedSong < musicNumber - 1)
				      selectedSong++;
				  
				  //�I�𒆂̋Ȃ��Ō�̋ȂȂ�A1���̋Ȃ͍ŏ��̋ȂƂȂ�B
				  else
				  	  selectedSong = 0;
			      break;
			}
			
			//�Ȗڂ�\���B
			outTunes(tunesList, selectedSong, musicNumber);
		}
		
	}while(key != 13);
	
	//��ʂ̏������B
	system("cls");
	
	//���݂̋Ȕԍ���Ԃ��B
	return selectedSong;
}

void outFrame(void){
	
	//���[�v�ϐ�
	int i;
	
	/*3��g��\������B
	�g�̑������c��3�}�X�Ȃ̂ŁAi*3-2���g�̏�[�̍��W�ƂȂ�B
	2�ڂ̘g�́A���ݑI�𒆂̋ȂȂ̂ŁA�g�������B
	*/
	for(i = 1; i <= 3; i++){
		gotoxy(1, i * 3 - 2);
		if(i == 2)
			printf("��========================================��\n");
			printf("���\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\��\n");
		gotoxy(1, i * 3 - 1);
		if(i == 2)
			printf("||                                        ||");
		else
			printf("|                                          |");
		gotoxy(1, i * 3);
		if(i == 2)
			printf("��========================================�v\n");
			printf("���\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�v\n");
	}
}

void outTunes(char tunesList[1000][50], int selectedSong, int musicNumber){
	
	//���[�v�ϐ�
	int i;
	
	//���݂̋Ȕԍ�(������ł�)
	int systemSelectedSong;
	
	/*3�̋Ȃ�g�̒��ɕ\���B
	selectedSong���^�񒆂̋ȂȂ̂ŁAi=-1����̃X�^�[�g�B
	*/
	for(i = -1; i < 2; i++){
		
		/*�\������Ȃ̋Ȕԍ����i�[�B
		�\������Ȕԍ���0�`�ő�l�Ȃ炻�̂܂܁B
		0�ȉ��Ȃ�Ȕԍ��̍ő�l����B
		�ő�l�ȏ�Ȃ�ŏ�����B
		*/
		systemSelectedSong = selectedSong + i;
		systemSelectedSong = systemSelectedSong < 0 ? musicNumber + i : (systemSelectedSong > musicNumber - 1 ? i - 1 : systemSelectedSong);
		
		/*�g�̑������c��3�}�X�Ȃ̂ŁAi*3+2���g�̒��S�̍��W�ƂȂ�B
		i�������ƌv�Z���Â炢�̂ŁA���i+1����B
		���Ȃ݂�x���W�͘g��2�}�X����̂ŁA3����̕\���B
		*/
		gotoxy(3, (i + 1) * 3 + 2);
		
		//40���ȓ��ŋȖ���\���B
		printf("%-40s", tunesList[systemSelectedSong]);
	}
}

/*������s1���炠�镶����s2���폜����֐��B
���݂܂���l�b�g����̃R�s�y�ł��B
*/

void strcln(char *s1, char *s2){

	char *p;
	p = strstr(s1, s2);
	if (p != NULL) {
		strcpy(p, p + strlen(s2));
		strcln(p + 1, s2);
	}
}

/*������̃R�s�[�p�̊֐��B
�R�s�[���ꂽ�z���Ԃ��B
*/
char* streqlcpy(char *s1, int size){
	
	char* subString;
	int i;
	
	subString = (char*)malloc(size * sizeof(char));
	
	for(i = 0; i <= size; i++){
		
		subString[i] = s1[i];
		
	}
	
	return subString;
}

void mainScreenOut(int tempo, int musicLength){
	int i, j = 0, k;
	int notePlace;
	clock_t now, old;
	clock_t nowNote, oldNote;
	int tapFlag = 0;
	char c;
	int count = 0;
	int longFlag = 0;
	int m;
	
	for(i = 1; i < 42; i++){
		
		gotoxy(40, i);
		
		if(i != 40){
			printf("| ");
		}else{
			printf("��");
		}
		
	}
	
	gotoxy(47, 5);
	printf("score");
	gotoxy(46, 6);
	printf("|     |");
	gotoxy(46, 7);
	printf("-------");
	
	i = 0;
	now = nowNote = start;
	oldNote = start;
	while(start - now <= musicLength){
		gotoxy(47, 6);
		printf("%5d", count);
#if 1		
		if(now - start <= getTiming(i) - tempo * 20 + 20 && now - start >= getTiming(i) - tempo * 20 - 20){
			i++;
		}
		nowNote = now;
		if(nowNote - oldNote > tempo - 1){
			oldNote = nowNote;
			for(k = j; k < i; k++){
				downY(k);
				notePlace = getY(k);
				if(notePlace <= 20 && notePlace >= 19)
					tapFlag = k;
				if(notePlace >= 21 && (getKind(k) != 3 && getKind(k) != 5 && getKind(k) != 6 && getKind(k) != 71 && getKind(k) != 73)){
					tapFlag = 0;
					j++;
					continue;
				
				}
				if((getKind(k) == 3 || getKind(k) == 5 || getKind(k) == 6 || getKind(k) == 71 || getKind(k) == 73) && notePlace <= 20 && notePlace >= 19){
					longFlag = 1;
				}
				if((getKind(k) == 3 || getKind(k) == 5 || getKind(k) == 6 || getKind(k) == 71 || getKind(k) == 73) && notePlace >= 21 && notePlace - getLength(k) / tempo < 21){
					gotoxy(40, 21);
					printf("| ");
					
				}
				if((getKind(k) == 3 || getKind(k) == 5 || getKind(k) == 6 || getKind(k) == 71 || getKind(k) == 73) && notePlace - getLength(k) / tempo == 21 && longFlag){
					gotoxy(40, 21);
					printf("| ");
					longFlag = 0;
					tapFlag = 0;
					j++;
					continue;
					
				}
				gotoxy(40, notePlace);
				switch(getKind(k)){
					case 1:
						printf("��");
						break;
					case 2:
						
						printf("\x1b[96m");
						printf("��");
						printf("\x1b[39m");
						break;
					case 3:
							if(notePlace < 21){
								printf("\x1b[93m");
								printf("�Z");
								printf("\x1b[39m");
							}
							for(m = 1; m < getLength(k) / tempo; m++){
								if(notePlace - m <= 0)
									break;
								if(notePlace - m < 21){
									gotoxy(40, notePlace - m);
									printf("\x1b[93m");
									printf("||");
									printf("\x1b[39m");
									if(notePlace - getLength(k) / tempo - m > 0){
										gotoxy(40, notePlace - getLength(k) / tempo - m);
										printf("| ");
									}
								}
							}
							if(!(getLength(k) / tempo < 2)){
								if(notePlace - getLength(k) / tempo < 21){
									gotoxy(40, notePlace - getLength(k) / tempo);
									printf("\x1b[93m");
									printf("�Z");
									printf("\x1b[39m");
									if(notePlace - getLength(k) / tempo - 1 > 0){
										gotoxy(40, notePlace - getLength(k) / tempo - 1);
										printf("| ");
									}
								}
							}
						break;
					case 4:
						printf("\x1b[95m");
						if(getLength(k) == 1){
							printf("��");
						}else if(getLength(k) == 2){
							printf("��");
						}else if(getLength(k) == 3){
							printf("��");
						}else if(getLength(k) == 4){
							printf("��");
						}else if(getLength(k) == 13){
							gotoxy(39, notePlace);
							printf("����");
						}else if(getLength(k) == 34){
							gotoxy(38, notePlace);
							printf("����");
						}else if(getLength(k) == 14){
							printf("����");
						}else if(getLength(k) == 23){
							gotoxy(38, notePlace);
							printf("����");
						}else if(getLength(k) == 24){
							gotoxy(39, notePlace);
							printf("����");
						}else if(getLength(k) == 12){
							gotoxy(38, notePlace);
							printf("����");
						}else if(getLength(k) == 5){
							printf("�^");
						}else if(getLength(k) == 6){
							printf("�_");
						}else if(getLength(k) == 7){
							printf("�^");
						}else if(getLength(k) == 8){
							printf("�_");
						}
						printf("\x1b[39m");
						break;
					case 5:
							if(notePlace < 21){
								printf("\x1b[38;5;166m");
								printf("��");
								printf("\x1b[39m");
							}
							for(m = 1; m < getLength(k) / tempo; m++){
								if(notePlace - m <= 0)
									break;
								if(notePlace - m < 21){
									gotoxy(40, notePlace - m);
									printf("\x1b[38;5;166m");
									printf("||");
									printf("\x1b[39m");
									if(notePlace - getLength(k) / tempo - m > 0){
										gotoxy(40, notePlace - getLength(k) / tempo - m);
										printf("| ");
									}
								}
							}
							if(!(getLength(k) / tempo < 2)){
								if(notePlace - getLength(k) / tempo < 21){
									gotoxy(40, notePlace - getLength(k) / tempo);
									printf("\x1b[38;5;166m");
									printf("��");
									printf("\x1b[39m");
									if(notePlace - getLength(k) / tempo - 1 > 0){
										gotoxy(40, notePlace - getLength(k) / tempo - 1);
										printf("| ");
									}
								}
							}
						break;
					case 6:
							if(notePlace < 21){
								printf("\x1b[92m");
								printf("��");
								printf("\x1b[39m");
							}
							for(m = 1; m < getLength(k) / tempo; m++){
								if(notePlace - m <= 0)
									break;
								if(notePlace - m < 21){
									gotoxy(40, notePlace - m);
									printf("\x1b[92m");
									if(notePlace % 2 == 0){
										printf("S");
									}else{
										printf("2");
									}
									printf("\x1b[39m");
									if(notePlace - getLength(k) / tempo - m > 0){
										gotoxy(40, notePlace - getLength(k) / tempo - m);
										printf("| ");
									}
								}
							}
							if(!(getLength(k) / tempo < 2)){
								if(notePlace - getLength(k) / tempo < 21){
									gotoxy(40, notePlace - getLength(k) / tempo);
									printf("\x1b[92m");
									printf("��");
									printf("\x1b[39m");
									if(notePlace - getLength(k) / tempo - 1 > 0){
										gotoxy(40, notePlace - getLength(k) / tempo - 1);
										printf("| ");
									}
								}
							}
						break;
						
					case 71:
							if(notePlace < 21){
								printf("\x1b[95m");
								printf("��");
								printf("\x1b[39m");
							}
							for(m = 1; m < getLength(k) / tempo; m++){
								if(notePlace - m <= 0)
									break;
								if(notePlace - m < 21){
									gotoxy(40, notePlace - m);
									printf("\x1b[95m");
									printf("��");
									printf("\x1b[39m");
									if(notePlace - getLength(k) / tempo - m > 0){
										gotoxy(40, notePlace - getLength(k) / tempo - m);
										printf("| ");
									}
								}
							}
							if(!(getLength(k) / tempo < 2)){
								if(notePlace - getLength(k) / tempo < 21){
									gotoxy(40, notePlace - getLength(k) / tempo);
									printf("\x1b[95m");
									printf("��");
									printf("\x1b[39m");
									if(notePlace - getLength(k) / tempo - 1 > 0){
										gotoxy(40, notePlace - getLength(k) / tempo - 1);
										printf("| ");
									}
								}
							}
						break;
						
					case 73:
							if(notePlace < 21){
								printf("\x1b[95m");
								printf("��");
								printf("\x1b[39m");
							}
							for(m = 1; m < getLength(k) / tempo; m++){
								if(notePlace - m <= 0)
									break;
								if(notePlace - m < 21){
									gotoxy(40, notePlace - m);
									printf("\x1b[95m");
									printf("��");
									printf("\x1b[39m");
									if(notePlace - getLength(k) / tempo - m > 0){
										gotoxy(40, notePlace - getLength(k) / tempo - m);
										printf("| ");
									}
								}
							}
							if(!(getLength(k) / tempo < 2)){
								if(notePlace - getLength(k) / tempo < 21){
									gotoxy(40, notePlace - getLength(k) / tempo);
									printf("\x1b[95m");
									printf("��");
									printf("\x1b[39m");
									if(notePlace - getLength(k) / tempo - 1 > 0){
										gotoxy(40, notePlace - getLength(k) / tempo - 1);
										printf("| ");
									}
								}
							}
						break;
				}
				if(notePlace >= 2 && notePlace <= 21 && (getKind(k) != 3 && getKind(k) != 5 && getKind(k) != 6)){
					gotoxy(39, notePlace - 1);
					printf("    ");
					gotoxy(40, notePlace - 1);
					printf("| ");
				}
			}
			if(kbhit()){
				c = getch();
				switch(getKind(tapFlag)){
					
					case 1:
						if(c == 's' || c == 'k'){
							count++;
						}
						break;
					case 2:
						if(c == 's' || c == 'k'){
							if(kbhit()){
								c = getch();
								if(c == 's' || c == 'k')
								count++;
							}
						}
						break;
					case 3:
						if(longFlag){
							if(c == 's' || c == 'k'){
								count++;
							}
						}
						break;
					case 4:
								switch(getLength(k)){
									case 1:
										if(c == 'd' || c == 'l') count++;
										break;
									case 2:
										if(c == 'x' || c == ',') count++;
										break;
									case 3:
										if(c == 'a' || c == 'j') count++;
										break;
									case 4:
										if(c == 'w' || c == 'i') count++;
										break;
									case 5:
										if(c == 'e' || c == 'o') count++;
										break;
									case 6:
										if(c == 'c' || c == '.') count++;
										break;
									case 7:
										if(c == 'z' || c == 'm') count++;
										break;
									case 8:
										if(c == 'q' || c == 'u') count++;
										break;
									case 13:
										if(c == 'd' || c == 'l'){
											if(kbhit()){
												c = getch();
												if(c == 'a' || c == 'j'){
													count++;
												}
											}
										}else if(c == 'a' || c == 'j'){
											if(kbhit()){
												c = getch();
												if(c == 'd' || c == 'l'){
													count++;
												}
											}
										}
										break;
									case 34:
										if(c == 'a' || c == 'j'){
											if(kbhit()){
												c = getch();
												if(c == 'w' || c == 'i'){
													count++;
												}
											}
										}else if(c == 'w' || c == 'i'){
											if(kbhit()){
												c = getch();
												if(c == 'a' || c == 'j'){
													count++;
												}
											}
										}
										break;
									case 14:
										if(c == 'd' || c == 'l'){
											if(kbhit()){
												c = getch();
												if(c == 'w' || c == 'i'){
													count++;
												}
											}
										}else if(c == 'w' || c == 'i'){
											if(kbhit()){
												c = getch();
												if(c == 'd' || c == 'l'){
													count++;
												}
											}
										}
										break;
									case 24:
										if(c == 'w' || c == 'i'){
										if(kbhit()){
											c = getch();
											if(c == 'x' || c == ','){
													count++;
											}
										}
										}else if(c == 'x' || c == ','){
											if(kbhit()){
												c = getch();
												if(c == 'w' || c == 'i'){
													count++;
												}
											}
										}
										break;
									case 12:
										if(c == 'd' || c == 'l'){
										if(kbhit()){
											c = getch();
											if(c == 'x' || c == ','){
													count++;
											}
										}
										}else if(c == 'x' || c == ','){
											if(kbhit()){
												c = getch();
												if(c == 'd' || c == 'l'){
													count++;
												}
											}
										}
										break;
								
							}
					case 5:
						if(longFlag){
							if(c == 's' || c == 'k'){
								count++;
							}
						}
						break;
					case 6:
						if(longFlag){
							if(c == 's' || c == 'k' || c == 'a' || c == 'j' || c == 'd' || c == 'l'){
								count++;
							}
						}
						break;
					case 71:
						if(longFlag){
							if(c == 'd' || c == 'l'){
								count++;
							}
						}
						break;
					case 73:
						if(longFlag){
							if(c == 'a' || c == 'j'){
								count++;
							}
						}
						break;
				}
			}
		}
#endif		
		now = clock();
		gotoxy(39, 20);
		printf(" �� ");
		gotoxy(21, 2);
		printf("  ");
	}
	
}

int readNotes(char *fileName, int *bpm, int *notesMax){

	int musicLength = 0;
	int subTiming, subLength, subKind;
	int i;
	
	FILE *musicFile;
	
	musicFile = fopen(fileName, "r");
	
	fscanf(musicFile, "%d", notesMax);
	fscanf(musicFile, "%d", bpm);
	fscanf(musicFile, "%d", &musicLength);
	
	newNoteArrey(*notesMax);
	
	for(i = 0; i < *notesMax; i++){
		fscanf(musicFile, "%d %d %d", &subTiming, &subLength, &subKind);
		initializeNote(i, subTiming, subLength, subKind);
		initializeY(i);
	}
	
	fclose(musicFile);
	
	return musicLength;
	
}

