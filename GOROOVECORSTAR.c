//すべて標準関数です
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

//音声スレッド関数
DWORD WINAPI startMusic(LPVOID arg)
{
	//曲の再生
	PlaySound(arg, NULL, (SND_ASYNC|SND_FILENAME) );
	
	start = clock();
	
	return 0;
}

int main(void){
	
	//曲目を格納する変数。1000曲、1曲あたり50文字まで対応。
	char tunesList[1000][50];
	
	//読み込む曲数の最大値を格納する変数。
	int musicNumber;
	
	//現在の曲番号を格納する変数。
	int serectMusicNumber;
	
	//スレッド生成用の変数。
	HANDLE hThread;
	DWORD dwThreadId;
	
	//曲ファイルの名前を格納する変数。
	char* musicFileName;
	
	int musicLength;
	
	int notesMax;
	int bpm;
	int tempo;
	
	Sleep(3000);
	
	//画面の初期化。
	system("cls");
	
	//曲の最大数を格納。
	musicNumber = tunesRead(tunesList);
	
	//曲選択画面を表示し、選択された曲番号を格納。
	serectMusicNumber = songSelectionScreen(tunesList, musicNumber);
	
	//現在の曲名に.wav拡張子を付けます。(曲名.wavがその曲のファイル名です。)
	strcat(tunesList[serectMusicNumber], ".wav");
	
	musicFileName = streqlcpy(tunesList[serectMusicNumber], sizeof(tunesList[serectMusicNumber]) / sizeof(char));
	
	//スレッドの生成
	hThread = CreateThread(
		NULL, //セキュリティ属性
		0, //スタックサイズ
		startMusic, //スレッド関数
		musicFileName, //スレッド関数に渡す引数
		0, //作成オプション(0またはCREATE_SUSPENDED)
		&dwThreadId//スレッドID
	);
	
	//先ほど文字列に挿入した.wavを消します。
	strcln(tunesList[serectMusicNumber], ".wav");
	
	musicLength = readNotes(strcat(tunesList[serectMusicNumber], ".txt"), &notesMax, &bpm);
	
	tempo = 60 * 1000 / bpm;
	
	mainScreenOut(tempo, musicLength);
	
	//先ほど文字列に挿入した.txtを消します。
	strcln(tunesList[serectMusicNumber], ".txt");
	
	//スレッドの終了を待つ
	WaitForSingleObject(hThread, INFINITE);

	//スレッドのハンドルを閉じる
	CloseHandle(hThread);
	
	Sleep(1);
	
	return 0;
	
}

int tunesRead(char tunesList[1000][50]){
	
	//曲目リストを読み込むためのファイルポインタ変数。
	FILE *tunes;
	
	//ループ変数
	int i;
	
	/*同じディレクトリのtunes.txtに曲目リストが入っています。
	安全のため、読み込み専用で開いてください。*/
	tunes = fopen("tunes.txt", "r");
	
	for(i = 0; fscanf(tunes, "%s", tunesList[i]) != EOF; i++);
	
	//ファイルを閉じます。
	fclose(tunes);
	
	//曲の最大数を返します。
	return i;
	
}

int songSelectionScreen(char tunesList[1000][50], int musicNumber){
	
	//現在選択中の曲番号を格納する変数。
	int selectedSong = 0;
	
	//入力されたキーの値を格納する変数。
	char key = 0;
	
	//枠を表示。
	outFrame();
	//曲目を表示。
	outTunes(tunesList, selectedSong, musicNumber);
	
	//Enterキーが押されるまで。
	do{
		
		if(kbhit()){
			
			switch(key = getch()){
				
				/*wまたはiまたは↑が押されたらリストを1つ上に。
				内部の処理的には添え字を1つ少なく。
				*/
				case 0x48:
				case 119:
				case 105:
				  if(selectedSong > 0)
				  	  selectedSong--;
				  //選択中の曲が最初の曲なら、1つ上の曲は最後の曲となる。
				  else
				      selectedSong = musicNumber - 1;
				  break;
				
				
				/*xまたは,または↓が押されたらリストを1つ下に。
				内部の処理的には添え字を1つ多く。
				*/
				case 0x50:
				case 120:
				case 44:
				  if(selectedSong < musicNumber - 1)
				      selectedSong++;
				  
				  //選択中の曲が最後の曲なら、1つ下の曲は最初の曲となる。
				  else
				  	  selectedSong = 0;
			      break;
			}
			
			//曲目を表示。
			outTunes(tunesList, selectedSong, musicNumber);
		}
		
	}while(key != 13);
	
	//画面の初期化。
	system("cls");
	
	//現在の曲番号を返す。
	return selectedSong;
}

void outFrame(void){
	
	//ループ変数
	int i;
	
	/*3回枠を表示する。
	枠の太さが縦に3マスなので、i*3-2が枠の上端の座標となる。
	2つ目の枠は、現在選択中の曲なので、枠が太い。
	*/
	for(i = 1; i <= 3; i++){
		gotoxy(1, i * 3 - 2);
		if(i == 2)
			printf("Γ========================================￢\n");
			printf("Γ――――――――――――――――――――￢\n");
		gotoxy(1, i * 3 - 1);
		if(i == 2)
			printf("||                                        ||");
		else
			printf("|                                          |");
		gotoxy(1, i * 3);
		if(i == 2)
			printf("∟========================================」\n");
			printf("∟――――――――――――――――――――」\n");
	}
}

void outTunes(char tunesList[1000][50], int selectedSong, int musicNumber){
	
	//ループ変数
	int i;
	
	//現在の曲番号(処理上での)
	int systemSelectedSong;
	
	/*3つの曲を枠の中に表示。
	selectedSongが真ん中の曲なので、i=-1からのスタート。
	*/
	for(i = -1; i < 2; i++){
		
		/*表示する曲の曲番号を格納。
		表示する曲番号が0～最大値ならそのまま。
		0以下なら曲番号の最大値から。
		最大値以上なら最初から。
		*/
		systemSelectedSong = selectedSong + i;
		systemSelectedSong = systemSelectedSong < 0 ? musicNumber + i : (systemSelectedSong > musicNumber - 1 ? i - 1 : systemSelectedSong);
		
		/*枠の太さが縦に3マスなので、i*3+2が枠の中心の座標となる。
		iが負だと計算しづらいので、先にi+1する。
		ちなみにx座標は枠が2マスあるので、3からの表示。
		*/
		gotoxy(3, (i + 1) * 3 + 2);
		
		//40字以内で曲名を表示。
		printf("%-40s", tunesList[systemSelectedSong]);
	}
}

/*文字列s1からある文字列s2を削除する関数。
すみませんネットからのコピペです。
*/

void strcln(char *s1, char *s2){

	char *p;
	p = strstr(s1, s2);
	if (p != NULL) {
		strcpy(p, p + strlen(s2));
		strcln(p + 1, s2);
	}
}

/*文字列のコピー用の関数。
コピーされた配列を返す。
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
			printf("●");
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
						printf("○");
						break;
					case 2:
						
						printf("\x1b[96m");
						printf("◎");
						printf("\x1b[39m");
						break;
					case 3:
							if(notePlace < 21){
								printf("\x1b[93m");
								printf("〇");
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
									printf("〇");
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
							printf("→");
						}else if(getLength(k) == 2){
							printf("↓");
						}else if(getLength(k) == 3){
							printf("←");
						}else if(getLength(k) == 4){
							printf("↑");
						}else if(getLength(k) == 13){
							gotoxy(39, notePlace);
							printf("←→");
						}else if(getLength(k) == 34){
							gotoxy(38, notePlace);
							printf("←↑");
						}else if(getLength(k) == 14){
							printf("↑→");
						}else if(getLength(k) == 23){
							gotoxy(38, notePlace);
							printf("←↓");
						}else if(getLength(k) == 24){
							gotoxy(39, notePlace);
							printf("↑↓");
						}else if(getLength(k) == 12){
							gotoxy(38, notePlace);
							printf("←↓");
						}else if(getLength(k) == 5){
							printf("／");
						}else if(getLength(k) == 6){
							printf("＼");
						}else if(getLength(k) == 7){
							printf("／");
						}else if(getLength(k) == 8){
							printf("＼");
						}
						printf("\x1b[39m");
						break;
					case 5:
							if(notePlace < 21){
								printf("\x1b[38;5;166m");
								printf("◇");
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
									printf("◇");
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
								printf("◎");
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
									printf("◎");
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
								printf("→");
								printf("\x1b[39m");
							}
							for(m = 1; m < getLength(k) / tempo; m++){
								if(notePlace - m <= 0)
									break;
								if(notePlace - m < 21){
									gotoxy(40, notePlace - m);
									printf("\x1b[95m");
									printf("□");
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
									printf("→");
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
								printf("←");
								printf("\x1b[39m");
							}
							for(m = 1; m < getLength(k) / tempo; m++){
								if(notePlace - m <= 0)
									break;
								if(notePlace - m < 21){
									gotoxy(40, notePlace - m);
									printf("\x1b[95m");
									printf("□");
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
									printf("←");
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
		printf(" ● ");
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

