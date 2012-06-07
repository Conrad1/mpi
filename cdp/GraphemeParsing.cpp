#include "stdafx.h"
#include "GraphemeParsing.h"
#include "ZorziNet.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




void GraphemeParsing::CreateOrths()
{
	//CreateOrths111
	FILE *fp, *fp2, *fp3;
	char FileWord[50], FilePhon[50], newWord[50],crapphon[50], crap1[50],crap2[50], crap3[50],tempFilePhon[50];
	int count = 0,i, j, ok1, ok2, ok3, tempsylls, tempstress,wordnum,thisnosylls = 0;
	float thisfreq, thisfreq2;

	int beforetempvowel = 0;
	fp2 = fopen("Here1.txt","a"); fprintf(fp2,"1\n"); fclose(fp2);

	if(ENGLISH == 1) fp = fopen("./EnglishRes/EveryThing.txt","r");
	else if(ITALIAN ==1) fp = fopen("./ItalianRes/ItalianWords2.HF.txt","r");
	else if(GERMAN == 1) fp = fopen("./GermanRes/AllGermanDB.txt","r");
	else if(FRENCH == 1) fp = fopen("./FrenchRes/AllFrenchDB.txt","r");

	if(fp == 0) exit(1);

#if ENGLISH
	while((fscanf(fp,"%s%s%d%f%d",FileWord, FilePhon, &tempstress, &thisfreq, &thisnosylls)) != EOF) {	
#elif ITALIAN
	while((fscanf(fp,"%s%s%d%d%f",FileWord, FilePhon, &thisnosylls, &tempstress, &thisfreq)) != EOF) {	
#elif GERMAN
	//while((fscanf(fp,"%s%s%d%f%f%s",FileWord, FilePhon, &tempstress, &thisfreq, &thisfreq2,crapphon)) != EOF) {		
	while((fscanf(fp,"%s%s%d%f%f",FileWord, FilePhon, &thisnosylls, &tempstress, &thisfreq)) != EOF) {		
#elif FRENCH
	while((fscanf(fp,"%s%s%d%d%f",FileWord, FilePhon, &thisnosylls, &tempstress, &thisfreq)) != EOF) {	
#endif
		
		if(thisfreq < 0) thisfreq = 0;
	//	fp2 = fopen("trryyyy.txt","a");
	//	fprintf(fp2,"%s %s %d %d %f \n",FileWord, FilePhon, thisnosylls, tempstress, thisfreq);
	//	fclose(fp2);

		ToTrain = 1;
		strcpy(GlobalWord,FileWord);
	
		if(ENGLISH == 1) for(i=0;i<strlen(FileWord);i++) newWord[i] = tolower(FileWord[i]);
		else strcpy(newWord,FileWord);
		if(GERMAN == 1) {
			//if(thisfreq == 0) thisfreq++;
		}
		i = strlen(FileWord);
		newWord[i] = 0;		
		strcpy(FileWord,newWord);
		strcpy(GlobalPhon,FilePhon);

		for(i=0;i<6;i++) {
			ntempcodas[i] = 0; ntempvowels = 0;
			ntemponsets[i] = 0 ;
			ntempcodas2[i] = 0; 
			ntemponsets2[i] =0; 
		}
		count++;
		
		if(FRENCH == 1) { /***************************WARNIGN DELETES THE SECOND*****/
			for(i=0;i<strlen(FilePhon);i++) {
				if(FilePhon[i] == ';') {
					FilePhon[i] = 0;
					break;
				}
			}
			if(FilePhon[0] == 'h') {
				strcpy(tempFilePhon,FilePhon);
				for(i=1;i<strlen(tempFilePhon);i++) {
					FilePhon[i-1] = tempFilePhon[i];
				}
				FilePhon[i-1] = 0;
			}
		}

		if(thisnosylls <= 3) {
			MakePhon(FilePhon);
			ok1 = CheckPhons(PhonString);
			ok2 = CheckOrths(FileWord);
		}
		if(ITALIAN == 1) {
			ok3 = CheckPhonsItalian(FilePhon);
			if(ok3 == -1) thisnosylls = 50;
			fp2 = fopen("shitty.txt","a");
			fprintf(fp2,"%d [%s] [%s] %d %d \n",ok3, FileWord, FilePhon, ok1, ok2);
			fclose(fp2);
		}
		if( ok1 == -1 || ok2 == -1 || thisnosylls > 3) {
			count--;
			fp2 = fopen("shitty.txt","a");
			fprintf(fp2,"[%s] [%s] %d %d \n",FileWord, FilePhon, ok1, ok2);
			fclose(fp2);
		} else {

			difvowels = 0;
			MakePhonStr(PhonString);  /*okay with '*' character in French? */
			//fp2=fopen("here.txt","a"); fprintf(fp2,"1 %s %d \n", GlobalWord, nphonvowels);fclose(fp2);
		
			//PrintWordName("Trained.1.txt");

			GlobalNPhonSylls = nphonvowels;	
			SegmentWord(FileWord); //turn it into consonants and graphemes
			//fp2=fopen("here222.txt","a"); fprintf(fp2,"2 %s\n", GlobalWord);fclose(fp2);

			ParseSegments(); //parse the consonants and graphemes
			//fp2=fopen("here3333.txt","a"); fprintf(fp2,"3 %s\n", GlobalWord);fclose(fp2);

			CreateOrthString();
			beforetempvowel = ntempvowels;
		//	if(nphonvowels == 2 && (ntempcodas[0] + ntemponsets[1] == nphoncodas[0]+nphononsets[1]) ) AddGraphemePhonemeInf();  	
	
			//fp2=fopen("here4444.txt","a"); fprintf(fp2,"3.1 %s\n", GlobalWord);fclose(fp2);

			if(ENGLISH == 1) AlignInputs();	
			else if (ITALIAN ==1) AlignInputsItalian();
			else if (GERMAN == 1) AlignInputsGerman();
			else if (FRENCH == 1) AlignInputsFrench();

		//	fp2=fopen("here555.txt","a"); fprintf(fp2,"4 %s\n", GlobalWord);fclose(fp2);

			//if(ntempvowels < 3) PrintWordName("BeforeFix.txt");	

			if(ENGLISH == 1) {
				FixExceptions();
			} else if (ITALIAN == 1) {
				FixExceptionsItalian();
			} else if (GERMAN ==1) {
				//do nothing
			} else if (FRENCH == 1) {
				FixExceptionsFrench();
			}
		//	fp2=fopen("here.txt","a"); fprintf(fp2,"%d 5 %s\n",count, GlobalWord);fclose(fp2);
			//PrintWord();
			//RecreateNewWord();
			//PrintWord();

			// PrintWordName("InterVocsEs.txt");
			if(nphonvowels == 2) {
				if( (strcmp(tempcodas[0][ntempcodas[0]-1],"e")) == 0 && strlen(tempvowels[0]) == 1 ) {
					//PrintWordName("TVs2.txt");
					//fp2 = fopen("Tvs2.phon.txt","a");
					//fprintf(fp2,"%c %s \n",PhonString[3], PhonString);
					//fclose(fp2);
				}
			}
			if(nphonvowels == 3) {
				if((strcmp(tempvowels[1],"e")) == 0 && strlen(tempvowels[0]) == 1) {
					//PrintWordName("TVs3.txt");
					/*fp2 = fopen("Tvs3.phon.txt","a");
					fprintf(fp2,"%c %s \n",PhonString[3],PhonString);
					fclose(fp2);*/
				}
			}


			//if(ToTrain == 1 || difvowels == 1) {
				fp2 = fopen("ResAll.txt","a");
			//	fprintf(fp2,"4 %d %d %s %s %s %s %s %s %d %d %d %d %10.0f\n",ok1, ok2, FileWord, AlignedInputs, OrthSegWord, OrthString, PhonString, FilePhon, ntempvowels,long double ,tempsylls, tempstress, thisfreq);
				fprintf(fp2," %s %s %s %s %s %s %d %d %1.0f \n",FileWord, AlignedInputs, OrthSegWord, OrthString, PhonString, FilePhon, ntempvowels, tempstress, thisfreq);
				fclose(fp2);
			//}

			if(ToTrain == 1) {
				//PrintWordName("Trained.1.txt");
				RecreateNewWord();
				fp2 = fopen("Res.txt","a");
			//	fprintf(fp2,"4 %d %d %s %s %s %s %s %s %d %d %d %d %10.0f\n",ok1, ok2, FileWord, AlignedInputs, OrthSegWord, OrthString, PhonString, FilePhon, ntempvowels,long double ,tempsylls, tempstress, thisfreq);
				fprintf(fp2," %s %s %s %s %s %s %d %d %1.0f \n",FileWord, AlignedInputs, OrthSegWord, OrthString, PhonString, FilePhon, ntempvowels, tempstress, thisfreq);
				fclose(fp2);

				if(nphonvowels == 2) {
				/*	j = 0;
					for(i=0;i<ntempcodas[0];i++) j+= strlen(tempcodas[0][i]);
					for(i=0;i<ntemponsets[1];i++) j+= strlen(temponsets[1][i]);

					fp2=fopen("duals.txt","a");
					fprintf(fp2,"%s %d %d \n",FileWord,j, ntempcodas[0]+ntemponsets[1]);
					fclose(fp2);*/
				}
			
				if(ENGLISH ==1) CreateGraphemePattern( log(thisfreq + 2)/log(1093547.0 + 2) );	
				else if (ITALIAN == 1) CreateGraphemePattern( log(thisfreq + 2)/log(606000000.0 + 2) );
				else if (GERMAN == 1) {
					if(thisnosylls < 4)	CreateGraphemePattern( log(thisfreq + 2)/log(150507.0 + 2) );
				} else if (FRENCH == 1) {
					if(thisnosylls < 4)	CreateGraphemePattern( log(thisfreq + 2.0)/log(867040.78 + 2.0) );
				}
				AddGraphemeDB();
			} else {
				fp2 = fopen("NotTrained.txt","a");
				fprintf(fp2,"%s %s %s %s %s %s %d %d %1.0f \n",FileWord, AlignedInputs, OrthSegWord, OrthString, PhonString, FilePhon, ntempvowels, tempstress, thisfreq);		
				fclose(fp2);
			}
		}
		///if(count == 100) break;
	}

	PrintGraphemeDB();
	//Train the network
	/*for(i=0;i<20;i++) {
		fp3 = fopen("test5.txt","a");
		fprintf(fp3,"%d \n",i);
		fclose(fp3);

		TheTrainingCycle = i;
		TrainSegmentations();
		SaveSegmentations();
		TestAllSegNetwork();
	}*/
	//difficulties sc/sch 

	/*fp=fopen("graphinfo.txt","w");
	for(i=0;i<10;i++) {
		fprintf(fp,"%d %d  \n",i, nGraphemesUpTo[i]);
		for(j=0;j<nGraphemesUpTo[i];j++) {
			fprintf(fp," %d %s %f %d %c \n",GraphemesInf[i][j], GraphemesInflets[i][j], GraphemeFreqsInf[i][j], PhonemesInf[i][j], PhonemesCInf[i][j]); 
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
*/
	fclose(fp);

}

void GraphemeParsing::PrintGraphemeDB()
{
	FILE *fp;
	int i,j,k;

	fp = fopen("DBRes.txt","w");

	for(i=0;i<NMARCOLETTERS2;i++) {
		fprintf(fp,"O %s %d %d %d\n",GraphemesList[i],OnsetCounts[i],VowelCounts[i],CodaCounts[i]);
	}
	fclose(fp);
}

void GraphemeParsing::AddGraphemeDB()
{
	int i,j,k;

	for(i=0;i<ntempvowels;i++) {
		for(j=0;j<ntemponsets[j];j++) {
			OnsetCounts[temponsetsnum[i][j]]++;
		}
		VowelCounts[tempvowelsnum[i]]++;
		for(j=0;j<ntempcodas[j];j++) {
			CodaCounts[tempcodasnum[i][j]]++;
		}
	}
}



void GraphemeParsing::CreateGraphemePattern(float freqmod)
{
	int i,j,k,num,spot;
	int ngraphemes;
	char graphemearray[40][20];
	int graphemenumbers[40];
	char letterstring[30];
	int insyll[40];
	int isCoda[40];
	FILE *fp,*fp1;


	PrintWord();

	ngraphemes = 0;
	
	for(i=0;i<ntempvowels;i++) {
		for(j=0;j<ntemponsets[i];j++) {
			strcpy(graphemearray[ngraphemes],temponsets[i][j]);		
			if(FRENCH==1 && ( (IsVowel(temponsets[i][j][0]) != -1 && strlen(temponsets[i][j]) == 1 )|| IsVowelGrapheme(temponsets[i][j])!= -1)) { 
					graphemenumbers[ngraphemes] = temponsetsnum[i][j] + NMARCOLETTERS; // move single letter vowels out.
					insyll[ngraphemes] = i;
					isCoda[ngraphemes] = 0;
					ngraphemes++;				
					//>NMARCOLETTERS-NLETTERS
					//place onset vowels in next set of leters.
			} else {
				graphemenumbers[ngraphemes] = temponsetsnum[i][j] + NMARCOLETTERS; // use different lets 					
				insyll[ngraphemes] = i;
				isCoda[ngraphemes] = 0;
				ngraphemes++;
			}
		}
		if(tempvowelsnum[i] != -1) {
			strcpy(graphemearray[ngraphemes], tempvowels[i]);
			graphemenumbers[ngraphemes] = tempvowelsnum[i];	
			insyll[ngraphemes] = i;
			isCoda[ngraphemes] = 0;
			ngraphemes++;
		}
		for(j=0;j<ntempcodas[i];j++) {
			strcpy(graphemearray[ngraphemes],tempcodas[i][j]);
			graphemenumbers[ngraphemes] = tempcodasnum[i][j];		
			if( ENGLISH ==1 && (strcmp(tempcodas[i][j],"e")) == 0) {
				graphemenumbers[ngraphemes] += NMARCOLETTERS;
			}
			if (FRENCH ==1 ) {
				if( (IsVowel(tempcodas[i][j][0]) != -1 && strlen(tempcodas[i][j]) == 1 )|| IsVowelGrapheme(tempcodas[i][j])!= -1) { 
					graphemenumbers[ngraphemes] += /*NLETTERS*/NMARCOLETTERS*2;
				}
			}
			insyll[ngraphemes] = i;
			isCoda[ngraphemes] = ntempcodas[i];
			ngraphemes++;
		}
	}
	if(ntemponsets[i] != 0){ //vowelless
		for(j=0;j<ntemponsets[i];j++) {
			strcpy(graphemearray[ngraphemes],temponsets[i][j]);
			graphemenumbers[ngraphemes] = temponsetsnum[i][j] + NMARCOLETTERS;
			ngraphemes++;
		}
	}

	//PrintWord();
	//Setup Memory
	//+-z-o-b-o-l-o
	/*fp = fopen("testststs.txt","a");
	fprintf(fp,"%d %s ", ngraphemes, GlobalWord);
	for(i=0;i<ngraphemes;i++) {
		fprintf(fp, "%s %d \n",graphemearray[i],graphemenumbers[i]);
	}
	fprintf(fp,"\n");
	fclose(fp);*/

	for(i=0;i<ngraphemes;i++) {
		for(j=1;j<4;j++) {
			if(i-j >= 0) {
				SplitNetMemory[graphemenumbers[i-j] + ((j-1) * (NMARCOLETTERS2))] = 1; 
			}
		}
		///INITIAL SPACE
		if(i == 0) {
			SplitNetMemory[(NMARCOLETTERS2) - 1] = 1;
			SplitNetMemory[(NMARCOLETTERS2 * 2) - 1] = 0;
			SplitNetMemory[(NMARCOLETTERS2 * 3) - 1] = 0;	
		} else if(i == 1) {
			SplitNetMemory[(NMARCOLETTERS2) - 1] = 0;
			SplitNetMemory[(NMARCOLETTERS2 * 2) - 1] = 1;
		} else if(i == 2) {
			SplitNetMemory[(NMARCOLETTERS2 * 2) - 1] = 0;
			SplitNetMemory[(NMARCOLETTERS2 * 3) - 1] = 1;
		} else if(i == 3) {
			SplitNetMemory[(NMARCOLETTERS2 * 3) - 1] = 0;
		} else {

		}

		//add letters to attention window.
		letterstring[0] = 0;
		//for(j=0;j<3;j++) {
		for(j=0;j<ATTENTIONWINDOWSIZE;j++) {
			if( (i + j) < ngraphemes) {
				strcat(letterstring,graphemearray[i + j]);
			}
		}
		for(j=strlen(letterstring);j<ATTENTIONWINDOWSIZE;j++) {
			letterstring[j] = '+';
		}
		letterstring[ATTENTIONWINDOWSIZE] = 0;

		//Letters.
		for(j=0;j<ATTENTIONWINDOWSIZE;j++) {
			SplitNetMemory[((NMARCOLETTERS2) * SPLITMEMORY) + (NLETTERS * j + FindLetter(letterstring[j]))] = 1;
		/*	fp = fopen("test2222.txt","a");
			fprintf(fp,"%d %d \n",((NMARCOLETTERS2) * SPLITMEMORY) + (NLETTERS * j + FindLetter(letterstring[j])),(NMARCOLETTERS2 * 3) - 1);
			fclose(fp);*/
		}
		
		/* num = findOnsetnum(graphemearray[i]);
		if(num == -1) {
			num = findVowelnum(graphemearray[i]);
			if(num != -1) num += NOnsets;
		}
		if(num == -1) {
			num = findCodanum(graphemearray[i]);
			if(num != -1) num += NOnsets + NVowels;	
			else {
				num = FindLetter(graphemearray[i][0]) + NGRAPHEMES;
			}
		}*/
		num = graphemenumbers[i];

	//	PrintWord();

		NSEGMENTATIONS++;
		if(ENGLISH == 1) fp = fopen("./EnglishRes/Segpatterns.txt","a");
		else if (ITALIAN ==1) fp = fopen("./ItalianRes/Segpatterns.Italian.txt","a");
		else if (GERMAN ==1) fp = fopen("./GermanRes/Segpatterns.German.txt","a");
		else if (FRENCH == 1) fp = fopen("./FrenchRes/Segpatterns.French.txt","a");
		//fprintf(fp,"%s %s %d %d %s %d ",GlobalWord,letterstring,((NMARCOLETTERS * 3) * 3) + (NLETTERS * 3),strlen(graphemearray[i]) - 1, graphemearray[i],num);
		//fprintf(fp,"%s %s %f %d %d %s %d %d %d ",GlobalWord,letterstring,freqmod,((NMARCOLETTERS2 * 3)) + (NLETTERS * ATTENTIONWINDOWSIZE),strlen(graphemearray[i]) - 1, graphemearray[i],num, ntempcodas[i], insyll[i]);
	//	fprintf(fp,"%s %s %f %d %d %s %d ",GlobalWord,letterstring,freqmod,((NMARCOLETTERS2 * 3)) + (NLETTERS * ATTENTIONWINDOWSIZE),strlen(graphemearray[i]) - 1, graphemearray[i],num);
		fprintf(fp,"%s %s %f %d %d %s %d ",GlobalWord,letterstring,freqmod,insyll[i],strlen(graphemearray[i]) - 1, graphemearray[i],num);

//		for(k=0;k<((NMARCOLETTERS * 3) * 3) + (NLETTERS * 3);k++) {
	
		spot = 0;
		for(j=0;j<3;j++) {
			for(k=0;k<NMARCOLETTERS2;k++) {
				fprintf(fp,"%d",SplitNetMemory[spot++]);
			}
			//fprintf(fp,"\n");
		}
		for(j=0;j<NLETTERS*ATTENTIONWINDOWSIZE;j++) {
			fprintf(fp,"%d",SplitNetMemory[spot++]);
		}
		fprintf(fp,"\n");
		fclose(fp);

		//for(k=0;k<(NMARCOLETTERS2 * 3) + (NLETTERS * ATTENTIONWINDOWSIZE);k++) {
		//	fprintf(fp, "%d",SplitNetMemory[k]);
		//}

		/*fprintf(fp,"\n");
		fclose(fp);*/

		//Reset mem
		for(j=1;j<4;j++) {
			if(i-j >= 0) {
				SplitNetMemory[graphemenumbers[i-j] + ((j-1) * (NMARCOLETTERS2)) ] = 0;
			}
		}
		for(j=0;j<ATTENTIONWINDOWSIZE;j++) {
			SplitNetMemory[((NMARCOLETTERS2) * SPLITMEMORY) + (NLETTERS * j + FindLetter(letterstring[j]))] = 0;
		}
	}
}




void GraphemeParsing::AlignInputsGerman() 
{
#if GERMAN
	int i,j,k,l,a,notchanged = 0;
	char NewWord[100];
	FILE *fp, *fp1;

	NewWord[0] = 0;
	
	FixEarlyExceptionsGerman();

	if(nphonvowels != ntempvowels) {
		fp = fopen("difvowels.txt","a");
		fprintf(fp, "%s  %d %d \n",GlobalWord,ntempvowels,nphonvowels);
		fclose(fp);
		strcpy(AlignedInputs,"XX");
		ToTrain = 0;
		difvowels = 1;
		return;
	} 
	ToTrain = 1;

	for(i=0;i<ntempvowels;i++) {
	
			//		fp1 = fopen("Split.eh.txt","a");
			//		fprintf(fp1,"SV %s %s %d %d %d %d\n",GlobalWord, tempvowels[i], nphononsets[i+1], ntempvowels,ntempcodas[i],ntemponsets[i+1]);
			//		fclose(fp1);
	
	/*for(j=0;j<ntempcodas[i];j++) {
			if((IsVowel(tempcodas[i][j][0])) != -1) {
				fp = fopen("difvowels.errorE. txt","a");
				fprintf(fp, "%s %d %d \n",GlobalWord,ntempvowels,nphonvowels);
				fclose(fp);
			}
		}*/
		strcat(NewWord,tempvowels[i]);
		strcat(NewWord,"+");
		if(i==/*MAXSYLLABLES*/ (ntempvowels-1)) { //FINAL SYLLABLE
			for(j=0;j<ntempcodas[i];j++) {
				strcat(NewWord,tempcodas[i][j]);
				strcat(NewWord,".");
			}
			strcat(NewWord,"+");
		} else {
			//	fp1 = fopen("Split.eh.txt","a");
			//	fprintf(fp1,"SV %s %s %d %d %d %d\n",GlobalWord, tempvowels[i], nphononsets[i+1], ntempvowels,ntempcodas[i],ntemponsets[i+1]);
			//		fclose(fp1);

			if(     ((strcmp(tempvowels[i],"eh")) == 0 && nphononsets[i+1] == 1 && ntemponsets[i+1] == 0 && ntempcodas[i] == 0) || \
					((strcmp(tempvowels[i],"Eh")) == 0 && nphononsets[i+1] == 1 && ntemponsets[i+1] == 0 && ntempcodas[i] == 0) ||\
					((strcmp(tempvowels[i],"ah")) == 0 && nphononsets[i+1] == 1 && ntemponsets[i+1] == 0 && ntempcodas[i] == 0) ||\
					((strcmp(tempvowels[i],"Ah")) == 0 && nphononsets[i+1] == 1 && ntemponsets[i+1] == 0 && ntempcodas[i] == 0) ||\
					((strcmp(tempvowels[i],"oh")) == 0 && nphononsets[i+1] == 1 && ntemponsets[i+1] == 0 && ntempcodas[i] == 0) ||\
					((strcmp(tempvowels[i],"Oh")) == 0 && nphononsets[i+1] == 1 && ntemponsets[i+1] == 0 && ntempcodas[i] == 0) ||\
					((strcmp(tempvowels[i],"uh")) == 0 && nphononsets[i+1] == 1 && ntemponsets[i+1] == 0 && ntempcodas[i] == 0) ||\
					((strcmp(tempvowels[i],"Uh")) == 0 && nphononsets[i+1] == 1 && ntemponsets[i+1] == 0 && ntempcodas[i] == 0) ||\
					((strcmp(tempvowels[i],"ih")) == 0 && nphononsets[i+1] == 1 && ntemponsets[i+1] == 0 && ntempcodas[i] == 0) ||\
					((strcmp(tempvowels[i],"auh")) == 0 && nphononsets[i+1] == 1 && ntemponsets[i+1] == 0 && ntempcodas[i] == 0) ||\
					((strcmp(tempvowels[i],"ieh")) == 0 && nphononsets[i+1] == 1 && ntemponsets[i+1] == 0 && ntempcodas[i] == 0) ||\
					((strcmp(tempvowels[i],"eih")) == 0 && nphononsets[i+1] == 1 && ntemponsets[i+1] == 0 && ntempcodas[i] == 0) 	) {
			
					j = strlen(tempvowels[i]);
					tempvowels[i][j-1] = 0;
					if(strlen(tempvowels[i]) == 1) tempvowelsnum[i] =  NGRAPHEMES + FindLetter(tempvowels[i][0]); 
					else tempvowelsnum[i] =  findGrapheme(tempvowels[i]); 
					strcpy(temponsets[i+1][0],"h");
					temponsetsnum[i+1][0] =  NGRAPHEMES + FindLetter('h');
					ntemponsets[i+1] = 1;
					ntempcodas[i] = 0;
					
					fp1 = fopen("Split.eh.txt","a");
					fprintf(fp1,"SV %s %d %d %d\n",GlobalWord, ntempvowels,ntempcodas[i],ntemponsets[i+1]);
					fclose(fp1);
			} else if( (ntempcodas[i] + ntemponsets[i+1]) == (nphoncodas[i] + nphononsets[i+1]) ) { //same
				for(j=0;j<nphoncodas[i];j++) {
					strcat(NewWord,tempcodas[i][j]);
					strcat(NewWord,".");
				}
				ntempcodas[i] = nphoncodas[i];
				strcat(NewWord,"+");
				for(j=0;j<nphononsets[i+1];j++) {
					strcat(NewWord,tempcodas[i][j+nphoncodas[i]]);
					strcat(NewWord,".");
					strcpy(temponsets[i+1][j],tempcodas[i][j+nphoncodas[i]]);
					temponsetsnum[i+1][j] = tempcodasnum[i][j+nphoncodas[i]];
				}
				ntemponsets[i+1] = nphononsets[i+1];				
				strcat(NewWord,"+");

				if(nphononsets[i+1] == 0 && nphoncodas[i] != 0 && i < ntempvowels-1) {
					fp1 = fopen("Split.NoOnsetsbutCoda.txt","a");
					fprintf(fp1,"SV %s %d %s %s\n",GlobalWord, ntempcodas[i],tempvowels[i],tempcodas[i][ntempcodas[i]-1]);
					fclose(fp1);		
				}
			} else if( (ntempcodas[i] + ntemponsets[i+1] + 1) == (nphoncodas[i] + nphononsets[i+1]) && ntempcodas[i] > 0) { //same
				if( (strcmp(tempcodas[i][0],"bb")) == 0 || \
					(strcmp(tempcodas[i][0],"pp")) == 0 ||\
					(strcmp(tempcodas[i][0],"zz")) == 0 ||\
					(strcmp(tempcodas[i][0],"nn")) == 0 ||\
					(strcmp(tempcodas[i][0],"tt")) == 0 ||\
					(strcmp(tempcodas[i][0],"rr")) == 0 ||\
					(strcmp(tempcodas[i][0],"ll")) == 0 ||\
					(strcmp(tempcodas[i][0],"ff")) == 0 ||\
					(strcmp(tempcodas[i][0],"mm")) == 0 ||\
					(strcmp(tempcodas[i][0],"ss")) == 0 ||\
					(strcmp(tempcodas[i][0],"dd")) == 0 ||\
					(strcmp(tempcodas[i][0],"sh")) == 0 ||\
					(strcmp(tempcodas[i][0],"ds")) == 0 ||\
					(strcmp(tempcodas[i][0],"gg")) == 0 ||\
					(strcmp(tempcodas[i][0],"tz")) == 0 ||\
					(strcmp(tempcodas[i][0],"tts")) == 0 ||\
					(strcmp(tempcodas[i][0],"sch")) == 0 ||	\
					(strcmp(tempcodas[i][0],"tsch")) == 0 ||\
					(strcmp(tempcodas[i][0],"th")) == 0 ||	\
					(strcmp(tempcodas[i][0],"ts")) == 0 ||	\
					(strcmp(tempcodas[i][0],"pf")) == 0 ||	\
					(strcmp(tempcodas[i][0],"ng")) == 0 ) {
					
								
					if(strlen(tempcodas[i][0]) == 2) {		
						temponsets[i+1][0][0] = tempcodas[i][0][1];
						temponsets[i+1][0][1] = 0;
						temponsetsnum[i+1][0] = NGRAPHEMES + FindLetter(temponsets[i+1][0][0]); 

						tempcodas[i][0][0] = tempcodas[i][0][0];
						tempcodas[i][0][1] = 0;
						tempcodasnum[i][0] = NGRAPHEMES + FindLetter(tempcodas[i][0][0]); 
					} else {
						j = strlen(tempcodas[i][0]);
						for(k=1;k<j;k++) {
							temponsets[i+1][0][k-1] = tempcodas[i][0][k];
						}
						temponsets[i+1][0][k-1] = 0;
						temponsetsnum[i+1][0] = findGrapheme(temponsets[i+1][0]); 

						tempcodas[i][0][1] = 0;
						temponsetsnum[i+1][0] = NGRAPHEMES + FindLetter(temponsets[i+1][0][0]); 
					}


					for(j=1;j<ntempcodas[i];j++) {
						strcpy(temponsets[i+1][j], tempcodas[i][j]);
						temponsetsnum[i+1][j] = tempcodasnum[i][j];
					}
					ntemponsets[i+1] = ntempcodas[i];
					ntempcodas[i] = 1;

					//PrintWord();
					fp1 = fopen("Split.bb.txt","a");
					fprintf(fp1,"SV %s %s %d %d %d\n",GlobalWord, temponsets[i+1][0], ntempvowels,ntempcodas[0],ntempcodas[1]);
					fclose(fp1);
				} else if( (strcmp(tempcodas[i][0], "x")) == 0) { 
					if(ntempcodas[i] > 1) {
						if( (strcmp(tempcodas[i+1][1],"th")) == 0) {
							strcpy(tempcodas[i+1][1],"t");
							strcpy(tempcodas[i+1][2],"h");
							tempcodasnum[i+1][1] = NGRAPHEMES + FindLetter('t');
							tempcodasnum[i+1][2] = NGRAPHEMES + FindLetter('h');
							ntempcodas[i+1]++;
						}

						ntemponsets[i+1] = 0;					
						if(nphoncodas[i] > 0) {
							for(j=nphoncodas[i] - 1;j<ntempcodas[i];j++) {
								strcpy(temponsets[i+1][j-(nphoncodas[i]-1)],tempcodas[i][j]);
								temponsetsnum[i+1][j-(nphoncodas[i]-1)] = tempcodasnum[i][j];
								ntemponsets[i+1]++;
							}
							ntempcodas[i] = nphoncodas[i] - 1;
						} else {
							for(j=0;j<ntempcodas[i];j++) {
								strcpy(temponsets[i+1][j],tempcodas[i][j]);
								temponsetsnum[i+1][j] = tempcodasnum[i][j];
							}
							ntemponsets[i+1] = ntempcodas[i];
							ntempcodas[i] = 0;
						}
					} else {
						//just one item. fix this later.
						for(j=0;j<ntempcodas[i];j++) {
							strcpy(temponsets[i+1][j],tempcodas[i][j]);
							temponsetsnum[i+1][j] = tempcodasnum[i][j];
						}
						ntemponsets[i+1] = ntempcodas[i];
						ntempcodas[i] = 0;
					}
					fp1 = fopen("Split.x.txt","a");
					fprintf(fp1,"SV %s %d %d %d\n",GlobalWord, ntempvowels,ntempcodas[0],ntempcodas[1]);
					fclose(fp1);
				}  else if ( (strcmp(tempcodas[i][0],"qu")) == 0 || (strcmp(tempcodas[i][1],"qu")) == 0 || (strcmp(tempcodas[i][2],"qu")) == 0) { 
					strcpy(temponsets[i+1][0],"qu");
					ntemponsets[i+1] = 1;
					temponsetsnum[i+1][0] = findGrapheme("qu");
					ntempcodas[i]--;
					fp1 = fopen("qu.txt","a");
					fprintf(fp1,"SV %s %d %d %d ton=%d tcn=%d %s %s\n",GlobalWord, ntempvowels,ntempcodas[0],ntempcodas[1],temponsetsnum[i+1][0],tempcodasnum[i][ntempcodas[i]],GraphemesList[3], temponsets[i+1][0]);
					fclose(fp1);
					
				} else if( (strcmp(tempcodas[i][ntempcodas[i]-1],"tch")) == 0 && phononsets[i+1][0] == 'x') {
					tempcodas[i][ntempcodas[i]-1][1] = 0;
					tempcodasnum[i][ntempcodas[i]-1] = NGRAPHEMES  + FindLetter('t');
					strcpy(temponsets[i+1][0],"ch");
					temponsetsnum[i+1][0] = findGrapheme(temponsets[i+1][0]);
					ntemponsets[i+1] = 1;
					fp1 = fopen("tch.txt","a");
					fprintf(fp1,"SV %s %d %d %d\n",GlobalWord, ntempvowels,ntempcodas[0],ntempcodas[1]);
					fclose(fp1);
		
				
				} else {
					for(k=1;k<=4;k++) {
						if(ntempcodas[i]>=k) {
							if ((strcmp(tempcodas[i][k],"ts")) == 0 || \
							(strcmp(tempcodas[i][k],"pp")) == 0 || \
							(strcmp(tempcodas[i][k],"zz")) == 0 || \
							(strcmp(tempcodas[i][k],"th")) == 0 || \
							(strcmp(tempcodas[i][k],"tz")) == 0 || \
							(strcmp(tempcodas[i][k],"ph")) == 0 || \
							(strcmp(tempcodas[i][k],"sh")) == 0 || \
							(strcmp(tempcodas[i][k],"dt")) == 0 || \
							(strcmp(tempcodas[i][k],"pf")) == 0 || \
							(strcmp(tempcodas[i][k],"ng")) == 0 || \
							(strcmp(tempcodas[i][k],"tt")) == 0 || \
							(strcmp(tempcodas[i][k],"ss")) == 0 || \
							(strcmp(tempcodas[i][k],"dd")) == 0 || \
							(strcmp(tempcodas[i][k],"gg")) == 0 || \
							(strcmp(tempcodas[i][k],"ff")) == 0 || \
							(strcmp(tempcodas[i][k],"bb")) == 0 || \
							(strcmp(tempcodas[i][k],"mm")) == 0 || \
							(strcmp(tempcodas[i][k],"kk")) == 0 || \
							(strcmp(tempcodas[i][k],"sch")) == 0 || \
							(strcmp(tempcodas[i][k],"tsch")) == 0 || \
							(strcmp(tempcodas[i][k],"th")) == 0 || \
							(strcmp(tempcodas[i][k],"ds")) == 0	){
							
								notchanged = 1;

							/*	fp1 = fopen("Split.ts.txt","a");
								fprintf(fp1,"%s %d\n",tempcodas[i][k],strlen(tempcodas[i][k]) );
								fclose(fp1);*/
								
								for(j=k+1;j<ntempcodas[i];j++) {
									strcpy(temponsets[i+1][j-k],tempcodas[i][j]);
									temponsetsnum[i+1][j-(k)] = tempcodasnum[i][j];
								}
								ntemponsets[i+1] = ntempcodas[i] -  k ;

								for(l=0;l<strlen(tempcodas[i][k]) - 1;l++) {
									temponsets[i+1][0][l] = tempcodas[i][k][l+1];
								}
								temponsets[i+1][0][l] = 0;

					

								if(strlen(temponsets[i+1][0]) == 1) {
									temponsetsnum[i+1][0] = NGRAPHEMES +FindLetter(temponsets[i+1][0][0]);
								} else {
									temponsetsnum[i+1][0] = findGrapheme(temponsets[i+1][0]);
								}
		
								/*fp1 = fopen("Split.ts.txt","a");
								fprintf(fp1,"%s \n",temponsets[i+1][0]);
								fclose(fp1);*/


								tempcodas[i][k][1] = 0;
								tempcodasnum[i][k] = NGRAPHEMES +FindLetter(tempcodas[i][k][0]);
								ntempcodas[i] = 2 + k-1;

								fp1 = fopen("Split.ts.txt","a");
								fprintf(fp1,"SV %s %s %s %d %d %d %d\n",GlobalWord, temponsets[i+1][0], tempcodas[i][k], strlen(tempcodas[i][k]), ntempvowels,ntempcodas[0],ntempcodas[1]);
								fclose(fp1);
								break;
							}
						}			 					
					}
					if(notchanged == 0) {
						fp1 = fopen("Split.stilldif.txt","a");
						fprintf(fp1,"SV %s %d %d %d\n",GlobalWord, ntempvowels,ntempcodas[0],ntempcodas[1]);
						fclose(fp1);
						ToTrain = 0;
					}
				}
			} else {
				fp = fopen("wotdis.txt","a");
				fprintf(fp, "%d %s %d %d \n",i, GlobalWord,ntempvowels,nphonvowels);
				fclose(fp);	
				ToTrain = 0;
				return;
			}
		}
	}

	for(i=0;i<ntempvowels-1;i++) {
		if(ntemponsets[i+1] == 0 && ntempcodas[i] > 0) {
				fp = fopen("wotdis.morph.txt","a");
				fprintf(fp, "%d %s %d %d \n",i, GlobalWord,ntempvowels,nphonvowels);
				fclose(fp);		
				//ToTrain = 0;
			//	return;
		}
	}


	if(i!=3) {
		for(i=ntempvowels;i<4;i++) {
			strcat(NewWord,"+");
			strcat(NewWord,"+");
			strcat(NewWord,"+");
		}
		j=strlen(NewWord);
		NewWord[j-1] = 0;
	}
	ToTrain = 1;
	strcpy(AlignedInputs,NewWord);
#endif
}

void GraphemeParsing::AlignInputsItalian() 
{
#if ITALIAN
	int i,j,k,a;
	char NewWord[100];
	FILE *fp, *fp1;

	NewWord[0] = 0;
	
	PrintWord();
	/*if(ENGLISH ==1) {
		FixEarlyExceptions();
	} else {*/
		//FixEarlyExceptionsItalian();
	//}
	/*	fp = fopen("difvowels1111.txt","a");
		fprintf(fp, "%s %d %d \n",GlobalWord,ntempvowels,nphonvowels);
		fclose(fp);*/
//	if(GlobalNPhonSylls != ntempvowels) {
	if(nphonvowels != ntempvowels) {
		fp = fopen("difvowels.txt","a");
		fprintf(fp, "%s %d %d \n",GlobalWord,ntempvowels,nphonvowels);
		fclose(fp);
		strcpy(AlignedInputs,"XX");
		ToTrain = 0;
		difvowels = 1;
		return;
	} 

	ToTrain = 1;

	strcpy(ThisWord,GlobalWord);

//	FindBestPlace();
	for(j=0;j<ntemponsets[0];j++) {
		strcat(NewWord,temponsets[0][j]);
		strcat(NewWord,".");
	}
	strcat(NewWord,"+");

	for(i=0;i<ntempvowels;i++) {
		/*for(j=0;j<ntempcodas[i];j++) {
			if((IsVowel(tempcodas[i][j][0])) != -1) {
				fp = fopen("difvowels.errorE. txt","a");
				fprintf(fp, "%s %d %d \n",GlobalWord,ntempvowels,nphonvowels);
				fclose(fp);
			}
		}*/
		strcat(NewWord,tempvowels[i]);
		strcat(NewWord,"+");
		if(i== (ntempvowels-1) ) { //FINAL SYLLABLE
			for(j=0;j<ntempcodas[i];j++) {
				strcat(NewWord,tempcodas[i][j]);
				strcat(NewWord,".");
			}
			strcat(NewWord,"+");
		} else {
			if( (ntempcodas[i] + ntemponsets[i+1]) == (nphoncodas[i] + nphononsets[i+1]) ) { //same
				for(j=0;j<nphoncodas[i];j++) {
					strcat(NewWord,tempcodas[i][j]);
					strcat(NewWord,".");
				}
				ntempcodas[i] = nphoncodas[i];
				strcat(NewWord,"+");
				for(j=0;j<nphononsets[i+1];j++) {
					strcat(NewWord,tempcodas[i][j+nphoncodas[i]]);
					strcat(NewWord,".");
					strcpy(temponsets[i+1][j],tempcodas[i][j+nphoncodas[i]]);
					temponsetsnum[i+1][j] = tempcodasnum[i][j+nphoncodas[i]];
				}
				ntemponsets[i+1] = nphononsets[i+1];				
				strcat(NewWord,"+");

				if(nphononsets[i+1] == 0 && nphoncodas[i] != 0 && i<ntempvowels) {
					fp1 = fopen("Split.Ita.NoOnsetsbutCoda.txt","a");
					fprintf(fp1,"SV %s %s %d %d %d\n",GlobalWord, temponsets[i+1][0], ntempvowels,ntempcodas[i+1],ntempcodas[i]);
					fclose(fp1);
			
				}



			} else if(ntempcodas[i]+ntemponsets[i+1] == (nphoncodas[i] + nphononsets[i+1] - 1) && \
				 (phononsets[i+1][nphononsets[i+1]-1] == 'j' || phononsets[i+1][nphononsets[i+1]-1] == 'w')	) { 

				for(j=0;j<nphoncodas[i];j++) {
					strcat(NewWord,tempcodas[i][j]);
					strcat(NewWord,".");
				}
				ntempcodas[i] = nphoncodas[i];
				strcat(NewWord,"+");
				for(j=0;j<nphononsets[i+1]-1;j++) {
					strcat(NewWord,tempcodas[i][j+nphoncodas[i]]);
					strcat(NewWord,".");
					strcpy(temponsets[i+1][j],tempcodas[i][j+nphoncodas[i]]);
					temponsetsnum[i+1][j] = tempcodasnum[i][j+nphoncodas[i]];
				}
				ntemponsets[i+1] = nphononsets[i+1] - 1;
	
				strcat(NewWord,"+");

				fp = fopen("wotdis2.txt","a");
				fprintf(fp, "%s %d %d \n",GlobalWord,ntempvowels,nphonvowels);
				fclose(fp);
			} else if( (ntempcodas[i]+ntemponsets[i+1]) == (nphoncodas[i] + nphononsets[i+1] - 1)  && \
				ntempcodas[i] == 1 && (strcmp(tempcodas[i][0],"sc")) == 0 ||\
				ntempcodas[i] == 2 && (strcmp(tempcodas[i][1],"sc")) == 0 ) 	{
		
				if(nphoncodas[i] == 1 && nphononsets[i+1] == 1) {
					
					strcpy(temponsets[i+1][0],"sc");
					temponsetsnum[i+1][0] = findGrapheme("sc");
					ntempcodas[i] = 0;
					ntemponsets[i+1] = 1;
					strcat(NewWord,"sc.+");
					fp = fopen("wotdis.sc.keep.txt","a");
					fprintf(fp, "%s %d %d %d\n",GlobalWord,ntempvowels,nphonvowels,FindLetter('c'));
					fclose(fp);

					
				} else {
					strcpy(temponsets[i+1][0],"s");
					strcpy(temponsets[i+1][1],"c");
					temponsetsnum[i+1][0] = NONSETS + NCODAS + NVOWELS + FindLetter('s');
					temponsetsnum[i+1][1] = NONSETS + NCODAS + NVOWELS + FindLetter('c');
					ntempcodas[i]--;;
					ntemponsets[i+1] = 2;

					if(ntempcodas[i] == 1) {
						strcat(NewWord,tempcodas[i][0]);
						strcat(NewWord,".");
					}
					strcat(NewWord,"s.c.+");
					fp = fopen("wotdis.sc.txt","a");
					fprintf(fp, "%s %d %d %d\n",GlobalWord,ntempvowels,nphonvowels,FindLetter('c'));
					fclose(fp);
				}
			} else if (ntempcodas[i] == 1) { 

				if(nphononsets[i+1] == 2 && i < ntempvowels-1) {
					fp = fopen("wotdis.onetwo.txt","a");
					fprintf(fp, "%d %s %s %d %d \n",i, GlobalWord,PhonString, ntempvowels,nphonvowels);
					fclose(fp);	
				}

				if((strcmp(tempcodas[i][0],"gl")) == 0 && phononsets[i+1][0] == 'g' && phononsets[i+1][1] == 'l') {
					strcpy(temponsets[i+1][0],"g");
					strcpy(temponsets[i+1][1],"l");
					temponsetsnum[i+1][0] = NONSETS + NCODAS + NVOWELS + FindLetter('g');
					temponsetsnum[i+1][1] = NONSETS + NCODAS + NVOWELS + FindLetter('l');
					ntempcodas[i] = 0;
					ntemponsets[i+1] = 2;
					
					strcat(NewWord,tempcodas[i][0]);
					strcat(NewWord,".g.l.+");	
				
					fp = fopen("wotdis.gl.txt","a");
					fprintf(fp, "%d %s %d %d \n",i, GlobalWord,ntempvowels,nphonvowels);
					fclose(fp);	
				} else {
					ntempcodas[i] = 0;
					strcat(NewWord,"+");
					for(j=0;j<1;j++) {
						strcat(NewWord,tempcodas[i][0]);
						strcat(NewWord,".");
						strcpy(temponsets[i+1][j],tempcodas[i][0]);
						temponsetsnum[i+1][j] = tempcodasnum[i][0];
					}
					ntemponsets[i+1] = 1;						
					strcat(NewWord,"+");	

					fp = fopen("wotdis.one.txt","a");
					fprintf(fp, "%d %s %s %d %d \n",i, GlobalWord,PhonString, ntempvowels,nphonvowels);
					fclose(fp);
				}
		//	} else if(ntempcodas[i]+ntemponsets[i+1] == (nphoncodas[i] + nphononsets[i+1] - 1) && (ntempcodas[i] > 1 )) { 
			} else if(ntempcodas[i]+ntemponsets[i+1] < (nphoncodas[i] + nphononsets[i+1]) && (ntempcodas[i] > 1 )) { 
		
				if( (strcmp(tempcodas[i][0],"sc")) == 0 && (strcmp(tempcodas[i][1],"r")) == 0 ||
					(strcmp(tempcodas[i][0],"sc")) == 0 && (strcmp(tempcodas[i][1],"l")) == 0) {
					
					strcpy(temponsets[i+1][0],"s");
					strcpy(temponsets[i+1][1],"c");
					strcpy(temponsets[i+1][2],tempcodas[i][1]);
					temponsetsnum[i+1][0] = NONSETS + NCODAS + NVOWELS + FindLetter('s');
					temponsetsnum[i+1][1] = NONSETS + NCODAS + NVOWELS + FindLetter('c');
					temponsetsnum[i+1][2] = NONSETS + NCODAS + NVOWELS + FindLetter(temponsets[i+1][2][0]);
					ntempcodas[i] = 0;
					ntemponsets[i+1] = 3;


					strcat(NewWord,"s.c.");
					strcat(NewWord,temponsets[i+1][2]);
					strcat(NewWord,".+");


					fp = fopen("wotdis.scr.txt","a");
					fprintf(fp, "%d %s %d %d \n",i, GlobalWord,ntempvowels,nphonvowels);
					fclose(fp);	
				}			
				if(/*ntempcodas[i] == 2*//*(strcmp(tempcodas[i][0],"g")) == 0 *//* && */ (strcmp(tempcodas[i][1],"gl")) == 0 && phononsets[i+1][0] == 'g' && phononsets[i+1][1] == 'l') {
					//strcpy(tempcodas[i][0],"g");
					strcpy(temponsets[i+1][0],"g");
					strcpy(temponsets[i+1][1],"l");
					//tempcodasnum[i][0] = NONSETS + NCODAS + NVOWELS + FindLetter(temp);
					temponsetsnum[i+1][0] = NONSETS + NCODAS + NVOWELS + FindLetter('g');
					temponsetsnum[i+1][1] = NONSETS + NCODAS + NVOWELS + FindLetter('l');
					ntempcodas[i] -= 1;
					ntemponsets[i+1] = 2;
				
					strcat(NewWord,tempcodas[i][0]);
					strcat(NewWord,".g.l.+");	
				
					fp = fopen("wotdis.ggl.txt","a");
					fprintf(fp, "%d %s %d %d \n",i, GlobalWord,ntempvowels,nphonvowels);
					fclose(fp);	
				}
			

			} else {
				fp = fopen("wotdis.txt","a");
				fprintf(fp, "%d %s %d %d \n",i, GlobalWord,ntempvowels,nphonvowels);
				fclose(fp);	
				ToTrain = 0;
				return;
			}
		}
	}

	for(i=0;i<ntempvowels-1;i++) {
		if(ntemponsets[i+1] == 0 && ntempcodas[i] > 0) {
				fp = fopen("wotdis.err1.txt","a");
				fprintf(fp, "%d %s %d %d \n",i, GlobalWord,ntempvowels,nphonvowels);
				fclose(fp);		
				ToTrain = 0;
				return;
		}
	}


	if(i!=3) {
		for(i=ntempvowels;i<4;i++) {
			strcat(NewWord,"+");
			strcat(NewWord,"+");
			strcat(NewWord,"+");
		}
		j=strlen(NewWord);
		NewWord[j-1] = 0;
	}
	
	strcpy(AlignedInputs,NewWord);
#endif
}

void GraphemeParsing::AlignInputsFrench() 
{
	int i,j,k,l,m,a;
	char NewWord[100];
	int e_count;
	FILE *fp, *fp1;

	NewWord[0] = 0;
	
	PrintWord();

	/*fp = fopen("here666666.txt","a");
		fprintf(fp, "%s %d %d \n",GlobalWord,ntempvowels,nphonvowels);
		fclose(fp);*/

	FixEarlyExceptionsFrench();

		/*fp = fopen("here7777.txt","a");
		fprintf(fp, "%s %d %d \n",GlobalWord,ntempvowels,nphonvowels);
		fclose(fp);*/

	if(nphonvowels != ntempvowels) {
		fp = fopen("difvowels.txt","a");
		fprintf(fp, "%s %d %d \n",GlobalWord,ntempvowels,nphonvowels);
		fclose(fp);
		strcpy(AlignedInputs,"XX");
		ToTrain = 0;
		difvowels = 1;
		return;
	} 

	ToTrain = 1;

	strcpy(ThisWord,GlobalWord);

//	FindBestPlace();
	for(j=0;j<ntemponsets[0];j++) {
		strcat(NewWord,temponsets[0][j]);
		strcat(NewWord,".");
	}
	strcat(NewWord,"+");
	//PrintWordName("test111.txt");
	for(i=0;i<ntempvowels;i++) {
		e_count = 0;
		for(j=0;j<ntempcodas[i];j++) {
			if(strcmp(tempcodas[i][j],"e") == 0) e_count++;
		}
		/*for(j=0;j<ntempcodas[i];j++) {
			if((IsVowel(tempcodas[i][j][0])) != -1) {
				fp = fopen("difvowels.errorE. txt","a");
				fprintf(fp, "%s %d %d \n",GlobalWord,ntempvowels,nphonvowels);
				fclose(fp);
			}
		}*/
		strcat(NewWord,tempvowels[i]);
		strcat(NewWord,"+");
		if(i == (ntempvowels-1) ) { //FINAL SYLLABLE
			for(j=0;j<ntempcodas[i];j++) {
				strcat(NewWord,tempcodas[i][j]);
				strcat(NewWord,".");
			}
			strcat(NewWord,"+");
		} else {
			if( (strcmp(tempcodas[i][0],"x")) == 0 && ntempcodas[i] > 0) {
					strcat(NewWord,"+");//codas
		
					for(j=0;j<ntempcodas[i];j++) {
						strcat(NewWord,tempcodas[i][j]);
						strcat(NewWord,".");	
						strcpy(temponsets[i+1][j],tempcodas[i][j]);
						temponsetsnum[i+1][j] = tempcodasnum[i][j];
					}
					strcat(NewWord,"+");
					ntemponsets[i+1] = ntempcodas[i];
					ntempcodas[i] = 0;
					fp = fopen("OnePhonCod.x.txt","a");
					fprintf(fp, "%d %s %d %d \n",i, GlobalWord,ntempvowels,nphonvowels);
					fclose(fp);	
			} else if( (ntempcodas[i] + ntemponsets[i+1]) == (nphoncodas[i] + nphononsets[i+1]) ) { //same
				for(j=0;j<nphoncodas[i];j++) {
					strcat(NewWord,tempcodas[i][j]);
					strcat(NewWord,".");
				}
				ntempcodas[i] = nphoncodas[i];
				strcat(NewWord,"+");
				for(j=0;j<nphononsets[i+1];j++) {
					strcat(NewWord,tempcodas[i][j+nphoncodas[i]]);
					strcat(NewWord,".");
					strcpy(temponsets[i+1][j],tempcodas[i][j+nphoncodas[i]]);
					temponsetsnum[i+1][j] = tempcodasnum[i][j+nphoncodas[i]];
				}
				ntemponsets[i+1] = nphononsets[i+1];				
				strcat(NewWord,"+");

				/*fp1 = fopen("normal.txt","a");
				fprintf(fp1,"SV %s %s %d %d %d\n",GlobalWord, temponsets[i+1][0], ntempvowels,ntempcodas[i+1],ntempcodas[i]);
				fclose(fp1);*/

				if(nphononsets[i+1] == 0 && nphoncodas[i] != 0 && i<ntempvowels) {
					fp1 = fopen("Split.Fr.NoOnsetsbutCoda.txt","a");
					fprintf(fp1,"SV %s %s %d %d %d\n",GlobalWord, temponsets[i+1][0], ntempvowels,ntempcodas[i+1],ntempcodas[i]);
					fclose(fp1);
				}
			} else if (ntempcodas[i] == 1 ) { 
					if(( strcmp(tempcodas[i][0],"cc")) == 0 && (nphoncodas[i] + nphononsets[i+1]) == 2) {
						//ntempcodas[i] = 1;
						fp = fopen("wotdis.onetwo.cc.txt","a");
						fprintf(fp, "%d %s %s %d %d \n",i, GlobalWord,PhonString, ntempvowels,nphonvowels);
						fclose(fp);	

						strcat(NewWord,tempcodas[i][0]);
						strcat(NewWord,".");	
						strcat(NewWord,"+");
						strcat(NewWord,"+");
					} else {
						strcat(NewWord,"+");
						strcat(NewWord,tempcodas[i][0]);
						strcat(NewWord,".");	
						strcpy(temponsets[i+1][0],tempcodas[i][0]);
						temponsetsnum[i+1][0] = tempcodasnum[i][0];
						ntempcodas[i] = 0;
						ntemponsets[i+1] = 1;
						strcat(NewWord,"+");

						fp = fopen("wotdis.onetwo.txt","a");
						fprintf(fp, "%d %s %s %d %d \n",i, GlobalWord,PhonString, ntempvowels,nphonvowels);
						fclose(fp);	
					}
			} else if(ntempcodas[i] == nphoncodas[i]+nphononsets[i+1]+1) {
				//for(j=1;j<ntempcodas[i];j++) {	
				for(j=0;j<ntempcodas[i];j++) {
					if((strcmp(tempcodas[i][j],"e")) == 0) break;
				}
				// adopterais  adop.terai IMAGERIE
				if(j!=ntempcodas[i]) {
					fp = fopen("plusone.txt","a");
					fprintf(fp, "%d %d %d %s %s %d %d %s\n",i,j, nphoncodas[i], GlobalWord,PhonString, ntempvowels,nphonvowels,NewWord);
					fclose(fp);
					if((strcmp(tempcodas[i][0],"e")) == 0) { //first e should always be coda
						k = 1;
					} else {
						for(k=0;k<nphoncodas[i];k++) {
							strcat(NewWord,tempcodas[i][k]);
							strcat(NewWord,".");		
						}
						if(k != 0 && (strcmp(tempcodas[i][k],"e")) == 0) {
							strcat(NewWord,tempcodas[i][k]);
							strcat(NewWord,".");
							k++;
						}
					}
					strcat(NewWord,"+");
				
					l=ntempcodas[i];
					ntempcodas[i] = k;
					ntemponsets[i+1] = 0;

					for(m=k;m<l;m++) {
						strcat(NewWord,tempcodas[i][m]);
						strcat(NewWord,".");	
						strcpy(temponsets[i+1][m-k],tempcodas[i][m]);
						temponsetsnum[i+1][m-k] = tempcodasnum[i][m];
						ntemponsets[i+1]++;
					}
					strcat(NewWord,"+");		
				} else if (nphoncodas[i] == 0) {
					strcat(NewWord,"+");
					ntemponsets[i+1] = 0;

					j = 0;
					if( (strcmp(tempcodas[i][1],"h")) == 0) {			
						for(j=0;j<ntempcodas[i];j++) {
							strcpy(temponsets[i+1][j],tempcodas[i][j]);
							temponsetsnum[i+1][j] = tempcodasnum[i][j];
							ntemponsets[i+1]++;
						}
						ntempcodas[i] = 0;		
					} else {
						for(j=1;j<ntempcodas[i];j++) {
							strcpy(temponsets[i+1][j-1],tempcodas[i][j]);
							temponsetsnum[i+1][j-1] = tempcodasnum[i][j];
							ntemponsets[i+1]++;
						}
						ntempcodas[i] = 1;
					}
					strcat(NewWord,"+");	
					fp = fopen("inherefix.zerio.txt","a");
					fprintf(fp, "%d %s %s %d %d %s\n",i, GlobalWord,PhonString, ntempvowels,nphonvowels,NewWord);
					fclose(fp);
				} else if (nphoncodas[i] == 1 && nphononsets[i+1] == 1) {
					//leave first codas (incledes e)
					fp = fopen("inherefix.one.one.txt","a");
					fprintf(fp, "%d %d %s %s %d %d \n",ntempcodas[i],i, GlobalWord,PhonString, ntempvowels,nphonvowels);
					fclose(fp);	
					if((strcmp(tempcodas[i][ntempcodas[i]-1],"h")) != 0) { 
						strcat(NewWord,tempcodas[i][0]); strcat(NewWord,".");
						strcat(NewWord,tempcodas[i][1]); strcat(NewWord,".");	
						ntempcodas[i] = 2;
						strcat(NewWord,"+");
						strcpy(temponsets[i+1][0],tempcodas[i][2]);
						temponsetsnum[i+1][0] = tempcodasnum[i][2];
						strcat(NewWord,temponsets[i+1][0]);
						ntemponsets[i+1] = 1;
						strcat(NewWord,"+");	
					} else {
						strcat(NewWord,tempcodas[i][0]); strcat(NewWord,".");
						ntempcodas[i] = 1;
						strcat(NewWord,"+");
						strcpy(temponsets[i+1][0],tempcodas[i][1]);
						temponsetsnum[i+1][0] = tempcodasnum[i][1];
						strcat(NewWord,temponsets[i+1][0]);
						strcpy(temponsets[i+1][1],tempcodas[i][2]);
						temponsetsnum[i+1][1] = tempcodasnum[i][2];
						strcat(NewWord,temponsets[i+1][2]);
						ntemponsets[i+1] = 2;
						strcat(NewWord,"+");	
					}

					fp = fopen("inherefix.TWO.ONE.txt","a");
					fprintf(fp, "%d %d %s %s %d %d \n",ntempcodas[i],i, GlobalWord,PhonString, ntempvowels,nphonvowels);
					fclose(fp);	
				} else if(nphoncodas[i] == 1 && (nphononsets[i+1] == 2 || nphononsets[i+1] ==3)) {
					fp = fopen("inherefix.one.two.txt","a");
					fprintf(fp, "%d %d %s %s %d %d \n",ntempcodas[i],i, GlobalWord,PhonString, ntempvowels,nphonvowels);
					fclose(fp);	
					if(nphoncodas[i] == 1 && strcmp(tempcodas[i][1],"e") == 0) {
						strcat(NewWord,tempcodas[i][0]);
						strcat(NewWord,tempcodas[i][1]);
						ntempcodas[i] = 2;
						strcat(NewWord,"+");
						strcpy(temponsets[i+1][0],tempcodas[i][2]);
						strcpy(temponsets[i+1][1],tempcodas[i][3]);
						if(nphononsets[i+1] == 3) strcpy(temponsets[i+1][2],tempcodas[i][4]);
						temponsetsnum[i+1][0] = tempcodasnum[i][2];
						temponsetsnum[i+1][1] = tempcodasnum[i][3];
						if(nphononsets[i+1] == 3) temponsetsnum[i+1][2] = tempcodasnum[i][4];
						strcat(NewWord,temponsets[i+1][0]);	strcat(NewWord,".");
						strcat(NewWord,temponsets[i+1][1]);	strcat(NewWord,".");
						if(nphononsets[i+1] == 3) strcat(NewWord,temponsets[i+1][2]); strcat(NewWord,".");
						if(nphononsets[i+1] == 3) ntemponsets[i+1] = 3;
						else ntemponsets[i+1] = 2;
						strcat(NewWord,"+");
					} else {
						strcat(NewWord,tempcodas[i][0]);
						strcat(NewWord,".");
						ntempcodas[i] = 1;
						strcat(NewWord,"+");
						strcpy(temponsets[i+1][0],tempcodas[i][1]);
						strcpy(temponsets[i+1][1],tempcodas[i][2]);
						strcpy(temponsets[i+1][2],tempcodas[i][3]);
						temponsetsnum[i+1][0] = tempcodasnum[i][1];
						temponsetsnum[i+1][1] = tempcodasnum[i][2];
						temponsetsnum[i+1][2] = tempcodasnum[i][3];
						strcat(NewWord,temponsets[i+1][0]);	strcat(NewWord,".");
						strcat(NewWord,temponsets[i+1][1]);	strcat(NewWord,".");
						strcat(NewWord,temponsets[i+1][2]);	strcat(NewWord,".");
						ntemponsets[i+1] = 3;
						if(nphononsets[i+1] == 3) {
							strcpy(temponsets[i+1][3],tempcodas[i][4]);
							temponsetsnum[i+1][3] = tempcodasnum[i][4];
							ntemponsets[i+1] = 4;
							strcat(NewWord,temponsets[i+1][3]);
							strcat(NewWord,".");					
						}			
						strcat(NewWord,"+");
					}
				} else {
					fp = fopen("inherefix.two.two.txt","a");
					fprintf(fp, "%d %d %s %s %d %d \n",ntempcodas[i],i, GlobalWord,PhonString, ntempvowels,nphonvowels);
					fclose(fp);	
				}
			} else if (nphoncodas[i] == 0) {

				//strcat(NewWord,"+");//codas
				
				for(j=0;j<ntempcodas[i];j++) {
					//fp = fopen("OnlyOnsets.one.plus.1e.txt","a");
				//	fprintf(fp, "%d %d %s\n",i,j,tempcodas[i][j]);
				//	fclose(fp);				
					if((strcmp(tempcodas[i][j],"e")) == 0) break;
				}
				if(j!=ntempcodas[i] && j >= 1  && (strcmp(tempcodas[i][j+1],"r")) == 0) {
					fp = fopen("OnlyOnsets.one.plus.1e.txt","a");
					fprintf(fp, "%d %s %d %d \n",i, GlobalWord,ntempvowels,nphonvowels);
					fclose(fp);	
					
					l = ntempcodas[i];		
					ntempcodas[i] = j-1;
					strcat(NewWord,"."); strcat(NewWord,"+");
					for(k=j-1;k<l;k++) {
						strcat(NewWord,tempcodas[i][k]);
						strcat(NewWord,".");	
						strcpy(temponsets[i+1][k-(j-1)],tempcodas[i][k]);
						temponsetsnum[i+1][k-(j-1)] = tempcodasnum[i][k];
					}
					ntemponsets[i+1] = k-(j-1);
					strcat(NewWord,"+");
				} else {
					strcat(NewWord,"+");
					//PrintWordName("test111.txt");
					for(j=0;j<ntempcodas[i];j++) {
						strcat(NewWord,tempcodas[i][j]);
						strcat(NewWord,".");	
						strcpy(temponsets[i+1][j],tempcodas[i][j]);
						temponsetsnum[i+1][j] = tempcodasnum[i][j];
					}
					ntemponsets[i+1] = ntempcodas[i];
					ntempcodas[i] = 0;
					strcat(NewWord,"+");
				//PrintWordName("test111.txt");
				}
				fp = fopen("OnlyOnsets.one.txt","a");
				fprintf(fp, "%d %s %d %d \n",i, GlobalWord,ntempvowels,nphonvowels);
				fclose(fp);	
			} else if (ntempcodas[i] == 0) {
				fp = fopen("zeros.txt","a");
				fprintf(fp, "%d %s %d %d \n",i, GlobalWord,ntempvowels,nphonvowels);
				fclose(fp);	
			} else if (nphoncodas[i] == 0 && nphononsets[i+1] > 0) {
				//strcat(NewWord,"+");//codas
				
				for(j=0;j<ntempcodas[i];j++) {
					fp = fopen("OnlyOnsets.one.plus.1e.txt","a");
					fprintf(fp, "%d %d %s\n",i,j,tempcodas[i][j]);
					fclose(fp);	
					
					if((strcmp(tempcodas[i][j],"e")) == 0) break;
				}
				if(j!=ntempcodas[i] /*&& j >= 1 */ /*&& (strcmp(tempcodas[i][j+1],"r")) == 0*/) {
					fp = fopen("OnlyOnsets.one.plus.1e.txt","a");
					fprintf(fp, "%d %s %d %d \n",i, GlobalWord,ntempvowels,nphonvowels);
					fclose(fp);	
					
					l = ntempcodas[i];		
					ntempcodas[i] = j-1;
					strcat(NewWord,"."); strcat(NewWord,"+");
					for(k=j;k<l;j++) {
						strcat(NewWord,tempcodas[i][k]);
						strcat(NewWord,".");	
						strcpy(temponsets[i+1][k-j],tempcodas[i][k]);
						temponsetsnum[i+1][k-j] = tempcodasnum[i][k];
					}
					ntemponsets[i+1] = k-j;
					strcat(NewWord,"+");

				} else {
					strcat(NewWord,"+");//codas
					for(j=0;j<ntempcodas[i];j++) {
						strcat(NewWord,tempcodas[i][j]);
						strcat(NewWord,".");	
						strcpy(temponsets[i+1][j],tempcodas[i][j]);
						temponsetsnum[i+1][j] = tempcodasnum[i][j];
					}
					ntemponsets[i+1] = ntempcodas[i];
					ntempcodas[i] = 0;
					strcat(NewWord,"+");
				}

				fp = fopen("OnlyOnsets.one.plus.txt","a");
				fprintf(fp, "%d %s %d %d \n",i, GlobalWord,ntempvowels,nphonvowels);
				fclose(fp);	
			} else if (nphoncodas[i] == 1) {	
				strcat(NewWord,tempcodas[i][0]);
				strcat(NewWord,"+");//codas
					
					//leave first in coda.
				for(j=1;j<ntempcodas[i];j++) {
					strcat(NewWord,tempcodas[i][j]);
					strcat(NewWord,".");	
					strcpy(temponsets[i+1][j-1],tempcodas[i][j]);
					temponsetsnum[i+1][j-1] = tempcodasnum[i][j];
				}
				strcat(NewWord,"+");
				fp = fopen("OnePhonCod.notx.txt","a");
				fprintf(fp, "%d %s %d %d \n",i, GlobalWord,ntempvowels,nphonvowels);
				fclose(fp);
			} else {
				fp = fopen("wotdis.txt","a");
				fprintf(fp, "%d %s %d %d \n",i, GlobalWord,ntempvowels,nphonvowels);
				fclose(fp);	
				ToTrain = 0;
				return;
			}
		}
	}

	/*for(i=0;i<ntempvowels-1;i++) {
		if(ntemponsets[i+1] == 0 && ntempcodas[i] > 0) {
				fp = fopen("wotdis.err1.txt","a");
				fprintf(fp, "%d %s %d %d \n",i, GlobalWord,ntempvowels,nphonvowels);
				fclose(fp);		
				ToTrain = 0;
				return;
		}
	}*/


	if(i!=3) {
		for(i=ntempvowels;i<4;i++) {
			strcat(NewWord,"+");
			strcat(NewWord,"+");
			strcat(NewWord,"+");
		}
		j=strlen(NewWord);
		NewWord[j-1] = 0;
	}
	PrintWord();
	strcpy(AlignedInputs,NewWord);
}



void GraphemeParsing::AlignInputs() 
{
	int i,j,k,a;
	char NewWord[100];
	FILE *fp, *fp1;

	NewWord[0] = 0;
	
	if(ENGLISH ==1) {
		FixEarlyExceptions();
	} else {
		//if(ITALIAN == 1) FixEarlyExceptionsItalian();
	}

//	if(GlobalNPhonSylls != ntempvowels) {
	if(nphonvowels != ntempvowels) {
		fp = fopen("difvowels.txt","a");
		fprintf(fp, "%s %d %d \n",GlobalWord,ntempvowels,nphonvowels);
		fclose(fp);
		strcpy(AlignedInputs,"XX");
		ToTrain = 0;
		difvowels = 1;
		return;
	} 
	ToTrain = 1;

	strcpy(ThisWord,GlobalWord);

//	FindBestPlace();
	for(j=0;j<ntemponsets[0];j++) {
		strcat(NewWord,temponsets[0][j]);
		strcat(NewWord,".");
	}
	strcat(NewWord,"+");


	//PrintWord();

	for(i=0;i<ntempvowels;i++) {	
		strcat(NewWord,tempvowels[i]);
		strcat(NewWord,"+");
		if(i==(ntempvowels-1)) { //FINAL SYLLABLE
			for(j=0;j<ntempcodas[i];j++) {
				strcat(NewWord,tempcodas[i][j]);
				strcat(NewWord,".");
			}
			strcat(NewWord,"+");
		} else {
		//Same
			if( (ntempcodas[i] + ntemponsets[i+1]) == (nphoncodas[i] + nphononsets[i+1]) ) { 
				for(j=0;j<nphoncodas[i];j++) {
					strcat(NewWord,tempcodas[i][j]);
					strcat(NewWord,".");
				}
				ntempcodas[i] = nphoncodas[i];
				strcat(NewWord,"+");
				for(j=0;j<nphononsets[i+1];j++) {
					strcat(NewWord,tempcodas[i][j+nphoncodas[i]]);
					strcat(NewWord,".");
					strcpy(temponsets[i+1][j],tempcodas[i][j+nphoncodas[i]]);
					temponsetsnum[i+1][j] = tempcodasnum[i][j+nphoncodas[i]];
				}
				ntemponsets[i+1] = nphononsets[i+1];

					if(nphononsets[i+1] == 0 && nphoncodas[i] != 0 && i < ntempvowels-1) {
					fp1 = fopen("Split.Eng.NoOnsetsbutCoda.txt","a");
					fprintf(fp1,"SV %s %d %s %s\n",GlobalWord, ntempcodas[i],tempvowels[i],tempcodas[i][ntempcodas[i]-1]);
					fclose(fp1);		
				}	
				strcat(NewWord,"+");
			} else if (i == (ntempvowels-1)) { //pile up codas at the end.DELETE THIS DUPLICATED!!!
				for(j=0;j<ntempcodas[i];j++) {
					strcat(NewWord,tempcodas[i][j]);
					strcat(NewWord,".");
				//strcpy(tempcodas[i][j],tempcodas[i][j]);
				//tempcodasnum[i][j] = tempcodasnum[i][j];
				}
	
			/*	fp = fopen("codapile.txt","a");
				fprintf(fp, "%s \n",GlobalWord);
				fclose(fp);	*/
			/*} else 	if(ntempvowels == 2 && ntempcodas[0] == 0 && nphononsets[1] == 1) {	
				a = strlen(tempvowels[0]);
				if(a > 1) {
					if(ConsonantNum(tempvowels[0][a-1]) != -1 && tempvowels[0][a-1] != 'y') {			
						fp1 = fopen("ConsonantsSplit.txt","a");
						fprintf(fp1,"%s \n",GlobalWord);
						fclose(fp1);
						
						ntemponsets[1] = 1;
						temponsets[1][0][0] = tempvowels[0][a-1];
						temponsets[1][0][1] = 0;
						temponsetsnum[1][0] = NONSETS+NVOWELS+NCODAS + FindLetter(temponsets[1][0][0]);
						ntempcodas[0] = 0;
						
						tempvowels[0][a-1] = 0;
						j = findVowelnum(tempvowels[0]);  
						if(j != -1) tempvowelsnum[0] = NONSETS + j;
						else tempvowelsnum[0] = NONSETS+NVOWELS+NCODAS + FindLetter(tempvowels[0][0]);						
					//NOrthSylls--;								}
					}

				} */
			} else {
			//	PrintWord();
				//nothing to put anywhere
			
/*				fp = fopen("Inhere.txt","a");
					fprintf(fp, "%s %d %d %d %d %d \n",GlobalWord,i,ntempcodas[i],ntemponsets[i+1],nphoncodas[i],nphononsets[i+1]);
					fclose(fp);	*/
				if(ntempcodas[i] == 0) {
					strcat(NewWord,"+");
					strcat(NewWord,"+");				
					fp = fopen("nocods.txt","a");
					fprintf(fp, "%s \n",GlobalWord);
					fclose(fp);		
				} else {
				//No phon codas, load up onsets.
				/*	fp = fopen("hascods.txt","a");
					fprintf(fp, "%s %d %d %s\n",GlobalWord,i, ntempcodas[i],tempcodas[i][1]);
					fclose(fp);	
					PrintWord();*/

					if(nphoncodas[i] == 0) {
						//forerunner.
						for(j=ntempcodas[i]-1;j>=0;j--) {
							if((strcmp(tempcodas[i][j],"e")) == 0) {
								break;
							}
						}
						j++;

						for(k=0;k<j;k++) {
							strcat(NewWord,tempcodas[i][k]);
							strcat(NewWord,".");
						}
						

						strcat(NewWord,"+");
						for(k=j;k<ntempcodas[i];k++) {
							strcat(NewWord,tempcodas[i][k]);
							strcat(NewWord,".");
							strcpy(temponsets[i+1][k-j],tempcodas[i][k]);
							temponsetsnum[i+1][k-j] = tempcodasnum[i][k];	
						}
						ntemponsets[i+1] = ntempcodas[i]-j;
						strcat(NewWord,"+");
						ntempcodas[i] = j;
					} else {
					//maximize onset if only one
						if(ntempcodas[i] == 1) {
							strcat(NewWord,"+");
							for(j=0;j<ntempcodas[i];j++) {
								strcat(NewWord,tempcodas[i][j]);
								strcat(NewWord,".");
								strcpy(temponsets[i+1][j],tempcodas[i][j]);
								temponsetsnum[i+1][j] = tempcodasnum[i][j];	
							}
							ntemponsets[i+1] = ntempcodas[i];
							ntempcodas[i] = 0;
							strcat(NewWord,"+");
							
						/*	fp = fopen("singlegraph.txt","a");
							fprintf(fp, "%s XX \n",GlobalWord);
							fclose(fp);*/
						} else {
							fp = fopen("difs.txt","a");
							fprintf(fp, "%s %d %d %d\n",GlobalWord,i,ntempvowels,nphonvowels);
							fclose(fp);



							int topcodgrapheme, topcodspot,temp,h;
							float topcod;
							topcod = 0;

							topcodspot = -1;

							if(nphononsets[1] == 0 && nphoncodas[0] == 0) { /*Double vowel*/
								topcod = 100;
								topcodgrapheme = 0;
								topcodspot = -1;
							} else if( (strcmp(tempcodas[i][0],"x")) == 0) {
							/*	fp = fopen("x difs.txt","a");
								fprintf(fp, "%s %d\n",GlobalWord,i);
								fclose(fp);	*/
								//errorin DB. ks.tr vs. k.str.
								
								if(nphoncodas[i] > 1) {
									if(phoncodas[i][0] == 'k' && phoncodas[i][1] == 's') {
										topcodspot = nphoncodas[i] - 1 ;
									} else {
										topcodspot = 0;
									}
								} else {
									topcodspot = 0;
								}

							} else if( (strcmp(tempcodas[i][1],"qu")) == 0) {
								topcodspot = 1;						
							} else {
								/*fp = fopen("search.txt","a");
								fprintf(fp, "%s %d\n",GlobalWord,i);
									fclose(fp);*/

								topcod = -1;
								for(h=ntempcodas[i]-1; h>= 0; h--) {
							//	for(h=0; h<ntempcodas[i]; h++) {
									for(j=0;j<nGraphemesUpTo[4];j++) {
										/*fp = fopen("thestrings43.txt","a");
										fprintf(fp,"-->%s %d %d [%s] n=%d %f\n",GlobalWord, i,tempcodasnum[i][h],tempcodas[i][h], nGraphemesUpTo[4], GraphemeFreqsInf[4][j]);
										fclose(fp);
										fp = fopen("thestrings43.txt","a");
										fprintf(fp,"%s %s %d %d  %d %d\n",tempcodas[i][h], GraphemesInflets[4][j], tempcodasnum[i][h],GraphemesInf[4][j], PhonemeIndex(phononsets[i+1][0]), PhonemesInf[4][j]);
										fclose(fp);*/
										if( (strcmp(tempcodas[i][h],"e")) == 0) {
											break;
										}
										if(tempcodasnum[i][h] == GraphemesInf[4][j] && PhonemeIndex(phononsets[i+1][0]) == PhonemesInf[4][j]) {		
										//	fp = fopen("thestrings45.txt","a");
										//	fprintf(fp,"-->%s %d %d [%s] %f\n",GlobalWord, i,tempcodasnum[i][h],tempcodas[i][h], GraphemeFreqsInf[4][j]);
										//	fclose(fp);
											
											if(GraphemeFreqsInf[4][j] > topcod) {
												topcod = GraphemeFreqsInf[4][j];
												topcodspot = h;	
												topcodgrapheme = j;
											}
										}
									}
								}
								//hack
								if((strcmp(tempcodas[i][ntempcodas[i]-1],"y")) == 0) {
									topcodspot = ntempcodas[i]-1;
								}

								/*fp = fopen("thestrings44.txt","a");
								fprintf(fp,"out %s %d %d %f \n",GlobalWord, i, topcodspot, topcod);
								fclose(fp);*/
							}
						
						

							if(topcodspot == -1) {
								if(ntempcodas[i] == 1) { //repeated above don't need. 
									topcod = 100;
									topcodspot = 0;	
									fp = fopen("topcod1.txt","a");
									fprintf(fp,"%s %s \n",ThisWord, ThisPhon);
									fclose(fp);
								} else if ((strcmp(tempcodas[i][0],"n")) == 0 && (strcmp(tempcodas[i][1],"x")) == 0 || \
										(strcmp(tempcodas[i][0],"n")) == 0 && (strcmp(tempcodas[i][1],"qu")) == 0) {
									topcod = 100;
									topcodspot = 1;
									fp = fopen("topcod2.txt","a");
									fprintf(fp,"%s %s \n",ThisWord, ThisPhon);
									fclose(fp);
								} else if( (strcmp(tempcodas[i][0],"x")) == 0 || (strcmp(tempcodas[i][0],"qu")) == 0) {
									topcod = 100;
									topcodspot = 0;
									fp = fopen("topcod3.txt","a");
									fprintf(fp,"%s %s \n",ThisWord, ThisPhon);
									fclose(fp);
								} else if (ntempcodas[i] > 1) {
									if((strcmp(tempcodas[i][ntempcodas[0] - 1],"r")) == 0 && phononsets[i+1][1] == 'r' || (strcmp(tempcodas[i][ntempcodas[0] - 1],"l")) == 0 && phononsets[i+1][1] == 'l') {
										topcod = 100;
										topcodspot = ntempcodas[0] - 2;
										fp = fopen("topcod4.txt","a");
										fprintf(fp,"%s %d \n",ThisWord, ThisPhon);
										fclose(fp);
									} else {
										fp = fopen("topcod5.txt","a");
										fprintf(fp,"%s %s \n",ThisWord, ThisPhon);
										fclose(fp);
									}	
								} else if (ntempcodas[i] == 0 && ntemponsets[i+1] == 0) {
									topcod = 100;
									fp = fopen("topcod6.txt","a");
									fprintf(fp,"%s %s \n",ThisWord, ThisPhon);
									fclose(fp);
								}			 
							}

							/*if(topcodspot != -1) {
								fp = fopen("thestrings44.txt","a");
								fprintf(fp,"%s ntc=%d nto=%d topcodsp=%d topcod=%f [%d %d %c %d %f] ",ThisWord, ntempcodas[0],ntemponsets[1],topcodspot,topcod, i, j,phononsets[1][0], PhonemesInf[4][topcodgrapheme],GraphemeFreqsInf[4][topcodgrapheme] );
								for(k=0;k<ntempcodas[0];k++) fprintf(fp," %s  ", tempcodas[0][k]);
								fprintf(fp,"\n");
								fclose(fp);
							} else {
								fp = fopen("thestrings55.txt","a");
								fprintf(fp,"%s \n",ThisWord);
								fclose(fp);
							}*/


							if(topcodspot !=  -1 ) {
							//ba.sting
								if( ntemponsets[i+1] == 1 && ntempcodas[i] == 0 || ntemponsets[i+1] == 0 && ntempcodas[i] == 0) {
								//do nothing, Must be split vowel. or n mid consonant.
								} else {

									fp = fopen("TopCod1.txt","a");
									fprintf(fp,"%s  %d %d ",ThisWord, ntempcodas[i], topcodspot);
									for(h=0;h<ntempcodas[0];h++) {
										fprintf(fp,"<%s> ",tempcodas[0][h]);
									}
									for(h=0;h<ntemponsets[1];h++) {
										fprintf(fp,"[%s] ",temponsets[1][h]);
									}
									fprintf(fp,"\n");
									fclose(fp);

									ntemponsets[i+1] = ntempcodas[i] - topcodspot;
									temp = ntempcodas[i];
									ntempcodas[i] = topcodspot;
									for(h=0;h<ntempcodas[0];h++) {
										//for(i=0;i<ntempcodas[0];i++) {
											//strcpy(tempcodas[0][i], tempcodas2[0][i]);
											//tempcodasnum[0][i] = tempcodasnum2[0][i];
										//}
									}
									for(h=0;h<ntemponsets[i+1];h++) {
										strcpy(temponsets[i+1][h],tempcodas[i][h+ntempcodas[i]]);
										temponsetsnum[i+1][h] = tempcodasnum[i][h+ntempcodas[i]];
									}
								}
								CreateOrthString();
								//PrintWord();
							} else {
									fp = fopen("TopCod100.txt","a");
									fprintf(fp,"%s ",ThisWord);
									for(h=0;h<ntempcodas[0];h++) {
										fprintf(fp,"<%s> ",tempcodas[0][h]);
									}
									for(h=0;h<ntemponsets[1];h++) {
										fprintf(fp,"[%s] ",temponsets[1][h]);
									}
									fprintf(fp,"\n");
									fclose(fp);
								

									ToTrain = 0;
									fp = fopen("bads.txt","a");
									fprintf(fp, "%d %s %s %s %d %d %d %d %d %d\n",h,GlobalWord,OrthString,GlobalPhon,ntempvowels,nphonvowels, ntempcodas[0],ntemponsets[1],nphoncodas[0],nphononsets[1]);
									fclose(fp);
							}

						}
					}
				}
			}
		}
	}
	if(i!=3) {
		for(i=ntempvowels;i<4;i++) {
			strcat(NewWord,"+");
			strcat(NewWord,"+");
			strcat(NewWord,"+");
		}
		j=strlen(NewWord);
		NewWord[j-1] = 0;
	}
	
	strcpy(AlignedInputs,NewWord);

	/*fp = fopen("sames.txt","a");	
	fprintf(fp, "%s %s  \n",GlobalWord, NewWord);
	fclose(fp);*/

}

void GraphemeParsing::TestAllSegNetwork()
{
	//this is for graphemes..tries to learn 3 grapheme template with in one.

	int a,h,i,j,k,l=0, spot = 0, codcount, tempspot, tempupto, start_i, used_v = -1, used_c2 = 0, tempmaxplace;
	float inputs2[3000],sum, Delta, tempmax, maxorth,minorth, minavg,totalerror;
	FILE *fp1,*fp2;
	char NewString[100], ChosenGrapheme2[8];
	char TempGlobalWord[100], bit1[30], bit2[30];
	int hasE;

	int  BigNumbers[50];
	int  UptoBig = 0;
	int  HasE = 0;
	int Espot;
	int g;
	char TemplateBit[15];

	char ScanWord[30],ScanInput[3000], ScanBit[50], thegraph[10];
	int ScanCrap, ScanAnswer, ScanNumber, ScanSyll;
	float freqmod;
	char tempname[50];
	


//	if(ENGLISH == 1) fp1 = fopen("./EnglishRes/AllGermanDB.txt","r");
	if(ENGLISH == 1) fp1 = fopen("./EnglishRes/Segpatterns.txt","r");
	else if(ITALIAN == 1) fp1 = fopen("./ItalianRes/Segpatterns.Italian.All.txt","r");
	else if(GERMAN ==1) fp1 = fopen("./GermanRes/Segpatterns.German.txt","r");
	else if(FRENCH==1) fp1 = fopen("./FrenchRes/Segpatterns.French.txt","r");
	
	fp2 = fopen("test6.txt","a");
	fprintf(fp2,"%d %d\n",fp2,fp1);
	fclose(fp2);

	if(fp1==0) exit(1);

	totalerror = 0;


	//for(a=0;a<NSEGMENTATIONS;a++) {
	//fprintf(fp,"%s %s %f %d %d %s %d ",GlobalWord,letterstring,freqmod,insyll[i],strlen(graphemearray[i]) - 1, graphemearray[i],num);

	
	while( (fscanf(fp1,"%s%s%f%d%d%s%d%s",ScanWord,ScanBit,&freqmod,&ScanSyll,&ScanAnswer,thegraph,&ScanNumber,ScanInput)) != EOF) {
		l++;
	//	fscanf(fp1,"%s%s%f%d%d%s%d%s",ScanWord,ScanBit,&freqmod,&ScanCrap,&ScanAnswer,thegraph,&ScanNumber,ScanInput);	
			// 3 IS FOR MEMORY NOT MAX SYLLABLES!!!!!!
		for(h=0;h<(NMARCOLETTERS2 * SPLITMEMORY) + (NLETTERS * ATTENTIONWINDOWSIZE);h++) {	
			if(ScanInput[h] == '0') inputs2[h] = 0;	
			else inputs2[h] = 1;
		}
		//Feedforward
		SplitMaxPos = -1;
		tempmax = -1;
		for(h=0;h<SPLITNETSPOTS;h++) {
			SplitNetSums[h] = 0;
			for(i=0;i< (NMARCOLETTERS2 * SPLITMEMORY) + (NLETTERS * ATTENTIONWINDOWSIZE);i++) {
				if(inputs2[i] > .01) { //Speed
					SplitNetSums[h] += SplitNetWeights[i][h] * inputs2[i];			
				}
			}
			SplitNetActs[h] = ActFunc(SplitNetSums[h]);
			if(SplitNetActs[h] > tempmax) {
				SplitMaxPos = h;
				tempmax = SplitNetActs[h];
			}
			if(SplitNetActs[h] < .05) SplitNetActs[h] = 0;
		}	

		for(h=0;h<SPLITNETSPOTS;h++) SplitNetAnswers[h] = 0;
		SplitNetAnswers[ScanNumber] = 1;
		
		if(ENGLISH == 1) sprintf(tempname,"Testspot.all.%d.txt",TheTrainingCycle);
		else if (ITALIAN == 1) sprintf(tempname,"Testspot5.Italian.%d.txt",TheTrainingCycle);
		else if (GERMAN==1) sprintf(tempname,"Testspot5.German.%d.txt",TheTrainingCycle);
		else if (FRENCH==1)  sprintf(tempname,"Testspot5.French.%d.txt",TheTrainingCycle);
		
		fp2=fopen(tempname,"a");
		fprintf(fp2,"%s %s %d %d %d %s ",ScanWord,ScanBit, ScanSyll, ScanAnswer, ScanNumber,GraphemesList[ScanNumber]);
		tempmax = -1;
		tempmaxplace = 0;

		for(h=0;h<SPLITNETSPOTS;h++) {
			if(SplitNetActs[h] > tempmax) {
				tempmax = SplitNetActs[h];
				tempmaxplace = h;
			}
			//	fprintf(fp2,"%2.2f ",SplitNetActs[h]);
		}
		fprintf(fp2, " %d %s\n",tempmaxplace,GraphemesList[tempmaxplace]);
		fclose(fp2);
		//if(l==100) break;
	}
	fclose(fp1);
}



void GraphemeParsing::TrainSegNetwork(int cycles)
{
	//Train the network
	int i;
	FILE *fp2;

	for(i=0;i<cycles;i++) {
		fp2 = fopen("test5.txt","a");
		fprintf(fp2,"%d \n",i);
		fclose(fp2);

		TheTrainingCycle = i;
		TrainSegmentations();
		TestAllSegNetwork();
		SaveSegmentations();
	}
}


void GraphemeParsing::SaveSegmentations()
{
	int X, Y,h, i,j;
	char names[1000];
	FILE *fp;

	if(ENGLISH == 1) sprintf(names,"EnglishSeg1.%d.txt",TheTrainingCycle);
	else if (ITALIAN == 1) sprintf(names,"ItalianSeg1.%d.txt",TheTrainingCycle);
	else if (GERMAN == 1) sprintf(names,"GermanSeg1.%d.txt",TheTrainingCycle);
	else sprintf(names,"FrenchSeg1.%d.txt",TheTrainingCycle);

	fp = fopen(names,"w");

	for(h=0;h<SPLITNETSPOTS;h++) {
		for(i=0;i<NSEGINPUTS;i++) {
			fprintf(fp,"%f ",SplitNetWeights[i][h]);
		}
	}
	fclose(fp);
}

void GraphemeParsing::LoadSegmentations(FILE *fp, int blank)
{
	int h,i;
	FILE *fp2;

/*	fp2=fopen("loading.txt","a");
	fprintf(fp2,"here \n");
	fclose(fp2);*/

	for(h=0;h<SPLITNETSPOTS;h++) {
		for(i=0;i< NSEGINPUTS;i++) {
			//fscanf(fp,"%f",&SplitNetWeights[i][h]);
			if(blank == 1) {
				SplitNetWeights[i][h] = 0;
			} else {
				fscanf(fp,"%f",&SplitNetWeights[i][h]);
			}
		}
	}
	/*fp2=fopen("loading.txt","a");
	fprintf(fp2,"here2 \n");
	fclose(fp2);*/

}


void GraphemeParsing::FixExceptionsItalian()	
{
#if ITALIAN
	int i,j,k;
	FILE *fp;

//////////////// EXPCEPTIONAL FIXES
	for(i=0;i<ntempvowels;i++) {
		
		
		if(ntemponsets[i] > 0 ) {
			if((strcmp(temponsets[i][0], "sc")) == 0 && temponsets[i][1][0] == 'h') {
				if(phononsets[i][0] == 's' && phononsets[i][1] == 'k' /*&& nphononsets[i] == 2*/) {
					fp = fopen("scwords1.txt","a");	
					fprintf(fp, "%s   \n",GlobalWord);
					fclose(fp);
					for(j=ntemponsets[i]+1; j>1;j--) {
						strcpy(temponsets[i][j],temponsets[i][j-1]);
						temponsetsnum[i][j] = temponsetsnum[i][j-1];
					}
					strcpy(temponsets[i][0],"s");
					strcpy(temponsets[i][1],"ch");
					temponsetsnum[i][0] = NGRAPHEMES + FindLetter(temponsets[i][0][0]);
					temponsetsnum[i][1] = findGrapheme(temponsets[i][1]);
					//ntemponsets[i]++;
				}
			} else if ((strcmp(temponsets[i][0], "sc")) == 0 /*&& ntemponsets[i] == 1*/) {
				if(phononsets[i][0] == 's' && phononsets[i][1] == 'k' && nphononsets[i] > 1) {
					fp = fopen("scwords.txt","a");	
					fprintf(fp, "%s   \n",GlobalWord);
					fclose(fp);
					for(j=ntemponsets[i]+1; j>1;j--) {
						strcpy(temponsets[i][j],temponsets[i][j-1]);
						temponsetsnum[i][j] = temponsetsnum[i][j-1];
					}
					strcpy(temponsets[i][0],"s");
					strcpy(temponsets[i][1],"c");
					temponsetsnum[i][0] = NGRAPHEMES + FindLetter(temponsets[i][0][0]);
					temponsetsnum[i][1] = NGRAPHEMES + FindLetter(temponsets[i][1][0]);
					ntemponsets[i]++;
				}
			
			}
			
		}
	}


	//semi glides /j/ and ia
	for(i=0;i<ntempvowels;i++) {
		if(nphononsets[i] > ntemponsets[i] /*&& 1 == 2*/) {
			if( phononsets[i][nphononsets[i] - 1] == 'w' || phononsets[i][nphononsets[i] - 1] == 'j') {
//				PrintWord();
				
					fp = fopen("semiglides.txt","a");	
					fprintf(fp, "%s   \n",GlobalWord);
					fclose(fp);
				
				if(strlen(tempvowels[i]) == 2) {
					strcpy(temponsets[i][ntemponsets[i]], tempvowels[i]);
					temponsets[i][ntemponsets[i]][1] = 0;
					temponsetsnum[i][ntemponsets[i]] = NGRAPHEMES + FindLetter(tempvowels[i][0]) /*+ NMARCOLETTERS*/;
					ntemponsets[i]++;
					
					
					tempvowelsnum[i] = NGRAPHEMES + FindLetter(tempvowels[i][1]) /*+ NMARCOLETTERS*/;	
					tempvowels[i][0] = tempvowels[i][1];
					tempvowels[i][1] = 0;
										
				
				//	PrintWord();
				}
			}
		}
	}


	RecreateNewWord();
#endif
}


void GraphemeParsing::FixExceptionsFrench()
{
	int i,j,k;
	FILE *fp;
	//redevenir

	//+en++t.i.+è++r.+e+m.ent.// KILL THIS
		if(ntempvowels > 0 && ntempcodas[ntempvowels-1] > 1) {
			if( (ntempcodas[ntempvowels-1] == 2 /* && tempcodas[ntempvowels-1][0][0] == 'n'*/) && \
				((strcmp(tempcodas[ntempvowels-1][0], "en"))== 0) && \
				((strcmp(tempcodas[ntempvowels-1][1],"t")) == 0) ) {

				fp = fopen("ents2.txt","a");	
				fprintf(fp, " %s \n",GlobalWord);
				fclose(fp);

				/*strcpy(tempcodas[ntempvowels-1][0],"ent");
				ntempcodas[ntempvowels-1] = 1;
				tempcodasnum[ntempvowels-1][0] = findGrapheme("ent");	*/		
			}		
		}

		if(ntempvowels > 1) {
			if((strcmp(tempvowels[ntempvowels-1],"e")) == 0 ) {
				if(ntempcodas[ntempvowels-1] == 1 ) {
					if((strcmp(tempcodas[ntempvowels-1][0],"r")) == 0) {
						strcpy(tempvowels[ntempvowels-1],"er");
						ntempcodas[ntempvowels-1] = 0;
						tempvowelsnum[ntempvowels-1] = findGrapheme("er");
						fp = fopen("er2.txt","a");	
						fprintf(fp, " %s \n",GlobalWord);
						fclose(fp);
					}
				}
			}
		}

		if(ntempvowels > 0) {
			if((strcmp(tempvowels[ntempvowels-1],"e")) == 0 ) {
				if(ntempcodas[ntempvowels-1] == 1 ) {
					if((strcmp(tempcodas[ntempvowels-1][0],"z")) == 0) {
						strcpy(tempvowels[ntempvowels-1],"ez");
						ntempcodas[ntempvowels-1] = 0;
						tempvowelsnum[ntempvowels-1] = findGrapheme("ez");
						fp = fopen("ez2.txt","a");	
						fprintf(fp, " %s \n",GlobalWord);
						fclose(fp);
					}
				}
			}
		}


		//	else if((strcmp(tempvowels[h],"aï")) == 0 && (phononsets[h+1][0] == 'j' || phoncodas[h][0] == 'j') && (nphoncodas[h] == 1 || nphononsets[h+1] == 1))splitspot = 1;
		// For some reaswon it isn't picking aI/j combos.
		for(i=0;i<ntempvowels;i++) {
			if((strcmp(tempvowels[i],"aï"))==0) {
				if((phoncodas[i][0] == 'j' && nphoncodas[i] == 1)|| (phononsets[i+1][0] == 'j' && nphononsets[i+1] == 1)) {
					strcpy(tempvowels[i],"a");
					tempvowelsnum[i] = NGRAPHEMES + FindLetter('a');
					for(j=ntempcodas[i];j>0;j--) {
						strcpy(tempcodas[i][j],tempcodas[i][j-1]);
						tempcodasnum[i][j] = tempcodasnum[i][j-1];
					}
					ntempcodas[i]++;
					strcpy(tempcodas[i][0],"ï");
					tempcodasnum[i][0] = NGRAPHEMES + FindLetter('ï');
					fp = fopen("ias.2.txt","a");	
					fprintf(fp, " %s \n",GlobalWord);
					fclose(fp);
					PrintWord();
				}
			}
		}

	if(ntempvowels > 1) { /* SHOULD NOT GET HERE ANYMORE */
		if(phononsets[ntempvowels-1][0] == 'm' && phonvowels[ntempvowels-1] == '@') {
			if((strcmp(tempcodas[ntempvowels-1][0],"m"))==0) {
			//	fp = fopen("mentfix.txt","a");	
				if((strcmp(tempcodas[ntempvowels-1][1],"ent"))==0) { 
					if((strcmp(tempvowels[ntempvowels-1],"e"))==0) {
						strcpy(tempcodas[ntempvowels-2][ntempcodas[ntempvowels-2]],temponsets[ntempvowels-1][0]);
						tempcodasnum[ntempvowels-2][0] = temponsetsnum[ntempvowels-1][0];
						ntempcodas[ntempvowels-2]++;
						strcpy(tempcodas[ntempvowels-2][ntempcodas[ntempvowels-2]],"e");
						tempcodasnum[ntempvowels-2][ntempcodas[ntempvowels-2]] = NGRAPHEMES + FindLetter('e');
						ntempcodas[ntempvowels-2]++;
						strcpy(temponsets[ntempvowels-1][0],"m");
						temponsetsnum[ntempvowels-1][0] = NGRAPHEMES + FindLetter('m');
						ntemponsets[ntempvowels-1] = 1;
						//strcpy(tempvowels[ntempvowels-1],"ent");
						//tempvowelsnum[ntempvowels-1] = findGrapheme("ent");
						//ntempcodas[ntempvowels-1] = 0;
						strcpy(tempvowels[ntempvowels-1],"en");
						tempvowelsnum[ntempvowels-1] = findGrapheme("en");
						strcpy(tempcodas[ntempvowels-1 ][0],"t");
						tempcodasnum[ntempvowels-1][0] = NGRAPHEMES + FindLetter('t');
						ntempcodas[ntempvowels-1] = 1;

						fp = fopen("mentfix.txt","a");	
						fprintf(fp, "4 %s \n",GlobalWord);
						fclose(fp);
						PrintWord();
					}
				}
			}
		}
	}

	if( (strcmp(tempvowels[ntempvowels-1],"aie")) == 0) {
		if(ntempcodas[ntempvowels-1] == 2) {
			if( (strcmp(tempcodas[ntempvowels-1][0],"n")) == 0 &&(strcmp(tempcodas[ntempvowels-1][0],"n")) == 0) {
				strcpy(tempvowels[ntempvowels-1],"ai");
				tempvowelsnum[ntempvowels-1] = findGrapheme("ai");
				strcpy(tempcodas[ntempvowels-1][0],"en");
				tempcodasnum[ntempvowels-1][0] = findGrapheme("en");
				strcpy(tempcodas[ntempvowels-1][1],"t");
				tempcodasnum[ntempvowels-1][1] = NGRAPHEMES + FindLetter('t');
				ntempcodas[ntempvowels-1] = 2;
				fp = fopen("aiefix.txt","a");	
				fprintf(fp, "4 %s \n",GlobalWord);
				fclose(fp);
			}
		}
	}
	if(/*ntempvowels > 1*/ 1==1) {
			if((strcmp(tempvowels[ntempvowels-1],"e")) == 0 ) {
				if(ntempcodas[ntempvowels-1] == 2) {
					if((strcmp(tempcodas[ntempvowels-1][0],"r")) == 0 && (strcmp(tempcodas[ntempvowels-1][1],"s")) == 0 ) {
						if((strcmp(temponsets[ntempvowels-1][ntemponsets[ntempvowels-1]-1],"i")) == 0) {
							strcpy(tempvowels[ntempvowels-1],"er");
							strcpy(tempcodas[ntempvowels-1][0],"s");
							tempcodasnum[ntempvowels-1][0] = NGRAPHEMES + FindLetter('s');
							ntempcodas[ntempvowels-1] = 1;
							tempvowelsnum[ntempvowels-1] = findGrapheme("er");
							fp = fopen("iers2.txt","a");	
							fprintf(fp, " %s \n",GlobalWord);
							fclose(fp);
						}
					}
				}
			}
	}
	//remercient	r.e.m.+e++r.c.+i+++en+t.+  r.e.m.+u+++en+t.++++
	//re.merc.i.ent, 
	

}

void GraphemeParsing::FixExceptions()
{
#if ENGLISH
//PrintWord();
	int i,j,k;
	FILE *fp;

	//final sm.

		//embezzler, able
	if(ntempvowels > 1 && ntempvowels + 1 == nphonvowels) {
		for(i=0;i<ntempvowels;i++) {
		}
	}
	for(i=0;i<ntempvowels-1;i++) {	
		if(((strcmp(tempvowels[i],"ir")) == 0) && ((strcmp(tempvowels[i+1],"e")) == 0)) {
			if(ntemponsets[i+1] == 0 && phonvowels[i+1] == '@') {
					strcpy(tempvowels[i],"i");
					strcpy(tempvowels[i+1],"re");
					tempvowelsnum[i] = NGRAPHEMES + FindLetter(tempvowels[i][0]);
					tempvowelsnum[i+1] = findGrapheme(tempvowels[i+1]);
					fp = fopen("irewords.txt","a");	
					fprintf(fp, "%s   \n",GlobalWord);
					fclose(fp);
			}
		}
	}
#endif
}

void GraphemeParsing::TrainSegmentations()
{
	//this is for graphemes..tries to learn 3 grapheme template with in one.

	int a,h,i,j,k, spot = 0, codcount, tempspot, tempupto, start_i, used_v = -1, used_c2 = 0, tempmaxplace;
	float inputs2[5000],sum, Delta, tempmax, maxorth,minorth, minavg,totalerror;
	FILE *fp1,*fp2;
	char NewString[100], ChosenGrapheme2[8];
	char TempGlobalWord[100], bit1[30], bit2[30];
	int hasE, count =0;

	int  BigNumbers[50];
	int  UptoBig = 0;
	int  HasE = 0;
	int Espot;
	int g;
	char TemplateBit[15];

	char ScanWord[30],ScanInput[5000], ScanBit[50], thegraph[10];
	int ScanCrap, ScanAnswer, ScanNumber;
	float freqmod;
	char tempname[50];
	

	if(ENGLISH == 1) fp1 = fopen("./EnglishRes/Segpatterns.txt","r");
	else if(ITALIAN == 1) fp1 = fopen("./ItalianRes/Segpatterns.Italian.txt","r");
	else if(GERMAN ==1)  fp1 = fopen("./GermanRes/Segpatterns.German.txt","r");
	else if(FRENCH == 1)  fp1 = fopen("./FrenchRes/Segpatterns.French.txt","r");

	fp2 = fopen("test6.txt","a");
	fprintf(fp2,"%d\n",fp1);
	fclose(fp2);

	if(fp1==0) exit(1);

	totalerror = 0;


	//for(a=0;a<NSEGMENTATIONS;a++) {
	//	fprintf(fp,"%s %s %f %d %d %s %d ",GlobalWord,letterstring,freqmod,insyll[i],strlen(graphemearray[i]) - 1, graphemearray[i],num);

	
	while( (fscanf(fp1,"%s%s%f%d%d%s%d%s",ScanWord,ScanBit,&freqmod,&ScanCrap,&ScanAnswer,thegraph,&ScanNumber,ScanInput)) != EOF) {
		count++;
		if(count%10000 == 0) {
			fp2 = fopen("test6.txt","a");
			fprintf(fp2,"%s %f %d\n",ScanWord, freqmod, ScanNumber);
			//fprintf(fp2,"sw=%s sb=%s fm=%f sc=%d sa=%, tg=%s sn=%d si=%s %d %d \n",ScanWord,ScanBit,freqmod,ScanCrap,ScanAnswer,thegraph,ScanNumber,ScanInput, strlen(ScanInput),(NMARCOLETTERS2 * SPLITMEMORY) + (NLETTERS * ATTENTIONWINDOWSIZE) ) ;	
			fclose(fp2);	
		}


		//	fscanf(fp1,"%s%s%f%d%d%s%d%s",ScanWord,ScanBit,&freqmod,&ScanCrap,&ScanAnswer,thegraph,&ScanNumber,ScanInput,(NMARCOLETTERS2 * SPLITMEMORY) + (NLETTERS * ATTENTIONWINDOWSIZE));
	
			// 3 IS FOR MEMORY NOT MAX SYLLABLES!!!!!!
		for(h=0;h<(NMARCOLETTERS2 * SPLITMEMORY) + (NLETTERS * ATTENTIONWINDOWSIZE);h++) {	
			if(ScanInput[h] == '0') inputs2[h] = 0;	
			else inputs2[h] = 1;
		}
		//Feedforward
		SplitMaxPos = -1;
		tempmax = -1;
		for(h=0;h<SPLITNETSPOTS;h++) {
			SplitNetSums[h] = 0;
			for(i=0;i< (NMARCOLETTERS2 * SPLITMEMORY) + (NLETTERS * ATTENTIONWINDOWSIZE);i++) {
				if(inputs2[i] > .01) { //Speed
					SplitNetSums[h] += SplitNetWeights[i][h] * inputs2[i];			
				}
			}
			SplitNetActs[h] = ActFunc(SplitNetSums[h]);
			if(SplitNetActs[h] > tempmax) {
				SplitMaxPos = h;
				tempmax = SplitNetActs[h];
			}
			if(SplitNetActs[h] < .05) SplitNetActs[h] = 0;
		}	



		
		for(h=0;h<SPLITNETSPOTS;h++) SplitNetAnswers[h] = 0;
		SplitNetAnswers[ScanNumber] = 1;

		for(h=0;h<SPLITNETSPOTS;h++) {
			SplitNetErrors[h] = SplitNetAnswers[h] - SplitNetActs[h];
			totalerror += abs(SplitNetErrors[h]);
		}

		for(h=0;h<SPLITNETSPOTS;h++) {
			for(i=0;i<(NMARCOLETTERS2 * SPLITMEMORY) + (NLETTERS * ATTENTIONWINDOWSIZE); i++) {	
				if(inputs2[i] > .05) {
					Delta = freqmod * .05 * SplitNetErrors[h] * inputs2[i];	
					if(Delta > 1) {
						fp1=fopen("shitty.txt","w");
						fprintf(fp1," %s %d %d fm=%f %f %f %f %f %f\n",ScanWord, h,i,  freqmod, Delta, inputs2[i], SplitNetActs[h],SplitNetErrors[h], SplitNetAnswers[h]);fclose(fp1);
						fprintf(fp1,"sw=%s sb=%s fm=%f sc=%d sa=%d tg=%s sn=%d si=%s %d %d \n",ScanWord,ScanBit,freqmod,ScanCrap,ScanAnswer,thegraph,ScanNumber,ScanInput, strlen(ScanInput),(NMARCOLETTERS2 * SPLITMEMORY) + (NLETTERS * ATTENTIONWINDOWSIZE) ) ;	
						exit(1);
					}
					SplitNetWeights[i][h] += Delta;
				}
			}
		}
	}
	fclose(fp1);
	//TestAllSegNetwork();
}

void GraphemeParsing::FixEarlyExceptions()
{
#if ENGLISH
	int a,h,i,j,k, l, LetterSpot, ecount;
	char theletter;
	FILE *fp, *fp1;


	 //PrintWordName("BeforeChops111.txt");
			
		/*fp = fopen("middle-e.11111.txt","a");
				fprintf(fp,"out %s %d %d %d\n",GlobalWord, ntempvowels,nphonvowels,ecount);
				fclose(fp);*/
		


//appliance, acknowledgemenet, grotesqueness,commencement
//if(ntempvowels == 3) {	

	//2 @ r pattern
	//virus v2@rVs
	//berated be.ra.ted, derag
	if(nphonvowels > 2) {
		for(i=0;i<nphonvowels-2;i++) {	
			if(phonvowels[i] == '2' && phonvowels[i+1] == '@' && nphononsets[i+1] == 0 && \
				nphoncodas[i] == 0 && phononsets[i+2][0] == 'r') {
			
				for(j=i+1;j<nphonvowels-1;j++) {
					for(k=0;k<nphononsets[j+1];k++){
						phononsets[j][k] = phononsets[j+1][k];
					}
					nphononsets[j] = nphononsets[j+1];
					phonvowels[j] = phonvowels[j+1];
					for(k=0;k<nphoncodas[j+1];k++) {
						phoncodas[j][k] = phoncodas[j+1][k]; 	
					}
					nphoncodas[j] = nphoncodas[j+1];
				}
				nphonvowels--;

				fp1 = fopen("phoncorrect.txt","a");
				fprintf(fp1,"%d %s %c %c %d %d %c\n",i,GlobalWord,phonvowels[i],phonvowels[i+1],nphoncodas[i],nphononsets[i+1],phononsets[i+1][0]);
				fclose(fp1);

				break;
			}
		}
	}

	 if(ntempvowels > 1) {		
		if( ((strcmp(tempcodas[ntempvowels-2][ntempcodas[ntempvowels-2]-1],"l")) == 0) && \
				phonvowels[ntempvowels-1] == '@' && \
				(strcmp(tempvowels[ntempvowels-1],"e")) == 0&& \
				ntempcodas[ntempvowels-2]>1 && ntempcodas[ntempvowels-1] < 2) { 

				fp1 = fopen("same.schwa.txt","a");
				fprintf(fp1,"%s %d\n",GlobalWord,ntempcodas[ntempvowels-1]);
				fclose(fp1);
			
				for(i=0;i<ntempcodas[ntempvowels-1];i++) {
					strcpy(tempcodas[ntempvowels-1][i+2],tempcodas[ntempvowels-1][i]);
					tempcodasnum[ntempvowels-1][i+2] = tempcodasnum[ntempvowels-1][i];
				}
				strcpy(tempcodas[ntempvowels-1][0],"l");
				tempcodasnum[ntempvowels-1][0]= NONSETS + NCODAS + NVOWELS + FindLetter('l');
				strcpy(tempcodas[ntempvowels-1][1],"e");
				tempcodasnum[ntempvowels-1][1]= NONSETS + NCODAS + NVOWELS + FindLetter('e');

				strcpy(tempvowels[ntempvowels-1],"*");
				tempvowelsnum[ntempvowels-1] = -1;
					
				ntempcodas[ntempvowels-1] += 2;
				ntempcodas[ntempvowels-2] -= 1;	
		}
	}

		//axolotl 
//cuddling
	
	if(/* ntempvowels + 1 == nphonvowels && */ ntempvowels > 1) {		
		for(i=0;i<ntempvowels;i++) {	
			
	//fp1 = fopen("dif.schwa.txt","a");
	//fprintf(fp1,"%d %d %s %d %d %c %c %c\n",i, i - (ntempvowels+1-nphonvowels), GlobalWord,ntempvowels,ntempcodas[0],phonvowels[i /*- (ntempvowels-nphonvowels)*/+1],phononsets[i /*- (ntempvowels-nphonvowels)*/ + 2][0],phononsets[i /*- (ntempvowels-nphonvowels)*/ + 1][0]);
	//fclose(fp1);
			
		//a.bler. a.bl@. cuddle
			if(ntempcodas[i] > 0) {
				if( ((strcmp(tempcodas[i][ntempcodas[i]-1],"l")) == 0) && \
					phonvowels[i /*- (ntempvowels-nphonvowels)*/+1] == '@' && \
					(nphoncodas[i+1] > 0 && phoncodas[i+1][0] == 'l') && \
					//(nphoncodas[i - (ntempvowels+1-nphonvowels)] > 0 && phoncodas[i - (ntempvowels+1-nphonvowels) +1][0] == 'l') && \

					ntempcodas[i]>1 ) { 
	
					fp1 = fopen("dif.schwa.txt","a");
					fprintf(fp1,"->%s %d  \n",GlobalWord,i );
					fclose(fp1);


						//ablution.
	
					//cudd@l.ing // appl.e.jack. app.*.l.e.jack cobb.lest.one
					for(j=ntempvowels;j>i;j--) {
						for(k=0;k<ntempcodas[j-1];k++) {
							strcpy(tempcodas[j][k],tempcodas[j-1][k]);
							tempcodasnum[j][k] = tempcodasnum[j-1][k];
						}
						ntempcodas[j] = ntempcodas[j-1];
						strcpy(tempvowels[j],tempvowels[j-1]);
						tempvowelsnum[j] = tempvowelsnum[j-1];
					}
					strcpy(tempvowels[i+1],"*");
					tempvowelsnum[i+1] = -1;

					strcpy(tempcodas[i+1][0],"l");
					tempcodasnum[i+1][0]= NONSETS + NCODAS + NVOWELS + FindLetter('l');
					ntempcodas[i+1] = 1;
			
					ntempcodas[i]--;
					ntempvowels++;
					//PrintWord();
					break;
				}
			}
		}
	}
			//cooperated. co.op.er.a.t.e.d. berated, derange. 
	//coo.p.er.a.t.e.d
	/*fp1 = fopen("threeE.0.txt","a");
	fprintf(fp1,"%s %d %d\n",GlobalWord,ntempcodas[ntempvowels-1],nphoncodas[nphonvowels-1]);
	fclose(fp1);*/

	if( ntempvowels > nphonvowels && \
		ntempvowels > 1 || \
		(((nphonvowels > 1 && strcmp(tempvowels[ntempvowels-1],"e")) == 0 /*&& (ntempcodas[ntempvowels-2] == 1) *//*&& strlen(tempcodas[ntempvowels-2][0]) == 1*/) && \
		ntempcodas[ntempvowels-1] == 0 && nphoncodas[nphonvowels-1] > 0)  || \
		(((nphonvowels > 1 && strcmp(tempvowels[ntempvowels-1],"e")) == 0 /*&& (ntempcodas[ntempvowels-2] == 1)*/ /*&& strlen(tempcodas[ntempvowels-2][0]) == 1*/)  && \
		ntempcodas[ntempvowels-1] == 1 && nphoncodas[nphonvowels-1] > 1)  ) {
			i = ntempvowels;
			if( (((strcmp(tempvowels[ntempvowels-1],"e")) == 0) && ntempcodas[ntempvowels-1] == 0) || \
				(((strcmp(tempvowels[ntempvowels-1],"ue")) == 0) && ntempcodas[ntempvowels-1] == 0)   ) {
		
				fp1 = fopen("threeE.0.txt","a");
				fprintf(fp1,"%s \n",GlobalWord);
				fclose(fp1);
				LetterSpot = FindLetter('e');			
				//tempcodas[ntempvowels - 2][ntempcodas[ntempvowels - 2]][0] = 'e' ;
				//tempcodas[ntempvowels - 2][ntempcodas[ntempvowels - 2]][1] = 0;
				strcpy(tempcodas[ntempvowels-2][ntempcodas[ntempvowels-2]],tempvowels[ntempvowels-1]);
				tempcodasnum[ntempvowels-2][ntempcodas[ntempvowels-2]] = tempvowelsnum[ntempvowels-1];
				if( (strcmp(tempvowels[ntempvowels-1],"e")) == 0) {
					tempcodasnum[ntempvowels - 2][ntempcodas[ntempvowels - 2]] = NONSETS+NVOWELS+NCODAS + LetterSpot; 
				} else {
					tempcodasnum[ntempvowels - 2][ntempcodas[ntempvowels - 2]] = findGrapheme("ue"); 
				}
				ntempcodas[ntempvowels - 2]++;
				tempvowels[ntempvowels-1][0] = 0;
				NOrthSylls--;
				ntempvowels--;
			} else if ( ((strcmp(tempvowels[ntempvowels-1],"e") == 0) && ntempcodas[ntempvowels-1] == 1 && ( (tempcodas[ntempvowels-1][0][0] == 's' || tempcodas[ntempvowels-1][0][0] == 'd')) && strlen(tempcodas[ntempvowels-1][0]) == 1) || \
						(strcmp(tempvowels[ntempvowels-1],"ue") == 0) && ntempcodas[ntempvowels-1] == 1 && ( (tempcodas[ntempvowels-1][0][0] == 's' || tempcodas[ntempvowels-1][0][0] == 'd')) && strlen(tempcodas[ntempvowels-1][0]) == 1) {
			
				fp1 = fopen("threeE.1.txt","a");
				fprintf(fp1,"%s \n",GlobalWord);
				fclose(fp1);
				
				LetterSpot = FindLetter('e');					
				
				strcpy(tempcodas[ntempvowels-2][ntempcodas[ntempvowels-2]],tempvowels[ntempvowels-1]);
				tempcodasnum[ntempvowels-2][ntempcodas[ntempvowels-2]] = tempvowelsnum[ntempvowels-1];

				if( (strcmp(tempvowels[ntempvowels-1],"e")) == 0) {
					tempcodasnum[ntempvowels - 2][ntempcodas[ntempvowels - 2]] = NONSETS+NVOWELS+NCODAS + LetterSpot; 
				} else {
					tempcodasnum[ntempvowels - 2][ntempcodas[ntempvowels - 2]] = findGrapheme("ue"); 
				}		
				ntempcodas[ntempvowels - 2]++;

				strcpy(tempcodas[ntempvowels - 2][ntempcodas[ntempvowels - 2]],tempcodas[ntempvowels-1][0]);
				tempcodasnum[ntempvowels - 2][ntempcodas[ntempvowels - 2]] = NONSETS+NVOWELS+NCODAS + FindLetter(tempcodas[ntempvowels-1][0][0]); 
				ntempcodas[ntempvowels - 2]++;

				NOrthSylls--;
				ntempvowels--;
			}
	}

	if(ntempvowels==5 || ntempvowels == 6 && InTraining == 1) {
		fp1 = fopen("Testvowelsdisc2.txt","a");
		fprintf(fp1,"%s %s\n",GlobalWord, tempvowels[1]);
		fclose(fp1);
		ToTrain = 3;
	}


	//if(  ( ntempvowels == 2 && nphonvowels == 1)  ) { 
	/*if(  ( ntempvowels == nphonvowels + 1)  ) { 
		if( ((strcmp(tempvowels[ntempvowels-1],"e")) == 0 || (strcmp(tempvowels[ntempvowels-1],"ue")) == 0) \
			&& ntempcodas[ntempvowels-1] == 0) {
		
			fp1 = fopen("finE.txt","a");
			fprintf(fp1,"%s \n",GlobalWord);
			fclose(fp1);				
			
				strcpy(tempcodas[ntempvowels-2][ntempcodas[ntempvowels-2]], tempvowels[ntempvowels-1]);
				tempcodasnum[ntempvowels-2][ntempcodas[ntempvowels-2]] = tempvowelsnum[ntempvowels-1] + NMARCOLETTERS;
				ntempcodas[ntempvowels-2]++;
				ntempvowels--;

		} else if( ntempcodas[ntempvowels-1] == 1 && \
				( (strcmp(tempvowels[ntempvowels-1], "e")) == 0 || (strcmp(tempvowels[ntempvowels-1],"ue")) == 0) && \
				( (strcmp(tempcodas[ntempvowels-1][ntempcodas[ntempvowels-1]-1],"d")) == 0 || (strcmp(tempcodas[ntempvowels-1][ntempcodas[ntempvowels-1]-1],"s")) == 0) )  {											
					fp1 = fopen("finE1.txt","a");
					fprintf(fp1,"%s \n",GlobalWord);
					fclose(fp1);	
				
				l = ntempcodas[ntempvowels-2];
			
				strcpy(tempcodas[ntempvowels-2][l], tempvowels[ntempvowels-1]);
				tempcodasnum[ntempvowels-2][l] = tempvowelsnum[ntempvowels-1] + NMARCOLETTERS; //turn it into a coda
					
				if((strcmp(tempcodas[ntempvowels][ntempcodas[ntempvowels]-2],"d")) == 0) strcpy(tempcodas[ntempvowels-1][l+1],"d");
				else strcpy(tempcodas[ntempvowels-2][l+1],"s");
	
				tempcodasnum[ntempvowels-2][l+1] = tempcodasnum[ntempvowels-1][0];
							
				ntempcodas[ntempvowels-2] += 2;
				ntempcodas[ntempvowels-1] = 0;
				ntempvowels--;	
		} 
	}*/
	
	

//	if(  ( ntempvowels == 1 && nphonvowels == 2)  ) {



	// sm words
	if(/*ntempvowels > 1*/ /*&& ntempvowels + 1 <= nphonvowels*/1==1) {
		if(ntempcodas[ntempvowels-1] == 3 || ntempcodas[ntempvowels-1] == 2) {
			if( (strcmp(tempcodas[ntempvowels-1][0],"s")) == 0 && (strcmp(tempcodas[ntempvowels-1][1],"m")) == 0) {
				fp = fopen("sms.txt","a");	
				fprintf(fp, "-->%s  %d \n",GlobalWord,ntempcodas[ntempvowels-1]);
				fclose(fp);

				strcpy(tempvowels[ntempvowels],"*");				
				tempvowelsnum[ntempvowels] = -1;

				strcpy(tempcodas[ntempvowels-1][0],"s");
				tempcodasnum[ntempvowels-1][0] = NGRAPHEMES + FindLetter('s');
		
				for(i=1;i<ntempcodas[ntempvowels-1];i++) {
					strcpy(tempcodas[ntempvowels][i-1],tempcodas[ntempvowels-1][i]);
					tempcodasnum[ntempvowels][i-1] = tempcodasnum[ntempvowels-1][i];
				}
				ntempcodas[ntempvowels] = ntempcodas[ntempvowels-1] - 1;
				ntempcodas[ntempvowels-1] = 1;	
				ntempvowels++;
			}
		}
	}

	//brittlest. candlepower
	//PrintWord();
	
	//move further syllables out?
	for(h=0;h<ntempvowels;h++) {
		if(  ( ntempvowels + 1 <= nphonvowels)  ) {	
			fp1 = fopen("triple.txt","a");
			fprintf(fp1,"%s %d %d %s %s\n",GlobalWord,ntempcodas[0],ntemponsets[1], tempcodas[0],temponsets[1]);
			fclose(fp1);
			//iest
			if( (strcmp(tempvowels[h],"ei")) == 0 && strcmp(tempcodas[h][0],"r") != 0 || \
				(strcmp(tempvowels[h],"ie")) == 0 && strcmp(tempcodas[h][0],"r") != 0 || \
				(strcmp(tempvowels[h],"oi")) == 0 && strcmp(tempcodas[h][0],"r") != 0 || \
				(strcmp(tempvowels[h],"ye")) == 0 && strcmp(tempcodas[h][0],"r") != 0 || \
				(strcmp(tempvowels[h],"ui")) == 0 && strcmp(tempcodas[h][0],"r") != 0 || \
				(strcmp(tempvowels[h],"ua")) == 0 && strcmp(tempcodas[h][0],"r") != 0 || \
				(strcmp(tempvowels[h],"ee")) == 0 && strcmp(tempcodas[h][0],"r") != 0 || \
				(strcmp(tempvowels[h],"oe")) == 0 && strcmp(tempcodas[h][0],"r") != 0 || \
				(strcmp(tempvowels[h],"ue")) == 0 && strcmp(tempcodas[h][0],"r") != 0 || \
				(strcmp(tempvowels[h],"io")) == 0 && strcmp(tempcodas[h][0],"r") != 0 || \
				(strcmp(tempvowels[h],"ia")) == 0 && strcmp(tempcodas[h][0],"r") != 0 || \
				(strcmp(tempvowels[h],"ae")) == 0 && strcmp(tempcodas[h][0],"r") != 0 || \
				(strcmp(tempvowels[h],"ai")) == 0 && strcmp(tempcodas[h][0],"r") != 0 || \
				(strcmp(tempvowels[h],"ea")) == 0 && strcmp(tempcodas[h][0],"r") != 0 || \
				(strcmp(tempvowels[h],"oa")) == 0 && strcmp(tempcodas[h][0],"r") != 0 || \
				(strcmp(tempvowels[h],"uo")) == 0 && strcmp(tempcodas[h][0],"r") != 0 || \
				(strcmp(tempvowels[h],"eo")) == 0 && strcmp(tempcodas[h][0],"r") != 0 || \
				(strcmp(tempvowels[h],"iu")) == 0 && strcmp(tempcodas[h][0],"r") != 0 || \
				(strcmp(tempvowels[h],"eu")) == 0 && strcmp(tempcodas[h][0],"r") != 0 || \
				(strcmp(tempvowels[h],"oo")) == 0 && strcmp(tempcodas[h][0],"r") != 0 || \
				(strcmp(tempvowels[h],"ou")) == 0 && strcmp(tempcodas[h][0],"r") != 0  ) {
				
					//iest
					if(ntempcodas[ntempvowels-1] == 2 && \
						((strcmp(tempcodas[ntempvowels-1][0],"s")) == 0) && \
						((strcmp(tempcodas[ntempvowels-1][1],"t")) == 0) && \
						((strcmp(tempvowels[ntempvowels-1],"ie")) == 0)  ) {
					
						h = ntempvowels -1;
					}
					//final -e appliance alieante
				//tru.ingnessful
				for(i=ntempvowels;i>= h+2; i--) {
					for(j=0;j<ntempcodas[i-1];j++) {
						strcpy(tempcodas[i][j],tempcodas[i-1][j]);
						tempcodasnum[i][j] = tempcodasnum[i-1][j];
					}
					ntempcodas[i] = ntempcodas[i-1];
					strcpy(tempvowels[i],tempvowels[i-1]);
					tempvowelsnum[i] = tempvowelsnum[i-1];
				}

				for(i=0;i<ntempcodas[h];i++) {
					strcpy(tempcodas[h+1][i],tempcodas[h][i]);
					tempcodasnum[h+1][i] = tempcodasnum[h][i];		
				}
				ntempcodas[h+1] = ntempcodas[h];
				ntempcodas[h] = 0;
	
				tempvowels[h+1][0] = tempvowels[h][1];
				tempvowels[h+1][1] = 0;
				tempvowels[h][1] = 0;
				tempvowelsnum[h] = NONSETS + NCODAS + NVOWELS + FindLetter(tempvowels[h][0]);
				tempvowelsnum[h+1] = NONSETS + NCODAS + NVOWELS + FindLetter(tempvowels[h+1][0]);
				ntempvowels++;

				
				fp1 = fopen("SplitVowel.1.txt","a");
				fprintf(fp1,"%s %d %d %d\n",GlobalWord, ntempvowels,ntempcodas[0],ntempcodas[1]);
				fclose(fp1);
			//	PrintWord();
				break;
			}
			if( (strcmp(tempvowels[h],"ei")) == 0 && strcmp(tempcodas[h][0],"r") == 0 || \
				(strcmp(tempvowels[h],"ie")) == 0 && strcmp(tempcodas[h][0],"r") == 0 || \
				(strcmp(tempvowels[h],"oi")) == 0 && strcmp(tempcodas[h][0],"r") == 0 || \
				(strcmp(tempvowels[h],"ye")) == 0 && strcmp(tempcodas[h][0],"r") == 0 || \
				(strcmp(tempvowels[h],"ui")) == 0 && strcmp(tempcodas[h][0],"r") == 0 || \
				(strcmp(tempvowels[h],"ua")) == 0 && strcmp(tempcodas[h][0],"r") == 0 || \
				(strcmp(tempvowels[h],"ee")) == 0 && strcmp(tempcodas[h][0],"r") == 0 || \
				(strcmp(tempvowels[h],"oe")) == 0 && strcmp(tempcodas[h][0],"r") == 0 || \
				(strcmp(tempvowels[h],"ue")) == 0 && strcmp(tempcodas[h][0],"r") == 0 || \
				(strcmp(tempvowels[h],"io")) == 0 && strcmp(tempcodas[h][0],"r") == 0 || \
				(strcmp(tempvowels[h],"ou")) == 0 && strcmp(tempcodas[h][0],"r") == 0 || \
				(strcmp(tempvowels[h],"ia")) == 0 && strcmp(tempcodas[h][0],"r") == 0 || \
				(strcmp(tempvowels[h],"uo")) == 0 && strcmp(tempcodas[h][0],"r") == 0 || \
				(strcmp(tempvowels[h],"eo")) == 0 && strcmp(tempcodas[h][0],"r") == 0 || \
				(strcmp(tempvowels[h],"iu")) == 0 && strcmp(tempcodas[h][0],"r") == 0 || \
				(strcmp(tempvowels[h],"oo")) == 0 && strcmp(tempcodas[h][0],"r") == 0 || \
				(strcmp(tempvowels[h],"ae")) == 0 && strcmp(tempcodas[h][0],"r") == 0  ) {

				//tru.ingnessful
				for(i=ntempvowels;i>= h+2; i--) {
					for(j=0;j<ntempcodas[i-1];j++) {
						strcpy(tempcodas[i][j],tempcodas[i-1][j]);
						tempcodasnum[i][j] = tempcodasnum[i-1][j];
					}
					ntempcodas[i] = ntempcodas[i-1];
					strcpy(tempvowels[i],tempvowels[i-1]);
					tempvowelsnum[i] = tempvowelsnum[i-1];
				}


				tempvowels[h+1][0] = tempvowels[h][1];
				tempvowels[h+1][1] = 'r';
				tempvowels[h+1][2] = 0;
				tempvowels[h][1] = 0;
				tempvowelsnum[h] = NONSETS + NCODAS + NVOWELS + FindLetter(tempvowels[h][0]);
				tempvowelsnum[h+1] = findGrapheme(tempvowels[h+1]);

				for(i=1;i<ntempcodas[h];i++) {
					strcpy(tempcodas[h+1][i-1],tempcodas[h][i]);
					tempcodasnum[h+1][i-1] = tempcodasnum[h][i];
				}
				ntempcodas[h+1] = ntempcodas[h] - 1;
				ntempcodas[h] = 0;
				ntempvowels++;
	

				fp1 = fopen("SplitVowel.2.txt","a");
				fprintf(fp1,"111 %s %d\n",GlobalWord,tempvowelsnum[h+1]);
				fclose(fp1);
				break;
			}
			if( (strcmp(tempvowels[h],"eir")) == 0 || \
				(strcmp(tempvowels[h],"ier")) == 0 || \
				(strcmp(tempvowels[h],"oar")) == 0 || \
				(strcmp(tempvowels[h],"oor")) == 0 || \
				(strcmp(tempvowels[h],"our")) == 0 || \
				(strcmp(tempvowels[h],"ear")) == 0 || \
				(strcmp(tempvowels[h],"uar")) == 0 	) {
			
				//tru.ingnessful
				for(i=ntempvowels;i>= h+2; i--) {
					for(j=0;j<ntempcodas[i-1];j++) {
						strcpy(tempcodas[i][j],tempcodas[i-1][j]);
						tempcodasnum[i][j] = tempcodasnum[i-1][j];
					}
					ntempcodas[i] = ntempcodas[i-1];
					strcpy(tempvowels[i],tempvowels[i-1]);
					tempvowelsnum[i] = tempvowelsnum[i-1];
				}		
				
				for(i=0;i<ntempcodas[h];i++) {
					strcpy(tempcodas[h+1][i],tempcodas[h][i]);
					tempcodasnum[h+1][i] = tempcodasnum[h][i];
				}
				ntempcodas[h+1] = ntempcodas[h];	
				ntempcodas[h] = 0;

				tempvowels[h+1][0] = tempvowels[h][1];
				tempvowels[h+1][1] = 'r';
				tempvowels[h+1][2] = 0;
				tempvowels[h][1] = 0;
				tempvowelsnum[h] = NONSETS + NCODAS + NVOWELS + FindLetter(tempvowels[h][0]);
				tempvowelsnum[h+1] = findGrapheme(tempvowels[1]);

				tempvowelsnum[h] = NONSETS + NCODAS + NVOWELS + FindLetter(tempvowels[h][0]);
				tempvowelsnum[h+1] = findGrapheme(tempvowels[h+1]);
			
				ntempvowels++;
				fp1 = fopen("SplitVowel.3.txt","a");
				fprintf(fp1,"%s \n",GlobalWord);
				fclose(fp1);
				break;
			}
		}
	}

/*	if(ntempvowels == nphonvowels && ntempvowels > 1) {
		if( (strcmp(tempvowels[ntempvowels-1],"e")) == 0 && \
			(strcmp(tempcodas[ntempvowels-2][ntempcodas[ntempvowels-2]-1],"l")) == 0 && \
			((ntempcodas[ntempvowels-1] == 0) || \
			(ntempcodas[ntempvowels-1] == 1 && ((strcmp(tempcodas[ntempvowels-1][0],"d")) == 0 || (strcmp(tempcodas[ntempvowels-1][0],"s")) == 0)) )) {
		

			fp = fopen("leswords.txt","a");	
			fprintf(fp, " %s %d %d %d\n",GlobalWord,ntempcodas[0],ntempvowels, nphonvowels);
			fclose(fp);


			strcpy(tempvowels[ntempvowels-1],"le");
			tempvowelsnum[ntempvowels-1] = findGrapheme("le");
			ntempcodas[ntempvowels-2]--;	
		}
	}*/

//	if(ntempvowels == 2 && ntempcodas[0] > 1) {
	
	if(ntempvowels == nphonvowels && ntempvowels > 1) {
	// centre//hundred
		for(i=1;i<ntempvowels;i++) {
			if(ntempcodas[i-1] > 0) {
				if( (strcmp(tempvowels[i],"e")) == 0 && \
				(strcmp(tempcodas[i-1][ntempcodas[i-1]-1],"r")) == 0 && \
				phonvowels[i] == '@' && \
				phononsets[i][nphononsets[i]-1] != 'r' && \
				((ntempcodas[i] == 0) || \
				(ntempcodas[i] == 1 && ((strcmp(tempcodas[i][0],"d")) == 0 || (strcmp(tempcodas[i][0],"s")) == 0)) )) {
				
					strcpy(tempvowels[ntempvowels-1],"re");
					tempvowelsnum[ntempvowels-1] = findGrapheme("re");
					ntempcodas[ntempvowels-2]--;

					fp = fopen("reswords.txt","a");	
					fprintf(fp, " %s %d \n",GlobalWord,ntempcodas[0]);
					fclose(fp);
				}
			}
		}	 
	}


	// aggl kn
	for(i=0;i<ntempvowels - 1;i++) {
		for(j=0;j<ntempcodas[i];j++) {
			if((strcmp(tempcodas[i][j], "kn")) == 0 || \
				(strcmp(tempcodas[i][j], "mb")) == 0|| \
				(strcmp(tempcodas[i][j], "ng")) == 0|| \
				(strcmp(tempcodas[i][j], "dg")) == 0|| \
				(strcmp(tempcodas[i][j], "gn")) == 0|| \
				(strcmp(tempcodas[i][j], "sh")) == 0|| \
				(strcmp(tempcodas[i][j], "ph")) == 0|| \
				(strcmp(tempcodas[i][j], "gh")) == 0|| \

				(strcmp(tempcodas[i][j], "th")) == 0)
			
			{

				if( (phoncodas[i][nphoncodas[0]-1] == 'k' && phononsets[i+1][0] == 'n') || \
					(phoncodas[i][nphoncodas[0]-1] == 'm' && phononsets[i+1][0] == 'b') || \
					(phoncodas[i][nphoncodas[0]-1] == 'N' && phononsets[i+1][0] == 'g') || \
					(phoncodas[i][nphoncodas[0]-1] == 'n' && phononsets[i+1][0] == 'g') || \
					(phoncodas[i][nphoncodas[0]-1] == 'n' && phononsets[i+1][0] == '_') || \
					(phoncodas[i][nphoncodas[0]-1] == 'd' && phononsets[i+1][0] == 'g') || \
					(phoncodas[i][nphoncodas[0]-1] == 'g' && phononsets[i+1][0] == 'n') || \
					(phoncodas[i][nphoncodas[0]-1] == 's' && phononsets[i+1][0] == 'h') || \
					(phoncodas[i][nphoncodas[0]-1] == 'z' && phononsets[i+1][0] == 'h') || \
					(phoncodas[i][nphoncodas[0]-1] == 'p' && phononsets[i+1][0] == 'h') || \
					(phoncodas[i][nphoncodas[0]-1] == 'g' && phononsets[i+1][0] == 'h') || \
					(phoncodas[i][nphoncodas[0]-1] == 't' && phononsets[i+1][0] == 'h')) {
				
					fp = fopen("Splitupwords.txt","a");	
					fprintf(fp, " %s %d %d\n",GlobalWord,ntempcodas[i],j);
					fclose(fp);
					//banknote
					//amble = amb^l
					for(k=ntempcodas[i];k>ntempcodas[i]-(ntempcodas[i]-j);k--) {
						strcpy(tempcodas[i][k],tempcodas[i][k-1]);
						tempcodasnum[i][k] = tempcodasnum[i][k-1];
						/*fp = fopen("knwords.txt","a");	
						fprintf(fp, " %s %d %s\n",GlobalWord,k, tempcodas[i][k-1]);
						fclose(fp);*/

					}

					tempcodasnum[i][j] = NGRAPHEMES + FindLetter(tempcodas[i][j][0]);
					tempcodasnum[i][j+1] = NGRAPHEMES + FindLetter(tempcodas[i][j][1]);
					tempcodas[i][j+1][0] = tempcodas[i][j][1];
					tempcodas[i][j+1][1] =  0;
					tempcodas[i][j][1] = 0; //should still have the first letter.
					ntempcodas[i]++;
				}
				RecreateNewWord();
			}
		}
	}


	//middle e. braceleted
	if(ntempvowels == (nphonvowels + 1) && ntempvowels > 2) {
		//for(i=ntempvowels-1;i>0;i--) { //don't do the last should be done
		ecount = 0;
		for(j=1;j<ntempvowels-1;j++) {
			if( (strcmp(tempvowels[j],"e")) == 0) ecount++;
		}
		if(ecount == 1) {
			for(i=1;i<ntempvowels-1;i++) {
				if( (strcmp(tempvowels[i],"e")) == 0) break;
			}
		}
		if( ecount != 1 && \
			((strcmp(tempcodas[ntempvowels-2][0],"l")) == 0 || (strcmp(tempcodas[ntempvowels-2][0],"ll")) == 0) &&  \
			((strcmp(tempvowels[ntempvowels-1],"y")) == 0) && \
			((strcmp(tempvowels[ntempvowels-2],"e")) == 0) ) {
	
				fp = fopen("lly.txt","a");
					fprintf(fp,"out %s %d %d %d\n",GlobalWord, ntempvowels,nphonvowels,ecount);
					fclose(fp);	


			i = ntempvowels - 2;
			ecount = 1;
		}
		/* if( ecount != 1 && \
			ntempcodas[ntempvowels-2] > 1 &&
			(strcmp(tempcodas[ntempvowels-2][ntempcodas[ntempvowels-2]-1],"l")) == 0 && \
			(strcmp(tempvowels[ntempvowels-1],"y")) == 0 && \
			(strcmp(tempvowels[ntempvowels-2],"e")) == 0 ) {

		}*/

		if(ecount == 2) {
			int bits[8];
			int bigger = -1;
			int spot = 0;

			for(i=0;i<8;i++) bits[i] = -1;
			for(i=1;i<ntempvowels-1;i++) {
				if( (strcmp(tempvowels[i],"e")) == 0) {
					bits[i] = ntempcodas[i-1];
				}
			}
			for(i=1;i<ntempvowels-1;i++) {
				if(ntempcodas[i-1] == 0 && bits[i] != -1 && ((strcmp(tempvowels[i-1],"er")) != 0) && strcmp(GlobalWord,"picturesqueness") != 0) {
					//surefooted
					spot = i;
					fp = fopen("middle-e.0.txt","a");
					fprintf(fp,"out %s %d %d %d\n",GlobalWord, ntempvowels,nphonvowels,ecount);
					fclose(fp);					
					break;
				}
				// daredevil vs. serenely

				if(bits[i] != -1) { /* There are coda consonants before the vowel*/
					if( (strcmp(tempcodas[i-1][ntempcodas[i-1]-1],"c")) == 0 || (strcmp(tempcodas[i-1][ntempcodas[i-1]-1],"dg")) == 0) {
							fp = fopen("cdg.0.txt","a");
							fprintf(fp,"out %s %d %d %d\n",GlobalWord, ntempvowels,nphonvowels,ecount);
							fclose(fp);		
						ecount = 1;
						break;
					}
			
					if(bigger == -1) {
						bigger = bits[i];
						spot = i;
					} else {
						if(bigger == bits[i]) {
							if( (strcmp(tempvowels[spot-1],"e")) == 0 && (strcmp(tempvowels[i-1],"e")) != 0 && ntempcodas[spot-1] == 1 && ntempcodas[i-1] == 1) {
							
								fp = fopen("singleE1.0.txt","a");
								fprintf(fp,"out %s %d %d %d\n",GlobalWord, ntempvowels,nphonvowels,ecount);
								fclose(fp);	
								ecount = 2;
								spot = i;
								break;
							} else if (( strcmp(tempvowels[spot-1],"e")) != 0 && (strcmp(tempvowels[i-1],"e")) == 0 && ntempcodas[spot-1] == 1 && ntempcodas[i-1] == 1) {
								fp = fopen("e.double.0.txt","a");
								fprintf(fp,"out %s %d %d %d\n",GlobalWord, ntempvowels,nphonvowels,ecount);
								fclose(fp);
								ecount = 2;
								//spot remains the same
								break;
							} else ecount = 100;
						} else {
							if(bigger < bits[i]) {
								fp = fopen("biggerlessbits.txt","a");
								fprintf(fp,"out %s %d %d %d\n",GlobalWord, ntempvowels,nphonvowels,ecount);
								fclose(fp);
								spot = i;
								break;
							}
						}
					}
				}
			}
			//last few. nessless
			if(i == ntempvowels - 1) {
				if((strcmp(tempcodas[ntempcodas[ntempvowels-1]][ntempcodas[ntempvowels-1]],"ss")) == 0) {
					spot = ntempvowels - 2;
				}
				if( (strcmp(GlobalWord,"revengefulness")) == 0 || \
					(strcmp(GlobalWord,"revengeful")) == 0 || \
					(strcmp(GlobalWord,"revengefulness")) == 0) {
					spot = 2;
					ecount = 1;
				}
				if( (strcmp(GlobalWord,"disengagement")) == 0 ||
					(strcmp(GlobalWord,"revengeful")) == 0 )	{
					spot = 3;
					ecount = 1;
				}
			}
			i = spot;
		}

		if(ecount != 0 && ecount != 1 && ecount != 2) {				
			fp = fopen("middle-e.notdone.txt","a");
			fprintf(fp,"out %s %d %d \n",GlobalWord, ntempvowels,nphonvowels);
			fclose(fp);
			
			if( (strcmp(GlobalWord, "defencelessness") != 0) && \
				(strcmp(GlobalWord,"defencelessly")) != 0 && \
				(strcmp(GlobalWord,"revengefully")) != 0) {
				ecount = 2;
				i = ntempvowels-2;
			} else {
				ecount = 2;
				i = ntempvowels - 3;
			}
		}
		if(ecount == 1 || ecount == 2 && i != 0) {
			//for(i=1;i<ntempvowels-1;i++) {				
			if(ecount == 1) {
				fp = fopen("middle-e.1.txt","a");
				fprintf(fp,"out %s %d %d %d\n",GlobalWord, ntempvowels,nphonvowels,ecount);
				fclose(fp);
			} else {
				fp = fopen("middle-e.2.txt","a");
				fprintf(fp,"out %s %d %d %d\n",GlobalWord, ntempvowels,nphonvowels,ecount);
				fclose(fp);
			}

				if((strcmp(tempvowels[i],"e")) == 0) {
					strcpy(tempcodas[i-1][ntempcodas[i-1]],"e");
					tempcodasnum[i-1][ntempcodas[i-1]] = NONSETS+NVOWELS+NCODAS + FindLetter('e');
					ntempcodas[i-1]++;			
				//	ntemponsets[i] = 0;

					for(j=0;j<ntempcodas[i];j++){
						strcpy(tempcodas[i-1][ntempcodas[i-1]],tempcodas[i][j]);
						tempcodasnum[i-1][ntempcodas[i-1]] = tempcodasnum[i][j];
						ntempcodas[i-1]++;
					}			
	
					for(j=i;j<ntempvowels;j++) {
						strcpy(tempvowels[j],tempvowels[j+1]);
						tempvowelsnum[j] = tempvowelsnum[j+1];
						for(k=0;k<ntempcodas[j+1];k++) {
							strcpy(tempcodas[j][k],tempcodas[j+1][k]);
							tempcodasnum[j][k] = tempcodasnum[j+1][k];
						}
						ntempcodas[j] = ntempcodas[j+1];
					}
					ntempvowels--;
					ntempcodas[j] = 0;
				//	PrintWord();
			//	}
			}
		} 
		
		//PrintWord();
	}



	//add this in a bit..
	//airy
	//if(ntempvowels == 2 && ntempcodas[0] == 0 && nphononsets[1] == 1) {
	//virus
	for(h=0;h<ntempvowels-1;h++) {
		if(/*ntempvowels == nphonvowels && */ ntempcodas[h] == 0 && nphononsets[h+1] == 1) {
			i = strlen(tempvowels[h]);
			theletter = tempvowels[h][i-1];
			if(tempvowels[h][i-1] == 'r' && phononsets[h+1][0] == 'r' || \
				tempvowels[h][i-1] == 'w' && phononsets[h+1][0] == 'w') {
		
				tempvowels[h][i-1] = 0;
				if((strlen(tempvowels[h])) != 1) {
					tempvowelsnum[h] = findGrapheme(tempvowels[h]);
				} else {
					tempvowelsnum[h] = NGRAPHEMES + FindLetter(tempvowels[h][0]);
				}
				tempcodas[h][0][0] = theletter;
				tempcodas[h][0][1] = 0;
				tempcodasnum[h][0] = NGRAPHEMES + FindLetter(toupper(tempcodas[h][0][0])); 
				ntempcodas[h] = 1;

			// and the rest!
				

					fp = fopen("FixedDVowels.txt","a");	
					fprintf(fp, " %s \n",GlobalWord);
					fclose(fp);

			}
		}
	}
	//backyard
	if(ntempvowels == nphonvowels+1) {
		for(i=1;i<ntempvowels-1;i++) {
			if( (strcmp(tempvowels[i],"y")) == 0 /*&& (phononsets[i][0] == 'j') */ && (ntempcodas[i] == 0) ) {
					fp = fopen("Y.Words.txt","a");	
					fprintf(fp, " %s \n",GlobalWord);
					fclose(fp);
				
				strcpy(tempcodas[i-1][ntempcodas[i-1]],tempvowels[i]);
				tempcodasnum[i-1][ntempcodas[i-1]] = NGRAPHEMES + FindLetter(tempvowels[i][0]);
				ntempcodas[i-1]++;

				for(j=i;j<ntempvowels;j++) {
					strcpy(tempvowels[j],tempvowels[j+1]);
					tempvowelsnum[j] = tempvowelsnum[j+1];
					for(k=0;k<ntempcodas[j+1];k++) {
						strcpy(tempcodas[j][k],tempcodas[j+1][k]);
						tempcodasnum[j][k] = tempcodasnum[j+1][k];
					}
					ntempcodas[j] = ntempcodas[j+1];
				}
				ntempvowels--;
				break;

			}
		}
	}

	//causeway. cae-sew.ay forerunner. s.o.m.ew.here.
	if(ntempvowels == nphonvowels+1) {
		for(i=1;i<ntempvowels-1;i++) {
			if((strcmp(tempvowels[i],"ew")) == 0 && (phononsets[i][0] == 'w') || \
				(strcmp(tempvowels[i],"ew")) == 0 && (phononsets[i][0] == 'r') || \
				(strcmp(tempvowels[i],"er")) == 0 && (phononsets[i][0] == 'r')  || \
				(strcmp(tempvowels[i],"ey")) == 0 && (phononsets[i][0] == 'j')  ) {
					
					fp = fopen("EW.ER.Words.txt","a");	
					fprintf(fp, " %s \n",GlobalWord);
					fclose(fp);


				strcpy(tempcodas[i-1][ntempcodas[i-1]],"e");
				tempcodasnum[i-1][ntempcodas[i-1]] = NGRAPHEMES + FindLetter(tempvowels[i][0]);
				ntempcodas[i-1]++; // 1. 

				if(ntempcodas[i] == 0) {
					tempcodas[i-1][ntempcodas[i-1]][0] = tempvowels[i][1];
					tempcodas[i-1][ntempcodas[i-1]][1] = 0;
					tempcodasnum[i-1][ntempcodas[i-1]] = NGRAPHEMES + FindLetter(tempvowels[i][1]);
					ntempcodas[i-1]++;
				} else { //elsewhere . els.ewh.ere
				
					tempcodas[i-1][ntempcodas[i-1]][1] = tempcodas[i][0][0];
					tempcodas[i-1][ntempcodas[i-1]][0] = tempvowels[i][1];
					tempcodas[i-1][ntempcodas[i-1]][2] = 0;

					/*fp = fopen("EW.Words.txt","a");	
					fprintf(fp, " %s %s\n",GlobalWord,tempcodas[i-1][ntempcodas[i-1]]);
					fclose(fp);*/

					tempcodasnum[i-1][ntempcodas[i-1]] = findGrapheme(tempcodas[i-1][ntempcodas[i-1]]);
					ntempcodas[i] = 0;
					//ntempcodas[i-1] = 2;
					ntempcodas[i-1]++;
				}
			

				for(j=i;j<ntempvowels;j++) {
					strcpy(tempvowels[j],tempvowels[j+1]);
					tempvowelsnum[j] = tempvowelsnum[j+1];
					for(k=0;k<ntempcodas[j+1];k++) {
						strcpy(tempcodas[j][k],tempcodas[j+1][k]);
						tempcodasnum[j][k] = tempcodasnum[j+1][k];
					}
					ntempcodas[j] = ntempcodas[j+1];
				}
				ntempvowels--;
				break;
			}
		}
	}
#endif
}

//amblyopes
void GraphemeParsing::FixEarlyExceptionsFrench()
{
#if FRENCH
	int a,h,i,j,k, l, LetterSpot, ecount, nmults,ok;
	char theletter;
	FILE *fp, *fp1;
	int splitspot;
	char thebit1[20],thebit2[20],bit1[20],bit2[20];
	

	/*fp = fopen("SFinalj.txt","a");	
	fprintf(fp, "%s %d %d %s %c\n", GlobalWord, ntempvowels,nphonvowels+1,tempvowels[ntempvowels-1],phononsets[nphonvowels-1][nphononsets[nphonvowels-1]-1]);
	fclose(fp);*/
	//Just final i wiht dif numbs of sylls
	
	for(i=0;i<ntempvowels;i++) {
		if(strcmp(tempvowels[i],"oo") ==0) break;
	}
	if(i!=ntempvowels) { //O.O words
		for(j=0;j<nphonvowels-1;j++) {
			if(phonvowels[j] == 'O' && phonvowels[j+1] == 'O') {
				break;
			}
		}
		if(j!=nphonvowels-1) {
			fp = fopen("OOsplit.txt","a");	
			fprintf(fp, "--%s \n", GlobalWord);
			fclose(fp);

			strcpy(tempvowels[i],"o"); 
			tempvowelsnum[i] = NGRAPHEMES + FindLetter('o');
				
			for(k=ntempvowels;k>i;k--) {
				for(l=0;l<ntempcodas[k-1];l++) {
					strcpy(tempcodas[k][l],tempcodas[k-1][l]);
					tempcodasnum[k][l] = tempcodasnum[k-1][l];
				}
				ntempcodas[k] = ntempcodas[k-1];
				strcpy(tempvowels[k],tempvowels[k-1]);
				tempvowelsnum[k] = tempvowelsnum[k-1];
			}
			ntempcodas[i] = 0;
			strcpy(tempvowels[i+1],"o");
			tempvowelsnum[i+1] = tempvowelsnum[i];
			ntempvowels++;

			PrintWord();
		}
	}

	for(h=0;h<ntempvowels-1;h++) {
		if(ntempcodas[h] == 0 && tempvowels[h][0] == 'e') { 
			i = strlen(tempvowels[h]);
			theletter = tempvowels[h][i-1];
			if( ((tempvowels[h][i-1] == 'm' ) || \
				(tempvowels[h][i-1] == 'r' ) ||  \
				(tempvowels[h][i-1] == 'y' ) || \
				(tempvowels[h][i-1] == 'n' ))) {
		
				//MAKE SPOT FOR NEW
				for(i=ntempcodas[h];i>0;i--) {
					strcpy(tempcodas[h][i],tempcodas[h][i-1]);
					tempcodasnum[h][i] = tempcodasnum[h][i-1];
				}
				ntempcodas[h]++;

				//FIX VOWEL
				i = strlen(tempvowels[h]);
				tempvowels[h][i-1] = 0;
				if((strlen(tempvowels[h])) != 1) tempvowelsnum[h] = findGrapheme(tempvowels[h]);
				else tempvowelsnum[h] = NGRAPHEMES + FindLetter(tempvowels[h][0]);
				
				//ADD NEW CONSONANT.
				tempcodas[h][0][0] = theletter;
				tempcodas[h][0][1] = 0;
				tempcodasnum[h][0] = NGRAPHEMES + FindLetter(tempcodas[h][0][0]); 
				ntempcodas[h] = 1;
				//PrintWord();
				fp = fopen("rejigsfirst.txt","a");	
				fprintf(fp, " %s %s %s %c %c %c\n",GlobalWord, tempvowels[h], tempcodas[h],phononsets[h+1][0],phoncodas[h][0],phononsets[h][0]);
				fclose(fp);
				PrintWord();
				//ntempcodas[h] = 1;
				//algerienne.
			
			}
		}
	}

	if(ntempvowels>3) {
		if(ntempcodas[ntempvowels-1] == 1 && strcmp(tempcodas[ntempvowels-1][0],"t") == 0) {
			if((strcmp(tempvowels[ntempvowels-1],"en")) == 0) {
				if(strcmp(tempcodas[ntempvowels-2][0],"m")==0 && ntempcodas[ntempvowels-2] == 1) {
					if(strcmp(tempvowels[ntempvowels-2],"e") == 0) {
					/*	fp = fopen("mentfirst.check.txt","a");
						fprintf(fp, " %s  %c %c %d %d %d\n",GlobalWord,phononsets[2][0],phononsets[nphononsets[nphonvowels-1]][0],nphononsets[nphonvowels-1],nphonvowels,nphononsets[nphonvowels-1],nphononsets[2]);
						fclose(fp);	*/			
						/*fp = fopen("phontestsss.txt","a");
						fprintf(fp,"%d \n",nphonvowels);
						for(i=0;i<nphonvowels;i++) {
							for(j=0;j<nphononsets[i];j++) fprintf(fp,"o%d=%c ",j,phononsets[i][j]);
							fprintf(fp,"%c ",phonvowels[i]);
							for(j=0;j<nphoncodas[i];j++) fprintf(fp,"c%d=%c ",j,phoncodas[i][j]);
							fprintf(fp,"\n");
						}
						fclose(fp);*/						
						if( (phononsets[nphonvowels-1][0] == 'm') && (nphononsets[nphonvowels-1] == 1) ) {
						//	fp = fopen("mentfirst.check.2.txt","a");fprintf(fp, " %s \n",GlobalWord);fclose(fp);		
							if(phonvowels[nphonvowels-1] == '@') {	
								fp = fopen("mentfirst.txt","a");	
								fprintf(fp, " %s \n",GlobalWord);
								fclose(fp);

								strcpy(tempcodas[ntempvowels-3][ntempcodas[ntempvowels-3]],"e");
								tempcodasnum[ntempvowels-3][ntempcodas[ntempvowels-3]] = NGRAPHEMES + FindLetter('e');
								ntempcodas[ntempvowels-3]++;
								strcpy(tempcodas[ntempvowels-3][ntempcodas[ntempvowels-3]],"m");
								tempcodasnum[ntempvowels-3][ntempcodas[ntempvowels-3]] = NGRAPHEMES + FindLetter('m');
								ntempcodas[ntempvowels-3]++;
								strcpy(tempvowels[ntempvowels-2],"en");
								tempvowelsnum[ntempvowels-2] = findGrapheme("en");
								strcpy(tempcodas[ntempvowels-2][0],"t");
								tempcodasnum[ntempvowels-2][0] = NGRAPHEMES + FindLetter('t');
								ntempcodas[ntempvowels-2] = 1;
								ntempvowels--;
								PrintWord();
							}
						}
					}
				} 
			}
		}
	}

	
	if(ntempvowels == nphonvowels+1 && ntempvowels > 1) {
		if(tempvowels[ntempvowels-1][0] == 'i' && strlen(tempvowels[ntempvowels-1]) > 1) {
			if(phononsets[nphonvowels-1][nphononsets[nphonvowels-1]-1] == 'j' && strlen(tempvowels[ntempvowels-1]) > 1) {			
				i = strlen(tempvowels[ntempvowels-1]);
				strcpy(thebit2, tempvowels[ntempvowels-1]+1);
				thebit2[strlen(tempvowels[ntempvowels-1]) - 1] = 0;
				strcpy(thebit1,tempvowels[ntempvowels-1]);
				thebit1[1] = 0;
				strcpy(tempcodas[ntempvowels-2][ntempcodas[ntempvowels-2]],"i");
				tempcodasnum[ntempvowels-2][ntempcodas[ntempvowels-2]] = NGRAPHEMES + FindLetter('i');
				ntempcodas[ntempvowels-2]++;
				strcpy(tempvowels[ntempvowels-1],thebit2);
				if(strlen(thebit2) > 1) tempvowelsnum[ntempvowels-1] = findGrapheme(thebit2);
				else tempvowelsnum[ntempvowels-1] = NGRAPHEMES + FindLetter(thebit2[0]);
				
			
				
				PrintWord();
			}
		}
	}


	//ez
	if( ntempvowels > 1) {
		if((ntempcodas[ntempvowels-1] == 1 && tempcodas[ntempvowels-1][0][0] == 'z') || \
			(ntempcodas[ntempvowels-1] == 1 && tempcodas[ntempvowels-1][0][0] == 's') ||
			(ntempcodas[ntempvowels-1] == 1 && tempcodas[ntempvowels-1][0][0] == 'r')  ) {
			i = strlen(tempvowels[ntempvowels-1]);
			if(i > 1 && tempvowels[ntempvowels-1][i-1] == 'e') {
				if(strcmp(tempvowels[ntempvowels],"oie") == 0) {
					strcpy(tempvowels[ntempvowels-2],"o");
					strcpy(tempvowels[ntempvowels-1],"i");
					strcpy(tempcodas[ntempvowels-1][1],tempvowels[ntempvowels-1]);
					strcpy(tempcodas[ntempvowels-1][0],"e");
					ntempcodas[ntempvowels-1] = 2;
					tempvowelsnum[ntempvowels-2] = FindLetter('o');
					tempvowelsnum[ntempvowels-1] = FindLetter('i');
					tempcodasnum[ntempvowels][0] = FindLetter('e');
					tempcodasnum[ntempvowels][1] = FindLetter(tempcodas[ntempvowels-1][1][0]);
				} else {
					bit1[0] = tempcodas[ntempvowels-1][0][0];
					bit1[1] = 0;
					tempvowels[ntempvowels-1][i-1] = 0;
					if(strlen(tempvowels[ntempvowels-1]) > 1) tempvowelsnum[ntempvowels-1] = findGrapheme(tempvowels[ntempvowels-1]);
					else tempvowelsnum[ntempvowels-1] = NGRAPHEMES + FindLetter(tempvowels[ntempvowels-1][0]);

					ntempcodas[ntempvowels-1] = 0;
					strcpy(tempvowels[ntempvowels],"e");
					tempvowelsnum[ntempvowels] = NGRAPHEMES + FindLetter('e');
					strcpy(tempcodas[ntempvowels][0],bit1);
					tempcodasnum[ntempvowels][0] = NGRAPHEMES + FindLetter(bit1[0]);
					ntempcodas[ntempvowels] = 1;
					ntempvowels++;

					fp = fopen("ezfix.txt","a");	
					fprintf(fp, " %s \n",GlobalWord);
					fclose(fp);
				}
			}
		}
	}
	//PrintWord();
	//Finale e.
	if(ntempvowels > 1 && strcmp(tempvowels[ntempvowels-1],"e") == 0 && ntempcodas[ntempvowels-1] == 0) {
		strcpy(tempcodas[ntempvowels-2][ntempcodas[ntempvowels-2]],"e");
		tempcodasnum[ntempvowels-2][ntempcodas[ntempvowels-2]] = NGRAPHEMES + FindLetter('e');
		ntempcodas[ntempvowels-2]++;
		ntempcodas[ntempvowels-1] = 0;
		ntempvowels--;	
		fp = fopen("finales.txt","a");	
		fprintf(fp, " %s \n",GlobalWord);
		fclose(fp);
	}
	//could add -c, -r(aient) -n, ll
	if(ntempvowels > 1 /*&& ntempcodas[ntempvowels-2] > 0*/) {
		//2011. if( (strcmp(tempcodas[ntempvowels-2][ntempcodas[ntempvowels-2]-1],"ss")) == 0) {
			if( (ntempcodas[ntempvowels-1] == 1 /* && tempcodas[ntempvowels-1][0][0] == 'n'*/) && \
				(tempcodas[ntempvowels-1][0][0] == 't') && \
				((strcmp(tempvowels[ntempvowels-1],"en")) == 0) && ntempvowels == nphonvowels-1 ) {
		
				strcpy(tempcodas[ntempvowels-2][ntempcodas[ntempvowels-2]],"en");
				tempcodasnum[ntempvowels-2][ntempcodas[ntempvowels-2]] = findGrapheme("en");
				strcpy(tempcodas[ntempvowels-2][ntempcodas[ntempvowels-2]+1],"t");
				tempcodasnum[ntempvowels-2][ntempcodas[ntempvowels-2]+1] = NGRAPHEMES + FindLetter('t');
				ntempcodas[ntempvowels-2]+=2;

				ntempcodas[ntempvowels-1] = 0;
				ntemponsets[ntempvowels-1]= 0;
				/*strcpy(tempcodas[ntempvowels-2][ntempcodas[ntempvowels-2]],"en");
				tempcodasnum[ntempvowels-2][ntempcodas[ntempvowels-2]] = findGrapheme("en");
				ntempcodas[ntempvowels-2]++;
				strcpy(tempcodas[ntempvowels-2][ntempcodas[ntempvowels-2]],"t");
				tempcodasnum[ntempvowels-2][ntempcodas[ntempvowels-2]] = NGRAPHEMES + FindLetter('n');
				ntempcodas[ntempvowels-2]++;
				ntempcodas[ntempvowels-1] = 0;*/

				ntempvowels--;	

				fp = fopen("ents.txt","a");	
				fprintf(fp, " %s \n",GlobalWord);
				fclose(fp);
			}
		//}
	}

	//finale e grapheme
	
	if( strlen(tempvowels[ntempvowels-1]) > 1 && tempvowels[ntempvowels-1][strlen(tempvowels[ntempvowels-1])-1] == 'e'&& ntempcodas[ntempvowels-1] == 0) {
		j = strlen(tempvowels[ntempvowels-1]);
		if(strcmp(tempvowels[ntempvowels-1],"oi") == 0) {
			strcpy(tempvowels[ntempvowels-1],"o");
			strcpy(tempvowels[ntempvowels],"i");
			strcpy(tempcodas[ntempvowels][0],"e");
			tempvowelsnum[ntempvowels-1] = NGRAPHEMES + FindLetter('o');
			tempvowelsnum[ntempvowels] = NGRAPHEMES + FindLetter('i');
			tempcodasnum[ntempvowels][0] = NGRAPHEMES + FindLetter('e');
			ntempcodas[ntempvowels] = 1;
			ntempvowels++;

			fp = fopen("finales.oi.es.txt","a");	
			fprintf(fp, " %s \n",GlobalWord);
			fclose(fp);
		} else {
			if(ntempvowels > 1 || (ntempvowels == 1 && strlen(tempvowels[0]) > 1) ) {
				ntempcodas[ntempvowels-1] = 0;	
				tempvowels[ntempvowels-1][j-1] = 0;	
				if(strlen(tempvowels[ntempvowels-1]) > 1) tempvowelsnum[ntempvowels-1] = findGrapheme(tempvowels[ntempvowels-1]);
				else tempvowelsnum[ntempvowels-1] = NGRAPHEMES + FindLetter(tempvowels[ntempvowels-1][0]);
					
				strcpy(tempcodas[ntempvowels-1][ntempcodas[ntempvowels-1]],"e");
				tempcodasnum[ntempvowels-1][ntempcodas[ntempvowels-1]] = NGRAPHEMES + FindLetter('e');
				ntempcodas[ntempvowels-1]++;
				/*	fp = fopen("finales.multi.single.in.txt","a");	
					fprintf(fp, " %s %s \n",GlobalWord, tempvowels[0]);
					fclose(fp);*/
			}
		}
		fp = fopen("finales.multi.single.e.txt","a");	
		fprintf(fp, " %s %s\n",GlobalWord, tempvowels[0]);
		fclose(fp);
	}
	//PrintWord();

	i = strlen(tempvowels[ntempvowels-1]);
	//oies  //tes
	if(tempvowels[ntempvowels-1][i-1] == 'e' && ntempcodas[ntempvowels-1] == 1 && \
		(tempcodas[ntempvowels-1][0][0] == 's')	) {
			//croies
		j = strlen(tempvowels[ntempvowels-1]);
		if(j > 1) {
			if((strcmp(tempvowels[ntempvowels-1],"oie")) == 0) {
				strcpy(tempvowels[ntempvowels-1],"o");
				ntempcodas[ntempvowels-1] = 0;
				strcpy(tempvowels[ntempvowels],"i");
				strcpy(tempcodas[ntempvowels][0],"e");
				strcpy(tempcodas[ntempvowels][1],"s");
				tempvowelsnum[ntempvowels-1] = NGRAPHEMES + FindLetter('o');
				tempvowelsnum[ntempvowels] = NGRAPHEMES + FindLetter('i');
				tempcodasnum[ntempvowels][0] = NGRAPHEMES + FindLetter('e');
				tempcodasnum[ntempvowels][1] = NGRAPHEMES + FindLetter('s');	
				ntempcodas[ntempvowels] = 2;
				ntempvowels++;
				PrintWord();

				fp = fopen("finales.oies.txt","a");	
				fprintf(fp, " %s \n",GlobalWord);
				fclose(fp);
			} else {
				tempvowels[ntempvowels-1][j-1] = 0;
				if(strlen(tempvowels[ntempvowels-1]) > 1) tempvowelsnum[ntempvowels-1] = findGrapheme(tempvowels[ntempvowels-1]);
				else tempvowelsnum[ntempvowels-1] = NGRAPHEMES + FindLetter(tempvowels[ntempvowels-1][0]);
				ntempcodas[ntempvowels-1] = 0;
				strcpy(tempcodas[ntempvowels-1][ntempcodas[ntempvowels-1]],"e");
				tempcodasnum[ntempvowels-1][ntempcodas[ntempvowels-1]] = NGRAPHEMES + FindLetter('e');
				ntempcodas[ntempvowels-1]++;
				strcpy(tempcodas[ntempvowels-1][ntempcodas[ntempvowels-1]], "s");
				tempcodasnum[ntempvowels-1][ntempcodas[ntempvowels-1]] = NGRAPHEMES + FindLetter('s');
				ntempcodas[ntempvowels-1]++;	
			}
		} else { 
			if(ntempvowels > 1) {
				strcpy(tempcodas[ntempvowels-2][ntempcodas[ntempvowels-2]],"e");
				tempcodasnum[ntempvowels-2][ntempcodas[ntempvowels-2]] = NGRAPHEMES + FindLetter('e');
				ntempcodas[ntempvowels-2]++;
				strcpy(tempcodas[ntempvowels-2][ntempcodas[ntempvowels-2]], "s");
				tempcodasnum[ntempvowels-2][ntempcodas[ntempvowels-2]] = NGRAPHEMES + FindLetter('s');
				ntempcodas[ntempvowels-2]++;	
				ntempcodas[ntempvowels-1] = 0;
				ntempvowels--;				
			}
		}
		PrintWordName("esstuff.txt");
		fp = fopen("finales.es.txt","a");	
		fprintf(fp, " %s \n",GlobalWord);
		fclose(fp);
	}
	
	
	//fix y amblyopes
	for(h=1;h<ntempvowels;h++) {
		if(ntempvowels > nphonvowels) {
			if( ( (strcmp(tempvowels[h],"y"))==0) || ((strcmp(tempvowels[h],"ï"))==0)) {
				if( (phoncodas[h-1][0] == 'j') || (phononsets[h+1][0] == 'j' && h <= nphonvowels-2) )  {
					if(phonvowels[h] != 'i') {
					//PrintWordName("testthis.txt");
					
						fp1 = fopen("y.fixe.txt","a");
						fprintf(fp1,"----->%c %c %c %c %d %s %s %d %d %s %s\n",tempvowels[h][0],phonvowels[h], phoncodas[h-1][0],phononsets[h+1][0],h,tempvowels[h],GlobalWord,ntempcodas[0],ntemponsets[1], tempcodas[0],temponsets[1]);
						fclose(fp1);	

						strcpy(tempcodas[h-1][ntempcodas[h-1]],tempvowels[h]);
						tempcodasnum[h-1][ntempcodas[h-1]] = NGRAPHEMES + FindLetter(tempvowels[h][0]);
						ntempcodas[h-1]++;

						for(i=0;i<ntempcodas[h]; i++){
							strcpy(tempcodas[h-1][ntempcodas[h-1]],tempcodas[h][i]);
							tempcodasnum[h-1][ntempcodas[h-1]] = tempcodasnum[h][i];
							ntempcodas[h-1]++;
						}
		
						 for(i=h;i<ntempvowels-1;i++) {
							strcpy(tempvowels[i],tempvowels[i+1]);
							tempvowelsnum[i] = tempvowelsnum[i+1];
							for(j=0;j<ntempcodas[i+1];j++) {
								strcpy(tempcodas[i][j],tempcodas[i+1][j]);
								tempcodasnum[i][j] = tempcodasnum[i+1][j];
							}
							ntempcodas[i] = ntempcodas[i+1];
						}
						ntempcodas[ntempvowels-1] = 0;
						ntempvowels--;
					}
				}			
			}
		}
	}
	

	for(h=0;h<ntempvowels-1;h++) {
		if(ntempcodas[h] == 0 && (strcmp(tempvowels[h],"ie")) == 0 && (strcmp(tempvowels[h+1],"u")) == 0 && ntempcodas[h] == 0) {
			strcpy(tempvowels[h],"i");
			strcpy(tempvowels[h+1],"eu");
			tempvowelsnum[h] = NGRAPHEMES + FindLetter('i');
			tempvowelsnum[h+1] = findGrapheme("eu");
		}
		if(ntempcodas[h] == 0 && (strcmp(tempvowels[h],"io")) == 0 && (strcmp(tempvowels[h+1],"u")) == 0 && ntempcodas[h] == 0) {
			strcpy(tempvowels[h],"i");
			strcpy(tempvowels[h+1],"ou");
			tempvowelsnum[h] = NGRAPHEMES + FindLetter('i');
			tempvowelsnum[h+1] = findGrapheme("ou");
		}
	}

	for(h=0;h<ntempvowels;h++) {	
		if(ntempvowels > nphonvowels) {
			if(h < ntempvowels -1) {
				if((((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"on")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"e")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"ia")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"a")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"en")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"an")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"o")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"e")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"â")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"é")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"è")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"ou")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"eu")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"ai")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"om")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"on")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"er")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"am")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"u")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"i")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"ée")) == 0) ||\
					((strcmp(tempvowels[h],"ou")) == 0 && (strcmp(tempvowels[h+1],"an")) == 0) ||\
					((strcmp(tempvowels[h],"ou")) == 0 && (strcmp(tempvowels[h+1],"a")) == 0) ||\
					((strcmp(tempvowels[h],"ou")) == 0 && (strcmp(tempvowels[h+1],"â")) == 0) ||\
					((strcmp(tempvowels[h],"ou")) == 0 && (strcmp(tempvowels[h+1],"é")) == 0) ||\
					((strcmp(tempvowels[h],"ou")) == 0 && (strcmp(tempvowels[h+1],"e")) == 0) ||\
					((strcmp(tempvowels[h],"ou")) == 0 && (strcmp(tempvowels[h+1],"ï")) == 0) ||\
					((strcmp(tempvowels[h],"ou")) == 0 && (strcmp(tempvowels[h+1],"î")) == 0) ||\
					((strcmp(tempvowels[h],"ou")) == 0 && (strcmp(tempvowels[h+1],"i")) == 0) ||\
					((strcmp(tempvowels[h],"ou")) == 0 && (strcmp(tempvowels[h+1],"eu")) == 0) ||\
					((strcmp(tempvowels[h],"ou")) == 0 && (strcmp(tempvowels[h+1],"è")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"e")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"i")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"a")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"an")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"on")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"ai")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"é")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"è")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"â")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"ai")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"an")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"uo")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"o")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"y")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"aie")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"î")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"in")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"ou")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"on")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"an")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"en")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"ai")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"e")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"eu")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"è")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"é")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"u")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"a")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"O")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"o")) == 0) ||\
					((strcmp(tempvowels[h],"e")) == 0 && (strcmp(tempvowels[h+1],"o")) == 0) ||\
					((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"in")) == 0) ||\
					((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"in")) == 0) ||\
					((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"î")) == 0) ||\
					((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"ie")) == 0) ||\
					((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"e")) == 0) ||\
					((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"ê")) == 0) ||\
					((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"è")) == 0) ||\
					((strcmp(tempvowels[h],"e")) == 0 && (strcmp(tempvowels[h+1],"y")) == 0) ||\
					((strcmp(tempvowels[h],"a")) == 0 && (strcmp(tempvowels[h+1],"y")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"y")) == 0) ||\
					((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"y")) == 0) ||\
					((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"ï")) == 0) ||\
					((strcmp(tempvowels[h],"ï")) == 0 && (strcmp(tempvowels[h+1],"a")) == 0) ||\
					((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"i")) == 0) ) && ntempcodas[h] == 0) {
			

					if (((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"y")) == 0 && ntempcodas[h] == 0&& phononsets[h+1][0] == 'j') ||\
						((strcmp(tempvowels[h],"e")) == 0 && (strcmp(tempvowels[h+1],"y")) == 0 && ntempcodas[h] == 0&& phononsets[h+1][0] == 'j') ||\
						((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"y")) == 0 && ntempcodas[h] == 0&& phononsets[h+1][0] == 'j') ||\
						((strcmp(tempvowels[h],"ï")) == 0 && (strcmp(tempvowels[h+1],"a")) == 0 && ntempcodas[h] == 0 && phononsets[h+1][0] == 'j') ||\
						((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"y")) == 0 && ntempcodas[h] == 0 && phononsets[h+1][0] == 'j') ||\
						((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"ï")) == 0 && (strcmp(tempvowels[h+2],"a")) == 0 && ntempcodas[h] == 0) ||\
						((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"ï")) == 0 && (strcmp(tempvowels[h+2],"a")) == 0 && ntempcodas[h] == 0) ||\
						((strcmp(tempvowels[h],"a")) == 0 && (strcmp(tempvowels[h+1],"y")) == 0 && ntempcodas[h] == 0 && phononsets[h+1][0] == 'j') 		) {
							fp1 = fopen("uy.txt","a");
							fprintf(fp1,"%s %s %d %d %s %s %d\n",GlobalWord,PhonString,ntempcodas[0],ntemponsets[1], tempcodas[0],temponsets[1],FindLetter('i'));
							fclose(fp1);

							strcpy(tempcodas[h][0],tempvowels[h+1]);
							tempcodasnum[h][0] = NGRAPHEMES + FindLetter(tempvowels[h+1][0]);
							ntempcodas[h] = 1;
							for(i=h+1;i<ntempvowels;i++) {
								strcpy(tempvowels[i],tempvowels[i+1]);
								tempvowelsnum[i] = tempvowelsnum[i+1];
								for(j=0;j<ntempcodas[i+1];j++) {
									strcpy(tempcodas[i][j],tempcodas[i+1][j]);
									tempcodasnum[i][j] = tempcodasnum[i+1][j];
								}
								ntempcodas[i] = ntempcodas[i+1];
							}
							ntempvowels--;
					} else {

						if(phononsets[h][nphononsets[h]-1] == 'j' ||\
							phononsets[h][nphononsets[h]-1] == 'w'||\
							(phononsets[h][0] == 'g' && (strcmp(tempcodas[h-1][ntempcodas[h-1]-1],"g")) == 0) ||\
							phononsets[h][nphononsets[h]-1] == '8') {
							fp1 = fopen("wotdisio.txt","a");
							fprintf(fp1,"%s %s %d %d %s %s %d\n",GlobalWord,PhonString,ntempcodas[0],ntemponsets[1], tempcodas[0],temponsets[1],FindLetter('i'));
							fclose(fp1);

							if(h!=0) {
								strcpy(tempcodas[h-1][ntempcodas[h-1]], tempvowels[h]);
								tempcodasnum[h-1][ntempcodas[h-1]] = tempvowelsnum[h];
								ntempcodas[h-1]++;
							} else {		
								strcpy(temponsets[0][ntemponsets[0]], tempvowels[h]);
								temponsetsnum[0][ntemponsets[0]] = tempvowelsnum[h];
								ntemponsets[0]++;
							}

							for(i=h;i<ntempvowels-1;i++) {
								strcpy(tempvowels[i],tempvowels[i+1]);
								tempvowelsnum[i] = tempvowelsnum[i+1];
								for(j=0;j<ntempcodas[i+1];j++) {
									strcpy(tempcodas[i][j],tempcodas[i+1][j]);
									tempcodasnum[i][j] = tempcodasnum[i+1][j];
								}
								ntempcodas[i] = ntempcodas[i+1];
							}
							ntempcodas[ntempvowels-1] = 0;
							ntempvowels--;
						}
					}
				}
			}
		}
	}


	
	for(h=0;h<ntempvowels/*-1*/;h++) {	
		splitspot = -1;
		if(h == 100) {
			if (ntemponsets[0] < nphononsets[h]) {
				
			}
		} else {
			/*fp = fopen("SplitVowels.cods.txt","a");	
			fprintf(fp, "%d %d %d %d %s %s %s %s %s %d\n",h,ntempcodas[h-1],nphoncodas[h-1],nphononsets[h],GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
			fclose(fp);*/

			//if(ntempcodas[h-1] < (nphoncodas[h-1]+nphononsets[h])) {
				if((strcmp(tempvowels[h],"oui")) == 0 && phononsets[h][nphononsets[h]-1] == '8') splitspot = 1;	
				else if((strcmp(tempvowels[h],"oui")) == 0 && ntempcodas[h] == 0 && phonvowels[h+1] == 'i')splitspot = 2;
				else if((strcmp(tempvowels[h],"oua")) == 0 && ntempcodas[h] == 0 && phonvowels[h+1] == 'w')splitspot = 2;
				else if((strcmp(tempvowels[h],"ouai")) == 0 && ntempcodas[h] == 0 && phonvowels[h+1] == 'w')splitspot = 2;				
				else if((strcmp(tempvowels[h],"oué")) == 0 && ntempcodas[h] == 0 && phonvowels[h+1] == 'w')splitspot = 2;
				else if((strcmp(tempvowels[h],"ouè")) == 0 && ntempcodas[h] == 0 && phonvowels[h+1] == 'w')splitspot = 2;
				else if((strcmp(tempvowels[h],"ouâ")) == 0 && ntempcodas[h] == 0 && phonvowels[h+1] == 'w')splitspot = 2;		
				else if((strcmp(tempvowels[h],"ouon")) == 0 && ntempcodas[h] == 0 && phonvowels[h+1] == 'w')splitspot = 2;		
				else if((strcmp(tempvowels[h],"oueu")) == 0 && ntempcodas[h] == 0 && phonvowels[h+1] == 'w')splitspot = 2;		
				else if((strcmp(tempvowels[h],"oie")) == 0 && ntempcodas[h] == 0 && phonvowels[h+1] == 'w')splitspot = 1;					
				else if((strcmp(tempvowels[h],"oi")) == 0 && phononsets[h][nphononsets[h]-1] == 'w')splitspot = 1;				
				else if((strcmp(tempvowels[h],"ouâ")) == 0 && phononsets[h][nphononsets[h]-1] == 'w')splitspot = 2;				
				else if((strcmp(tempvowels[h],"oueu")) == 0 && phononsets[h][nphononsets[h]-1] == 'w')splitspot = 2;				
				else if((strcmp(tempvowels[h],"ouon")) == 0 && phononsets[h][nphononsets[h]-1] == 'w')splitspot = 2;			
				else if((strcmp(tempvowels[h],"ouhai")) == 0 && phononsets[h][nphononsets[h]-1] == 'w')splitspot = 2;			
				else if((strcmp(tempvowels[h],"ouai")) == 0 && phononsets[h][nphononsets[h]-1] == 'w')splitspot = 2;			
				else if((strcmp(tempvowels[h],"iau")) == 0 && phononsets[h][nphononsets[h]-1] == 'j')splitspot = 1;
				else if((strcmp(tempvowels[h],"io")) == 0 && phononsets[h][nphononsets[h]-1] == 'j')splitspot = 1;
				else if((strcmp(tempvowels[h],"ieu")) == 0 && phononsets[h][nphononsets[h]-1] == 'j')splitspot = 1;
				else if((strcmp(tempvowels[h],"oie")) == 0 && phononsets[h][nphononsets[h]-1] == 'j')splitspot = 1;
				else if((strcmp(tempvowels[h],"oie")) == 0 && phononsets[h][nphononsets[h]-1] == 'w')splitspot = 1;
				else if((strcmp(tempvowels[h],"eoi")) == 0 && phononsets[h][nphononsets[h]-1] == 'w')splitspot = 1;
				else if((strcmp(tempvowels[h],"ien")) == 0 && phononsets[h][nphononsets[h]-1] == 'j') splitspot = 1;	
				else if((strcmp(tempvowels[h],"ion")) == 0 && phononsets[h][nphononsets[h]-1] == 'j') splitspot = 1;		
				else if((strcmp(tempvowels[h],"uin")) == 0 && phononsets[h][nphononsets[h]-1] == 'j') splitspot = 1;		
				else if((strcmp(tempvowels[h],"ue")) == 0 && phononsets[h][nphononsets[h]-1] == '8')splitspot = 1;
				else if((strcmp(tempvowels[h],"ué")) == 0 && phononsets[h][nphononsets[h]-1] == '8')splitspot = 1;
				else if((strcmp(tempvowels[h],"ie")) == 0 && phononsets[h][nphononsets[h]-1] == 'j')splitspot = 1;	
				else if((strcmp(tempvowels[h],"oue")) == 0 && phononsets[h][nphononsets[h]-1] == 'w')splitspot = 2;
				else if((strcmp(tempvowels[h],"oué")) == 0 && phononsets[h][nphononsets[h]-1 ]== 'w')splitspot = 2;
				else if((strcmp(tempvowels[h],"ouè")) == 0 && phononsets[h][nphononsets[h]-1 ]== 'w')splitspot = 2;
				else if((strcmp(tempvowels[h],"oua")) == 0 && phononsets[h][nphononsets[h]-1] == 'w')splitspot = 2;
				else if((strcmp(tempvowels[h],"oui")) == 0 && phononsets[h][nphononsets[h]-1] == 'w')splitspot = 2;
				else if((strcmp(tempvowels[h],"iaie")) == 0 && phononsets[h][nphononsets[h]-1] == 'j') splitspot = 1;
				else if((strcmp(tempvowels[h],"ui")) == 0 && phononsets[h][nphononsets[h]-1] == '8')splitspot = 1;
				else if((strcmp(tempvowels[h],"ou")) == 0 && phononsets[h][nphononsets[h]-1] == '8')splitspot = 1;
				else if((strcmp(tempvowels[h],"ueu")) == 0 && phononsets[h][nphononsets[h]-1] == '8')splitspot = 1;
				else if((strcmp(tempvowels[h],"uin")) == 0 && phononsets[h][nphononsets[h]-1] == '8')splitspot = 2;		
				else if((strcmp(tempvowels[h],"aï")) == 0 && phononsets[h][nphononsets[h]-1] == 'j')splitspot = 1;
				else if((strcmp(tempvowels[h],"iei")) == 0 && phononsets[h][nphononsets[h]-1] == 'j')splitspot = 1;
		
		//	}


			if(splitspot != -1 ) {
				i = strlen(tempvowels[h]);
				strcpy(thebit2, tempvowels[h]+splitspot);
				thebit2[strlen(tempvowels[h]) - splitspot] = 0;
				strcpy(thebit1,tempvowels[h]);
				thebit1[splitspot] = 0;

					fp = fopen("SplitVowels.en.txt","a");	
					fprintf(fp, " %s %s %s %s %s %d\n",GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
					fclose(fp);

				if(/*h!=0*/1==1) {
					if(h!=0) {
						strcpy(tempcodas[h-1][ntempcodas[h-1]],thebit1);
						if(strlen(thebit1) > 1)	tempcodasnum[h-1][ntempcodas[h-1]] = findGrapheme(thebit1);
						else tempcodasnum[h-1][ntempcodas[h-1]] = NGRAPHEMES + FindLetter(thebit1[0]);
						ntempcodas[h-1]++;
					} else {
						if((strcmp(tempvowels[h],"ouhai")) == 0) {	
								fp = fopen("SplitVowels.ouhai.txt","a");	
								fprintf(fp, "1 %s %s %s %s %s %d\n",GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
								fclose(fp);
							strcpy(tempvowels[h],"ai");
							tempvowelsnum[h] = findGrapheme("ai");
							strcpy(temponsets[h][1],"ou");
							temponsetsnum[h][1] = findGrapheme("ou");
							strcpy(temponsets[h][2],"h");
							temponsetsnum[h][2] = NGRAPHEMES + FindLetter('h');	
							ntemponsets[h] = 3;
							//ntempvowels++ /* = 100*/;
							h=100;
							fp = fopen("SplitVowels.ouhai.txt","a");	
							fprintf(fp, "2 %s %s %s %s %s %d\n",GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
							fclose(fp);

						//	PrintWord();

						} else {
							strcpy(temponsets[0][ntemponsets[0]],thebit1);
							if(strlen(thebit1) > 1)	temponsetsnum[0][ntemponsets[0]] = findGrapheme(thebit1);
							else temponsetsnum[0][ntemponsets[0]] = NGRAPHEMES + FindLetter(thebit1[0]);
							ntemponsets[0]++;
						}
					}
					if((strcmp(tempvowels[h],"iaie")) == 0) {
						strcpy(tempvowels[h],"ai");
						tempvowelsnum[h] = findGrapheme("ai");
						strcpy(tempcodas[h][0],"en");
						tempcodasnum[h][0] = findGrapheme("en");
					} else if ((strcmp(tempvowels[h],"ouhai")) == 0) {
								//do nothing fixed above.
					} else if ( ( (thebit2[strlen(thebit2)-1] == 'i') || (thebit2[strlen(thebit2)-1] == 'e')|| (thebit2[strlen(thebit2)-1] == 'o')) && \
							((strcmp(tempcodas[h][0],"n")) == 0) ) {
								
							fp = fopen("SplitVowels.in.txt","a");	
							fprintf(fp, " %s %s %s %s %s %d\n",GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
							fclose(fp);
							//croient, zoint
							
							if(strlen(thebit2)>1) {
								fp = fopen("SplitVowels.in.1.txt","a");	
								fprintf(fp, " %s %s %s %s %s %d\n",GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
								fclose(fp);
								strcpy(tempvowels[h],thebit2);
								thebit1[0] = tempvowels[h][strlen(tempvowels[h])-1];
								thebit1[1] = 'n';
								thebit1[2] = 0; //en

								tempvowels[h][strlen(tempvowels[h])-1] = 0;
								if(strlen(tempvowels[h]) == 1) tempvowelsnum[h] = NGRAPHEMES + FindLetter(tempvowels[h][0]);
								else findGrapheme(tempvowels[h]);

								strcpy(tempcodas[h][0],thebit1);
								tempcodasnum[h][0] = findGrapheme(thebit1);
							} else {
								fp = fopen("SplitVowels.in.2.txt","a");	
								fprintf(fp, " %s %s %s %s %s %d\n",GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
								fclose(fp);

								thebit2[1] = 'n';
								thebit2[2] = 0;
								strcpy(tempvowels[h],thebit2);
								if(strlen(tempvowels[h]) == 1) tempvowelsnum[h] = NGRAPHEMES + FindLetter(tempvowels[h][0]);
								else tempvowelsnum[h] = findGrapheme(tempvowels[h]);
								for(i=0;i<ntempcodas[h]-1;i++) {
									strcpy(tempcodas[h][i],tempcodas[h][i+1]);
									tempcodasnum[h][i] = tempcodasnum[h][i+1];
								}
								ntempcodas[h]--;
							}			
					} else {
						strcpy(tempvowels[h],thebit2);
						if(strlen(thebit2) > 1) tempvowelsnum[h] = findGrapheme(thebit2);
						else tempvowelsnum[h] = NGRAPHEMES + FindLetter(thebit2[0]);
					}
					fp = fopen("SplitVowels.txt","a");	
					fprintf(fp, "%c %s %s %s %s %s %s %d\n",thebit2[strlen(thebit2)-1], thebit2, GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
					fclose(fp);

				} else {
					strcpy(temponsets[0][ntemponsets[0]],thebit1);
					if(strlen(thebit1) > 1)	temponsetsnum[0][ntemponsets[0]] = findGrapheme(thebit1);
					else temponsetsnum[0][ntemponsets[0]] = NGRAPHEMES + FindLetter(thebit1[0]);
					ntemponsets[0]++;

					strcpy(tempvowels[h],thebit2);
					if(strlen(thebit2) > 1) tempvowelsnum[h] = findGrapheme(thebit2);
					else tempvowelsnum[h] = NGRAPHEMES + FindLetter(thebit2[0]);

					fp = fopen("SplitVowels.first.txt","a");	
					fprintf(fp, " %s %s %s %s %s %d\n",GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
					fclose(fp);
					PrintWord();
				}
			} 

			//aien
			//affluence split vowels!
			if( ((strcmp(tempvowels[h],"ue")) == 0 && phonvowels[h] == 'y' && (strcmp(tempcodas[h][0],"n"))==0 ) ) {		
				if(h!=0) {
					//a.ff.l.ue.nce.ment
					for(i=ntempvowels;i>h+1;i--) {
						strcpy(tempvowels[i],tempvowels[i-1]);
						tempvowelsnum[i] = tempvowelsnum[i-1];
						for(j=0;j<ntempcodas[i-1];j++) {
							strcpy(tempcodas[i][j],tempcodas[i-1][j]);
							tempcodasnum[i][j] = tempcodasnum[i-1][j];
						}
						ntempcodas[i] = ntempcodas[i-1];
					}

					strcpy(tempvowels[h],"u");
					tempvowelsnum[h] = NGRAPHEMES + FindLetter('u');

					strcpy(tempvowels[h+1],"en");
					tempvowelsnum[h+1] = findGrapheme("en");
					for(j=0;j<ntempcodas[h]-1;j++) {
						strcpy(tempcodas[h+1][j],tempcodas[h][j+1]);
						tempcodasnum[h+1][j] = tempcodasnum[h][j+1];
					}
					ntempcodas[h+1] = ntempcodas[h] - 1;
					ntempcodas[h] = 0;
					ntempvowels++;
									
					fp = fopen("SplitVowels.en.txt","a");	
					fprintf(fp, " %s %s %s %s %s %d\n",GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
					fclose(fp);
				}
			}	
		}
	}
//	PrintWordName("test.ouhai.txt");
	
	for(h=0;h<ntempvowels;h++) {
		splitspot = -1;
		if((strcmp(tempvowels[h],"oue")) == 0 && phonvowels[h] == 'u') splitspot = 2;
		else if( ((strcmp(tempvowels[h],"ouai")) == 0)/*&& phonvowels[h] == 'u'*/)splitspot = 2;
		else if( ((strcmp(tempvowels[h],"ouai")) == 0) /*&& phonvowels[h] == 'u'*/)splitspot = 2;
		else if( ((strcmp(tempvowels[h],"ie")) == 0) && ((strcmp(tempcodas[h][0],"n")) == 0) && h == ntempvowels-1 && ntempcodas[h] == 2) splitspot = 1; //cient
		else if( ((strcmp(tempvowels[h],"ie")) == 0) && ((strcmp(tempcodas[h][0],"s")) == 0) &&  h == ntempvowels-1 && ntempcodas[h] == 1) splitspot = 1; //ies
		else if( ((strcmp(tempvowels[h],"ie")) == 0) && ((strcmp(tempvowels[h+1],"u")) == 0) && tempcodas[h] == 0) splitspot = 1;	
		else if( ((strcmp(tempvowels[h],"ue")) == 0) && phonvowels[h] == 'y' && phonvowels[h+1] == 'E' && (nphoncodas[h]+nphononsets[h+1]) == 0) splitspot = 1;	
		else if( ((strcmp(tempvowels[h],"ueu")) == 0) && phonvowels[h] == 'y' && phonvowels[h+1] == '2' && (nphoncodas[h]+nphononsets[h+1]) == 0) splitspot = 1;	
		else if((strcmp(tempvowels[h],"io")) == 0 && phononsets[h+1][0] == 'j' && phonvowels[h+1] == 'O')splitspot = 1;
		
		if(ntempvowels<nphonvowels || splitspot != -1) {
			if((strcmp(tempvowels[h],"eui")) == 0 && phonvowels[h] == '2') splitspot = 2;
			else if((strcmp(tempvowels[h],"ué")) == 0 && phonvowels[h] == 'y' && phonvowels[h+1] == 'e') splitspot = 1;
			else if((strcmp(tempvowels[h],"ue")) == 0 && phonvowels[h] == 'y') splitspot = 1;
			else if((strcmp(tempvowels[h],"oueu")) == 0 /*&& phonvowels[h] == 'u'*/)splitspot = 2;
			else if((strcmp(tempvowels[h],"ouon")) == 0 /*&& phonvowels[h] == 'u'*/)splitspot = 2;
			else if((strcmp(tempvowels[h],"ouhai")) == 0 /*&& phonvowels[h] == 'u'*/)splitspot = 2;	
			else if((strcmp(tempvowels[h],"oue")) == 0 && phonvowels[h] == 'u')splitspot = 2;
			else if((strcmp(tempvowels[h],"oué")) == 0 && phonvowels[h] == 'u')splitspot = 2;
			else if((strcmp(tempvowels[h],"ou")) == 0 && phonvowels[h] == 'é')splitspot = 2;
			else if((strcmp(tempvowels[h],"oui")) == 0 && phonvowels[h] == 'u')splitspot = 2;
			else if((strcmp(tempvowels[h],"oie")) == 0 && phonvowels[h] == 'w')splitspot = 1;
			else if((strcmp(tempvowels[h],"aï")) == 0 && phonvowels[h] == 'a')splitspot = 1;
			else if((strcmp(tempvowels[h],"oua")) == 0 && phonvowels[h] == 'u')splitspot = 2;//clouant
			else if((strcmp(tempvowels[h],"oueu")) == 0 && phonvowels[h] == 'u')splitspot = 2;
			else if((strcmp(tempvowels[h],"aou")) == 0 && phonvowels[h] == 'u')splitspot = 1;
			else if((strcmp(tempvowels[h],"aou")) == 0 && phonvowels[h] == 'a')splitspot = 1;
			else if((strcmp(tempvowels[h],"ée")) == 0 && phonvowels[h] == 'a')splitspot = 1;
			else if((strcmp(tempvowels[h],"ée")) == 0 && phonvowels[h] == 'e')splitspot = 1;
			else if((strcmp(tempvowels[h],"ouhai")) == 0 && phonvowels[h] == 'e')splitspot = 2;
			else if((strcmp(tempvowels[h],"iei")) == 0 && phonvowels[h] == 'e')splitspot = 1;
			else if((strcmp(tempvowels[h],"oe")) == 0 && phonvowels[h] == 'e')splitspot = 1;
			else if((strcmp(tempvowels[h],"ué")) == 0 && phononsets[h][nphononsets[h]-1] == '8')splitspot = 1;
			else if((strcmp(tempvowels[h],"iau")) == 0 && phononsets[h+1][0] == 'j')splitspot = 1;	
			else if((strcmp(tempvowels[h],"iaie")) == 0 && phononsets[h+1][0] == 'j')splitspot = 1;	
			else if((strcmp(tempvowels[h],"ieu")) == 0 && phononsets[h+1][0] == 'j')splitspot = 1;
			else if((strcmp(tempvowels[h],"ie")) == 0 && phononsets[h+1][0] == 'j' && phonvowels[h+1] == '@')splitspot = 1;		
			else if((strcmp(tempvowels[h],"ion")) == 0 && phononsets[h+1][0] == 'j')splitspot = 1;
			else if((strcmp(tempvowels[h],"io")) == 0 && phononsets[h+1][0] == 'j')splitspot = 1;
			else if((strcmp(tempvowels[h],"ie")) == 0 && (phononsets[h+1][1] == 'j' ))splitspot = 1;
			else if((strcmp(tempvowels[h],"aï")) == 0 && (phononsets[h+1][0] == 'j' || phoncodas[h][0] == 'j') && (nphoncodas[h] == 1 || nphononsets[h+1] == 1))splitspot = 1;
			else if((strcmp(tempvowels[h],"ieu")) == 0 && phononsets[h][nphononsets[h]-1] == 'j')splitspot = 1;
			else if((strcmp(tempvowels[h],"oua")) == 0 && nphoncodas[h] == 0 && phonvowels[h] == 'u')splitspot = 2;
			else if((strcmp(tempvowels[h],"ouè")) == 0 && nphoncodas[h] == 0 && phonvowels[h] == 'u')splitspot = 2;
			else if((strcmp(tempvowels[h],"eui")) == 0 && nphoncodas[h] == 0 && phonvowels[h] == 'u')splitspot = 2;
		

			if(splitspot != -1) {
				strcpy(thebit2, tempvowels[h]+splitspot);
				strcpy(thebit1, tempvowels[h]);
				thebit1[splitspot] = 0;
				
				fp = fopen("SplitVowels.two.txt","a");	
				fprintf(fp, " %s %s %s %s %s %d\n",GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
				fclose(fp);
								
				if( (strcmp(tempvowels[h],"ue") == 0 && strcmp(tempcodas[h][0],"n") == 0) || //
					(strcmp(tempvowels[h],"iaie") == 0 && strcmp(tempcodas[h][0],"n") == 0) ) {		
					
					fp = fopen("SplitVowels.iaie.txt","a");	
					fprintf(fp, " %s %s %s %s %s \n",GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2);
					fclose(fp);

					for(i=ntempvowels;i>h;i--) {
						strcpy(tempvowels[i],tempvowels[i-1]);
						tempvowelsnum[i] = tempvowelsnum[i-1];
						for(j=0;j<ntempcodas[i-1];j++) {
							strcpy(tempcodas[i][j],tempcodas[i-1][j]);
							tempcodasnum[i][j] = tempcodasnum[i-1][j];
						}
						ntempcodas[i] = ntempcodas[i-1];
					}		
					ntempcodas[h] = 0;
					ntempvowels++;
					//fluence f.l.u.0.n.ce
					if(strcmp(tempvowels[h],"u") == 0) {
						strcpy(tempvowels[h],"u");
						tempvowelsnum[h] = NGRAPHEMES + FindLetter('u');
						strcpy(tempvowels[h+1],"en");
						tempvowelsnum[h+1] = findGrapheme("en");
						for(i=0;i<ntempcodas[h+1]-1;i++) {
							strcpy(tempcodas[h+1][i],tempcodas[h+1][i+1]);
							tempcodasnum[h+1][i] = tempcodasnum[h+1][i+1];
						}
						ntempcodas[h+1]--;
					} else {
						strcpy(tempvowels[h],"i");
						tempvowelsnum[h] = NGRAPHEMES + FindLetter('i');
						ntempcodas[h] = 0;
						ntemponsets[h+1] = 0;
						strcpy(tempvowels[h+1],"ai");
						tempvowelsnum[h+1] = findGrapheme("ai");
						strcpy(tempcodas[h+1][0],"en");
						tempcodasnum[h+1][0] = findGrapheme("en");
						strcpy(tempcodas[h+1][1],"t");
						tempcodasnum[h+1][1] = NGRAPHEMES + FindLetter('t');
						ntempcodas[h+1] = 2;
					}
		
					
					
				} /*else if((strcmp(tempvowels[h],"aï"))==0) {
					strcpy(tempcodas[h][0],"ï");
					tempcodasnum[h][0] = NGRAPHEMES + FindLetter('ï');

					fp = fopen("SplitVowels.two.ai.txt","a");	
					fprintf(fp, " %s %s %s %s %s %d\n",GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
					fclose(fp);
				} */ else if ( (tempvowels[h][strlen(tempvowels[h])-1] == 'i' || tempvowels[h][strlen(tempvowels[h])-1] == 'e') && \
							((strcmp(tempcodas[h][0],"n")) == 0)) {
					
					bit1[0] = tempvowels[h][strlen(tempvowels[h])-1];
					bit1[1] = 'n';
					bit1[2] = 0; 

						//client.elle
					for(i=ntempvowels;i>h+1;i--) {
						strcpy(tempvowels[i],tempvowels[i-1]);
						tempvowelsnum[i] = tempvowelsnum[i-1];
						for(j=0;j<ntempcodas[i-1];j++) {
							strcpy(tempcodas[i][j],tempcodas[i-1][j]);
							tempcodasnum[i][j] = tempcodasnum[i-1][j];
						}
						ntempcodas[i] = ntempcodas[i-1];
					}		

					for(j=1;j<ntempcodas[h];j++) {
						strcpy(tempcodas[h+1][j-1],tempcodas[h][j]);
						tempcodasnum[h+1][j-1] = tempcodasnum[h][j];
					}
					ntempcodas[h+1] = ntempcodas[h]-1;
					ntempcodas[h] = 0;  //cl.i.[en].t [2]
					strcpy(tempvowels[h+1],bit1);
					tempvowelsnum[h+1] = findGrapheme(bit1);
					//cli[h].ent[h+1],elle[h+2]
					ntempvowels++;

						strcpy(tempvowels[h],thebit1);
						if(strlen(thebit1) > 1)	tempvowelsnum[h] = findGrapheme(thebit1);
						else tempvowelsnum[h] = NGRAPHEMES + FindLetter(thebit1[0]);

					fp = fopen("SplitVowels.two.2.txt","a");	
					fprintf(fp, " %s %s %s %s %s %d\n",GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
					fclose(fp);
				} else if ((strcmp(tempvowels[h],"ie")) == 0 && (strcmp(tempcodas[h][0],"s")) == 0) {
						strcpy(tempvowels[h],"i");
						strcpy(tempcodas[h][0],"e");
						strcpy(tempcodas[h][1],"s");
						ntempcodas[h] = 2;
						tempvowelsnum[h] = NGRAPHEMES + FindLetter('i');
						tempcodasnum[h][0] = NGRAPHEMES + FindLetter('e');
						tempcodasnum[h][1] = NGRAPHEMES + FindLetter('s');
					
				} /*else if((strcmp(tempvowels[h],"ie")) == 0 && ((strcmp(tempvowels[h+1],"u") == 0)) && ntempcodas[h] == 0) {
					strcpy(tempcodas[h-1][ntempcodas[h]-1],"i");
					strcpy(tempvowels[h],"eu");
					tempvowelsnum[h] = NGRAPHEMES + FindLetter('i');
					tempcodasnum[h-1][ntempcodas[h]-1] = NGRAPHEMES + FindLetter('i');
					tempvowelsnum[h] = findGrapheme("eu");
					ntempcodas[h-1]++;			
				}*/ else {  /* clouai */
					for(i=ntempvowels;i>h;i--) {
						strcpy(tempvowels[i],tempvowels[i-1]);
						tempvowelsnum[i] = tempvowelsnum[i-1];
						for(j=0;j<ntempcodas[i-1];j++) {
							strcpy(tempcodas[i][j],tempcodas[i-1][j]);
							tempcodasnum[i][j] = tempcodasnum[i-1][j];
						}
						ntempcodas[i] = ntempcodas[i-1];
					}	
					ntempcodas[h] = 0;

			
					strcpy(tempvowels[h+1],thebit2);
					if(strlen(thebit2) > 1)	tempvowelsnum[h+1] = findGrapheme(thebit2);
					else tempvowelsnum[h+1] = NGRAPHEMES + FindLetter(thebit2[0]);

					strcpy(tempvowels[h],thebit1);
					if(strlen(thebit1) > 1)	tempvowelsnum[h] = findGrapheme(thebit1);
					else tempvowelsnum[h] = NGRAPHEMES + FindLetter(thebit1[0]);

					ntempvowels++;		
				}
				fp = fopen("SplitVowels.two.txt","a");	
				fprintf(fp, " %s %s %s %s %s %d\n",GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
				fclose(fp);
			}
		}
	}

    
	// fix up ambiguous consonants in graphemes
	//PrintWord();
	for(h=0;h<ntempvowels;h++) {
		if(1== 1) {
			i = strlen(tempvowels[h]);
			theletter = tempvowels[h][i-1];
			if( ((tempvowels[h][i-1] == 'm' && (((phononsets[h+1][0] == 'm' && h+1<ntempvowels && nphoncodas[h] == 0)) || (phoncodas[h][0] == 'm' && nphoncodas[h]>0) || (phononsets[h+1][0] == 'm' && nphononsets[h+1] > 0 && nphonvowels > h && nphoncodas[h] == 0))) || \
				(tempvowels[h][i-1] == 'r' && (((phononsets[h+1][0] == 'R' && h+1<ntempvowels && nphoncodas[h] == 0)) || (phoncodas[h][0] == 'R' && nphoncodas[h]>0)  || (phononsets[h+1][0] == 'R' && nphononsets[h+1] > 0 && nphonvowels > h && nphoncodas[h] == 0))) || \
				(tempvowels[h][i-1] == 'y' && phononsets[h+1][0] == 'j' && (strlen(tempvowels[h]) > 1) ) || \
				(tempvowels[h][i-1] == 'n' && (((phononsets[h+1][0] == 'n' && h+1<ntempvowels && nphoncodas[h] == 0)) || (phoncodas[h][0] == 'n' && nphoncodas[h]>0)  || (phononsets[h+1][0] == 'n' && nphononsets[h+1] > 0 && nphonvowels > h && nphoncodas[h] == 0)))) || \
				(tempvowels[h][i-1] == 'n' &&  ((strcmp(tempcodas[h][0],"e")) == 0) && ntempcodas[h] > 0) ||
				(tempvowels[h][i-1] == 'n' && ntempcodas[h] == 0 && (strcmp(tempvowels[h+1],"e")) == 0 && h< ntempvowels-1) && \
				/*!(ntempcodas[h] > 0 && (tempvowels[h][i-1] == tempcodas[h][0][0]))*/1==1 ) {
		
				//MAKE SPOT FOR NEW
				for(i=ntempcodas[h];i>0;i--) {
					strcpy(tempcodas[h][i],tempcodas[h][i-1]);
					tempcodasnum[h][i] = tempcodasnum[h][i-1];
				}
				ntempcodas[h]++;

				//FIX VOWEL
				i = strlen(tempvowels[h]);
				tempvowels[h][i-1] = 0;
				if((strlen(tempvowels[h])) != 1) tempvowelsnum[h] = findGrapheme(tempvowels[h]);
				else tempvowelsnum[h] = NGRAPHEMES + FindLetter(tempvowels[h][0]);
				
				//ADD NEW CONSONANT.
				tempcodas[h][0][0] = theletter;
				tempcodas[h][0][1] = 0;
				tempcodasnum[h][0] = NGRAPHEMES + FindLetter(tempcodas[h][0][0]); 
				//PrintWord();

				//ntempcodas[h] = 1;
				//algerienne.
				for(i=0;i<ntempcodas[h];i++) {
					if(i+1<ntempcodas[h]) {
						if(strlen(tempcodas[h][i]) == 1) {
							if(strcmp(tempcodas[h][i],tempcodas[h][i+1])== 0) {
								bit1[0] = tempcodas[h][i][0];  //double letter
								bit1[1] = tempcodas[h][i][0];
								bit1[2] = 0;

								strcpy(tempcodas[h][i],bit1);
								tempcodasnum[h][i] = findGrapheme(tempcodas[h][i]);
								for(j=i+1;j<ntempcodas[h];j++) {
									strcpy(tempcodas[h][j],tempcodas[h][j+1]);
									tempcodasnum[h][j] = tempcodasnum[h][j+1];
								}
								ntempcodas[h]--;
								fp = fopen("rejigs.txt","a");	
								fprintf(fp, " %s %s %s %c %c %c\n",GlobalWord, tempvowels[h], tempcodas[h],phononsets[h+1][0],phoncodas[h][0],phononsets[h][0]);
								fclose(fp);
								PrintWord();
							}
						}
					}
				}
				fp = fopen("FixedDVowels.txt","a");	
				fprintf(fp, " %s %s %s %c %c %c\n",GlobalWord, tempvowels[h], tempcodas[h],phononsets[h+1][0],phoncodas[h][0],phononsets[h][0]);
				fclose(fp);
			}
		}
	}
	//nasal plus double n.
	for(h=0;h<ntempvowels;h++) {
		i = strlen(tempvowels[h]); //9n.
		if( tempvowels[h][i-1] == 'm'||tempvowels[h][i-1] == 'r'||tempvowels[h][i-1] == 'n') {
			if(ntempcodas[h] > 0) { //annexe
				if(tempvowels[h][i-1] == tempcodas[h][0][0]) {
					if(h==ntempvowels-1) {
						/*if(ntempcodas[h] > nphoncodas[h]) */ok = 1;
					} else {
						/*if(ntempcodas[h] > nphoncodas[h] + nphononsets[h+1])*/ ok = 1;
					}
					if(ok == 1) {
						if(strcmp(tempvowels[h],"ion") ==0) {
							for(j=ntempvowels;j<h;j--) {
								strcpy(tempvowels[h],tempvowels[h-1]);
								tempvowelsnum[h] = tempvowelsnum[h-1];
								for(k=0;k<ntempcodas[j-1];k++){
									strcpy(tempcodas[h][k],tempcodas[h-1][k]);
									tempcodasnum[h][k] = tempcodasnum[h-1][k];
								}
								ntempcodas[h] = ntempcodas[h-1];
							}
							strcpy(tempvowels[h],"i");
							strcpy(tempvowels[h+1],"o");

							tempcodas[h+1][0][1] = tempcodas[h][0][0]; //double letter
							tempcodas[h+1][0][2] = 0;
							tempcodasnum[h+1][0] = findGrapheme(tempcodas[h][0]);	
							ntempcodas[h] = 0;
					
						} else {
							tempvowels[h][i-1] = 0;
							if(strlen(tempvowels[h]) == 1) tempvowelsnum[h] = NGRAPHEMES + FindLetter(tempvowels[h][0]);
							else tempvowelsnum[h] = findGrapheme(tempvowels[h]);			
							tempcodas[h][0][1] = tempcodas[h][0][0]; //double letter
							tempcodas[h][0][2] = 0;
							tempcodasnum[h][0] = findGrapheme(tempcodas[h][0]);	
						}
						fp = fopen("DOubleVowels.txt","a");	
						fprintf(fp, " %s %s %s %c %c %c\n",GlobalWord, tempvowels[h], tempcodas[h],phononsets[h+1][0],phoncodas[h][0],phononsets[h][0]);
						fclose(fp);
					}												
				}
			} 
		}
	}
	//ue, ae, oe
	if( (strcmp(tempvowels[ntempvowels-1],"aie")) == 0 || \
		(strcmp(tempvowels[ntempvowels-1],"oue")) == 0	||\
		(strcmp(tempvowels[ntempvowels-1],"ue")) == 0	||\
		(strcmp(tempvowels[ntempvowels-1],"ae")) == 0	||\
		(strcmp(tempvowels[ntempvowels-1],"oe")) == 0	||\
		(strcmp(tempvowels[ntempvowels-1],"oie")) == 0	) {  /*will need rejiggin to n-1 vowels */
		
		if( ((strcmp(tempcodas[ntempvowels-1][0],"n")) == 0) && ntempcodas[ntempvowels-1] > 0) {
			strcpy(bit1,tempvowels[ntempvowels-1]);
			bit1[strlen(tempvowels[ntempvowels-1])-1] = 0;
			strcpy(tempvowels[ntempvowels-1],bit1);
			if(strlen(bit1) > 1) tempvowelsnum[ntempvowels-1] = findGrapheme(bit1);
			else tempvowelsnum[ntempvowels-1] = NGRAPHEMES + FindLetter(bit1[0]);
			strcpy(tempcodas[ntempvowels-1][0],"en");
			tempcodasnum[ntempvowels-1][0] = findGrapheme("en");
			//PrintWord();
			fp1 = fopen("aie.double.txt","a");
			fprintf(fp1,"%s %d %s %s %d %d %s %s\n",tempvowels[ntempvowels-1],h,tempvowels[h],GlobalWord,ntempcodas[0],ntemponsets[1], tempcodas[0],temponsets[1]);
			fclose(fp1);
		}
	}

	for(h=0;h<ntempvowels-1;h++) {
		ok = 0;
		//if(ntempvowels==nphonvowels) {
			if( ((ConsonantNum(tempvowels[h][strlen(tempvowels[h])-1])) != -1) && ntempcodas[h] == 0) {
					strcpy(bit1,tempvowels[h]);
					bit1[strlen(tempvowels[h])-1] = 0;
					bit2[0] = tempvowels[h][strlen(tempvowels[h])-1];
					bit2[1] = 0;

					strcpy(tempvowels[h],bit1);
					if(strlen(tempvowels[h]) == 1) tempvowelsnum[h] = NGRAPHEMES + FindLetter(bit1[0]);
					else  tempvowelsnum[h] = findGrapheme(bit1);
	
					strcpy(tempcodas[h][0],bit2);
					tempcodasnum[h][0] = NGRAPHEMES + FindLetter(bit2[0]);
					ntempcodas[h] = 1;
					//PrintWordName("test1.1.1.txt");
					fp1 = fopen("nocods1.double.txt","a");
					fprintf(fp1,"[%s] %d %s %s %d %d %s %s\n",tempcodas[h][0],h,tempvowels[h],GlobalWord,ntempcodas[0],ntemponsets[1], tempcodas[0],temponsets[1]);
					fclose(fp1);	
			}
	}

	//acheminent
	h = ntempvowels-1;
	i = strlen(tempvowels[h]);
	if(i > 1 && ntempcodas[h] > 0) {
			if(ConsonantNum(tempvowels[h][i-1]) != -1 && ConsonantNum(tempcodas[h][0][0]) == -1) {
					bit1[0] = tempvowels[h][0];
					bit1[1] = 0; //e
					bit2[0] = tempvowels[h][1];
					bit2[1] = 0; //n
					
					strcpy(tempvowels[h],bit1);
					tempvowelsnum[h] = NGRAPHEMES + FindLetter(bit1[0]);
				
					for(j=ntempcodas[h];j>0;j--) {
						strcpy(tempcodas[h][j], tempcodas[h][j-1]);
						tempcodasnum[h][j] = tempcodasnum[h][j-1];
					}
					ntempcodas[h]++;
					strcpy(tempcodas[h][0],bit2);
					tempcodasnum[h][0] = NGRAPHEMES + FindLetter(bit2[0]);

				
					fp1 = fopen("enem.end.txt","a");
					fprintf(fp1,"%d %s %s %d %d %s %s\n",h,tempvowels[h],GlobalWord,ntempcodas[0],ntemponsets[1], tempcodas[0],temponsets[1]);
					fclose(fp1);
			}
	}


//	abattement.
	//a.b..ai.ss..e.r..ai...en.t
	//acceptera  .a.cc..e.pt..e.r..a..  ***ak***s**Ep***tR*a************
	//acheminent (3)
	//for(h=0;h<ntempvowels;h++) {	
	

	//b,bb,d,dd,qu, s, ss,m,mm,l,ll, cqu, ch,d,t,tt,v,g,gu,sc,n,nn,f,ff,pp,p,dm,gn,r,c
	for(h=ntempvowels-2;h>=0;h--) {	
	//for(h=1;h<ntempvowels-1;h++) {
		//if(((strcmp(tempvowels[h],"e")) == 0) || ((strcmp(tempvowels[h],"em")) == 0) ||((strcmp(tempvowels[h],"en")) == 0) ){
		if((strcmp(tempvowels[h],"e")) == 0) {
			ok = 0;
		
			if(h!=0 ) { //accentuerent
				if(  ntempvowels > nphonvowels && \
				( (ntempcodas[h-1] == 1 && ntempcodas[h] == 1) || \
				  (ntempcodas[h-1] == 2 && ntempcodas[h] == 1 && (strcmp(tempcodas[h][0],"r")) == 0) || \
				  (ntempcodas[h-1] == 1 && ntempcodas[h] == 2 && (strcmp(tempcodas[h][0],"v")) == 0) || \
				  (ntempcodas[h-1] == 0 && ntempcodas[h] == 1 && (strcmp(tempcodas[h][0],"r")) == 0) || \
				  (ntempcodas[h-1] == 1 && (strlen(tempvowels[h]) == 2)) ))  ok=1;
				
			} else {
				if(ntempvowels==nphonvowels) {
					if( ((strcmp(tempvowels[h],"em")) == 0) && ((strcmp(tempcodas[h][0],"en")) == 0)/* && (phononsets[h+1][0] == 'm' || phononsets[h+1][0] == 'n')*/)ok = 1;
				}
				if(  ntempvowels > nphonvowels && \
				( (ntemponsets[0] == 1 && ntempcodas[h] == 1) || \
				  (ntemponsets[0] == 2 && ntempcodas[h] == 1 && (strcmp(tempcodas[h][0],"r")) == 0) || \
				  (ntemponsets[0] == 1 && strlen(tempvowels[h]) == 2) ))  ok=1;		
			}

			if(ok == 1) {
				if(ntempcodas[h-1] > 0) {
					int minuses = 0;
					if(phononsets[h][nphononsets[h]-1] == 'w' || phononsets[h][nphononsets[h]-1] == 'j' || phononsets[h][nphononsets[h]-1] == '8') minuses = 1;
			
					if(h!=0 ) {			
						if((nphoncodas[h-1]+nphononsets[h]-minuses<2)) {
							ok = 0;
							fp1 = fopen("sr.double.ecount2.rejected.txt","a");
							fprintf(fp1,"%s %d %d %d %d %d\n", GlobalWord, ntempcodas[h-1], ntempcodas[h], nphoncodas[h-1], nphononsets[h], minuses);
							fclose(fp1);
						}
					} else {
						if(nphononsets[0] - minuses < 2) {
							ok = 0;
							fp1 = fopen("sr.double.ecount2.rejected.txt","a");
							fprintf(fp1,"%s %d %d %d %d \n", GlobalWord, ntemponsets[0],ntempcodas[0],nphononsets[0],minuses );	
							fclose(fp1);
						}
					}
				}
			}

			if(ok==1) {
				int e_count = 0;
				for(i=0;i<ntempvowels-1;i++) {
					if(tempvowels[i][0] == 'e' && strlen(tempvowels[i]) == 1) e_count++;
				}
				fp1 = fopen("sr.double.ecount2.txt","a");
				fprintf(fp1,"%d %d %d %s %s %d %d %s %s\n",e_count, ntempvowels, h,tempvowels[h],GlobalWord,ntempcodas[0],ntemponsets[1], tempcodas[0],temponsets[1]);
				fclose(fp1);

				if(h>0) {
					if(strlen(tempvowels[h]) == 1) {
						strcpy(tempcodas[h-1][ntempcodas[h-1]], tempvowels[h]);
						tempcodasnum[h-1][ntempcodas[h-1]] = tempvowelsnum[h];
						ntempcodas[h-1]++;
					} else {  //baudelarian
						strcpy(bit1,"e");
						bit2[0] = tempvowels[h][1];
						bit2[1] = 0;
						strcpy(tempcodas[h-1][ntempcodas[h-1]], bit1);
						tempcodasnum[h-1][ntempcodas[h-1]] = NGRAPHEMES + FindLetter('e');;
						ntempcodas[h-1]++;
						strcpy(tempcodas[h-1][ntempcodas[h-1]], bit2);
						tempcodasnum[h-1][ntempcodas[h-1]] = NGRAPHEMES + FindLetter(bit2[0]);
						ntempcodas[h-1]++;
					}
				
					for(i=0;i<ntempcodas[h];i++) {
						strcpy(tempcodas[h-1][ntempcodas[h-1]],tempcodas[h][i]);
						tempcodasnum[h-1][ntempcodas[h-1]] = tempcodasnum[h][i];
						ntempcodas[h-1]++;
					}
					for(i=h;i<ntempvowels-1;i++) {
						strcpy(tempvowels[i],tempvowels[i+1]);
						tempvowelsnum[i] = tempvowelsnum[i+1];
						for(j=0;j<ntempcodas[i+1];j++) {
							strcpy(tempcodas[i][j],tempcodas[i+1][j]);
							tempcodasnum[i][j] = tempcodasnum[i+1][j];
						}
						ntempcodas[i] = ntempcodas[i+1];
					}
					ntempvowels--;	
				} else {
					if(strlen(tempvowels[h]) == 1) {
						strcpy(temponsets[0][ntemponsets[0]], tempvowels[h]);
						temponsetsnum[0][ntemponsets[0]] = tempvowelsnum[h];
						ntemponsets[0]++;
					} else {
						strcpy(bit1,"e");
						bit2[0] = tempvowels[h][1];
						bit2[1] = 0;
						strcpy(temponsets[0][ntemponsets[0]], bit1);
						temponsetsnum[0][ntemponsets[0]] = NGRAPHEMES + FindLetter('e');;
						ntemponsets[0]++;
						strcpy(temponsets[0][ntemponsets[0]], bit2);
						temponsetsnum[0][ntemponsets[0]] = NGRAPHEMES + FindLetter(bit2[0]);
						ntemponsets[0]++;
					}
				
					for(i=0;i<ntempcodas[h];i++) {
						strcpy(temponsets[0][ntemponsets[0]],tempcodas[h][i]);
						temponsetsnum[0][ntemponsets[0]] = tempcodasnum[h][i];
						ntemponsets[0]++;
					}
					for(i=h;i<ntempvowels-1;i++) {
						strcpy(tempvowels[i],tempvowels[i+1]);
						tempvowelsnum[i] = tempvowelsnum[i+1];
						for(j=0;j<ntempcodas[i+1];j++) {
							strcpy(tempcodas[i][j],tempcodas[i+1][j]);
							tempcodasnum[i][j] = tempcodasnum[i+1][j];
						}
						ntempcodas[i] = ntempcodas[i+1];
					}
					ntempcodas[ntempvowels-1] = 0;
					ntempvowels--;
				}
				fp1 = fopen("sr.double.txt","a");
				fprintf(fp1," %s %d %s %s %d %d %s %s\n",tempcodas[h][0],h,tempvowels[h],GlobalWord,ntempcodas[0],ntemponsets[1], tempcodas[0],temponsets[1]);
				fclose(fp1);
				PrintWord();		
			}
		}
	}
	//PrintWord();
	//kill end e letters as they may be silent. developement.
	if((ntempvowels > nphonvowels && ntempvowels >= 2)) {
		
	}
	
	if((ntempvowels > nphonvowels && ntempvowels >= 2)) {
		if( ((strcmp(tempvowels[ntempvowels-1],"e")) == 0 || \
			(strcmp(tempvowels[ntempvowels-1],"ue")) == 0 || \
			(strcmp(tempvowels[ntempvowels-1],"en")) == 0) && \
			!(ntempcodas[ntempvowels-1] == 1 && tempcodas[ntempvowels-1][0][0] == 'z') ) {
	
			strcpy(tempcodas[ntempvowels-2][ntempcodas[ntempvowels-2]], tempvowels[ntempvowels-1]);
			tempcodasnum[ntempvowels-2][ntempcodas[ntempvowels-2]] = tempvowelsnum[ntempvowels-1];
			ntempcodas[ntempvowels-2]++;	

			for(i=0;i<ntempcodas[ntempvowels-1];i++) {
				strcpy(tempcodas[ntempvowels-2][ntempcodas[ntempvowels-2]],tempcodas[ntempvowels-1][i]);
				tempcodasnum[ntempvowels-2][ntempcodas[ntempvowels-2]] = tempcodasnum[ntempvowels-1][i];
				ntempcodas[ntempvowels-2]++;
			}
			ntempcodas[ntempvowels-1] = 0;
			ntempvowels--;

			fp1 = fopen("e2.end.txt","a");
			fprintf(fp1,"%d %d %d %s %s %d %d %s %s\n",h,ntempvowels,nphonvowels,tempvowels[h],GlobalWord,ntempcodas[0],ntemponsets[1], tempcodas[0],temponsets[1]);
			fclose(fp1);
		}
	}
	//PrintWord();
	h=0;
	if( (phononsets[h][0] == 's' && phononsets[h][1] == 'k') && nphononsets[0] > 1) {		
		if( ntemponsets[h] > 0 && (strcmp(temponsets[h][0],"sc")) == 0) {
			for(i=ntemponsets[0];i>0;i--) {
				strcpy(temponsets[h][i],temponsets[h][i-1]);
				temponsetsnum[h][i] = temponsetsnum[h][i-1];
			}
			strcpy(temponsets[h][0],"s");
			strcpy(temponsets[h][1],"c");
			temponsetsnum[h][0] = NGRAPHEMES + FindLetter('s');
			temponsetsnum[h][1] = NGRAPHEMES + FindLetter('c');
			ntemponsets[0]++;
			fp1 = fopen("sc.split.2.txt","a");
			fprintf(fp1,"%d %s %s %d %d %s %s\n",h,tempvowels[h],GlobalWord,ntempcodas[0],ntemponsets[1], tempcodas[0],temponsets[1]);
			fclose(fp1);
		}		
	}

	//FIX sc.
	for(h=0;h<ntempvowels;h++) {
		//if(h != 0) {
			if(( (phononsets[h+1][0] == 's' && phononsets[h+1][1] == 'k' && (h != ntempvowels-1) && (nphononsets[h+1] > 1)) || \
				(phoncodas[h][0] == 's' && phononsets[h+1][0] == 'k' && (h != ntempvowels-1) && nphoncodas[h] > 0 && nphononsets[h+1] > 0)  )) {				
				
				if( ((ntempcodas[h] > 0 && strcmp(tempcodas[h][0],"sc")) == 0) || \
					(ntempcodas[h] > 1 && (strcmp(tempcodas[h][1],"sc")) == 0) ) {
			
					if( (strcmp(tempcodas[h][1],"sc")) != 0) {
						for(i=ntempcodas[h];i>0;i--) {
							strcpy(tempcodas[h][i],tempcodas[h][i-1]);
							tempcodasnum[h][i] = tempcodasnum[h][i-1];
						}
						strcpy(tempcodas[h][0],"s");
						strcpy(tempcodas[h][1],"c");
						tempcodasnum[h][0] = NGRAPHEMES + FindLetter('s');
						tempcodasnum[h][1] = NGRAPHEMES + FindLetter('c');
					} else {
						for(i=ntempcodas[h];i>1;i--) {
							strcpy(tempcodas[h][i],tempcodas[h][i-1]);
							tempcodasnum[h][i] = tempcodasnum[h][i-1];
						}
						strcpy(tempcodas[h][1],"s");
						strcpy(tempcodas[h][2],"c");
						tempcodasnum[h][1] = NGRAPHEMES + FindLetter('s');
						tempcodasnum[h][2] = NGRAPHEMES + FindLetter('c');
					}	
					ntempcodas[h]++;
					fp1 = fopen("sc.split.txt","a");
					fprintf(fp1,"%d %s %s %d %d %s %s\n",h,tempvowels[h],GlobalWord,ntempcodas[0],ntemponsets[1], tempcodas[0],temponsets[1]);
					fclose(fp1);
				}					
			}
	//	} else {
			
		//}
	}


	//fix -eo grapheme
	for(h=1;h<ntempvowels;h++) {
		if(strcmp(tempvowels[h],"eo") == 0) {
			strcpy(tempcodas[h-1][ntempcodas[h-1]],"e");
			tempcodasnum[h-1][ntempcodas[h-1]] = NGRAPHEMES + FindLetter('e');
			ntempcodas[h-1]++;
			if(strcmp(tempcodas[h][0],"n") == 0) {
				strcpy(tempvowels[h],"on");
				tempvowelsnum[h] = findGrapheme(tempvowels[h]);
				for(i=0;i<ntempcodas[h];i++) {
					strcpy(tempcodas[h][i], tempcodas[h][i+1]);
					tempcodasnum[h][i] = tempcodasnum[h][i+1];
				}
				ntempcodas[h]--;
			} else {
				strcpy(tempvowels[h],"o");
				tempvowelsnum[h] = NGRAPHEMES + FindLetter('o');
			}
		}
	}

	for(h=1;h<ntempvowels;h++) {
		if(strcmp(tempvowels[h],"eon") == 0) {
			strcpy(tempcodas[h-1][ntempcodas[h-1]],"e");
			tempcodasnum[h-1][ntempcodas[h-1]] = NGRAPHEMES + FindLetter('e');
			ntempcodas[h-1]++;
			strcpy(tempvowels[h],"on");
			tempvowelsnum[h] = findGrapheme(tempvowels[h]);
		}
	}

	//repeate to catch a few missing
	if( (ntempvowels > nphonvowels && ntempvowels >= 2)) {
		if( ((strcmp(tempvowels[ntempvowels-1],"e")) == 0 || \
			(strcmp(tempvowels[ntempvowels-1],"ue")) == 0 || \
			(strcmp(tempvowels[ntempvowels-1],"en")) == 0) && \
			!(ntempcodas[ntempvowels-1] == 1 && tempcodas[ntempvowels-1][0][0] == 'z') ) {

		
				strcpy(tempcodas[ntempvowels-2][ntempcodas[ntempvowels-2]], tempvowels[ntempvowels-1]);
				tempcodasnum[ntempvowels-2][ntempcodas[ntempvowels-2]] = tempvowelsnum[ntempvowels-1];
				ntempcodas[ntempvowels-2]++;	

				for(i=0;i<ntempcodas[ntempvowels-1];i++) {
					strcpy(tempcodas[ntempvowels-2][ntempcodas[ntempvowels-2]],tempcodas[ntempvowels-1][i]);
					tempcodasnum[ntempvowels-2][ntempcodas[ntempvowels-2]] = tempcodasnum[ntempvowels-1][i];
					ntempcodas[ntempvowels-2]++;
				}
				ntempcodas[ntempvowels-1] = 0;
				ntempvowels--;

				fp1 = fopen("e2.2nd.end.txt","a");
				fprintf(fp1,"%d %d %d %s %s %d %d %s %s\n",h,ntempvowels,nphonvowels,tempvowels[h],GlobalWord,ntempcodas[0],ntemponsets[1], tempcodas[0],temponsets[1]);
				fclose(fp1);
		
		}
	}

	for(h=0;h<ntempvowels;h++) {
		splitspot = -1;
		if((strcmp(tempvowels[h],"oue")) == 0 && phonvowels[h] == 'u') splitspot = 2;
		else if( ((strcmp(tempvowels[h],"ouai")) == 0)/*&& phonvowels[h] == 'u'*/)splitspot = 2;
		else if( ((strcmp(tempvowels[h],"ouai")) == 0) /*&& phonvowels[h] == 'u'*/)splitspot = 2;
		else if( ((strcmp(tempvowels[h],"ie")) == 0) && ((strcmp(tempcodas[h][0],"n")) == 0) && h == ntempvowels-1 && ntempcodas[h] == 2) splitspot = 1; //cient
		else if( ((strcmp(tempvowels[h],"ie")) == 0) && ((strcmp(tempcodas[h][0],"s")) == 0) &&  h == ntempvowels-1 && ntempcodas[h] == 1) splitspot = 1; //ies
		else if( ((strcmp(tempvowels[h],"ie")) == 0) && ((strcmp(tempvowels[h+1],"u")) == 0) && tempcodas[h] == 0) splitspot = 1;	
		else if( ((strcmp(tempvowels[h],"ue")) == 0) && phonvowels[h] == 'y' && phonvowels[h+1] == 'E' && (nphoncodas[h]+nphononsets[h+1]) == 0) splitspot = 1;	
		else if( ((strcmp(tempvowels[h],"ueu")) == 0) && phonvowels[h] == 'y' && phonvowels[h+1] == '2' && (nphoncodas[h]+nphononsets[h+1]) == 0) splitspot = 1;	
		else if((strcmp(tempvowels[h],"io")) == 0 && phononsets[h+1][0] == 'j' && phonvowels[h+1] == 'O')splitspot = 1;
		
		if(ntempvowels<nphonvowels || splitspot != -1) {
			if((strcmp(tempvowels[h],"eui")) == 0 && phonvowels[h] == '2') splitspot = 2;
			else if((strcmp(tempvowels[h],"ué")) == 0 && phonvowels[h] == 'y' && phonvowels[h+1] == 'e') splitspot = 1;
			else if((strcmp(tempvowels[h],"ue")) == 0 && phonvowels[h] == 'y') splitspot = 1;
			else if((strcmp(tempvowels[h],"oueu")) == 0 /*&& phonvowels[h] == 'u'*/)splitspot = 2;
			else if((strcmp(tempvowels[h],"ouon")) == 0 /*&& phonvowels[h] == 'u'*/)splitspot = 2;
	//		else if((strcmp(tempvowels[h],"ouhai")) == 0 /*&& phonvowels[h] == 'u'*/)splitspot = 2;	
			else if((strcmp(tempvowels[h],"oue")) == 0 && phonvowels[h] == 'u')splitspot = 2;
			else if((strcmp(tempvowels[h],"oué")) == 0 && phonvowels[h] == 'u')splitspot = 2;
			else if((strcmp(tempvowels[h],"ou")) == 0 && phonvowels[h] == 'é')splitspot = 2;
			else if((strcmp(tempvowels[h],"oui")) == 0 && phonvowels[h] == 'u')splitspot = 2;
			else if((strcmp(tempvowels[h],"oie")) == 0 && phonvowels[h] == 'w')splitspot = 1;
			else if((strcmp(tempvowels[h],"aï")) == 0 && phonvowels[h] == 'a')splitspot = 1;
			else if((strcmp(tempvowels[h],"oua")) == 0 && phonvowels[h] == 'u')splitspot = 2;//clouant
			else if((strcmp(tempvowels[h],"oueu")) == 0 && phonvowels[h] == 'u')splitspot = 2;
			else if((strcmp(tempvowels[h],"aou")) == 0 && phonvowels[h] == 'u')splitspot = 1;
			else if((strcmp(tempvowels[h],"aou")) == 0 && phonvowels[h] == 'a')splitspot = 1;
			else if((strcmp(tempvowels[h],"ée")) == 0 && phonvowels[h] == 'a')splitspot = 1;
			else if((strcmp(tempvowels[h],"ée")) == 0 && phonvowels[h] == 'e')splitspot = 1;
			else if((strcmp(tempvowels[h],"ouhai")) == 0 && phonvowels[h] == 'e')splitspot = 2;
			else if((strcmp(tempvowels[h],"iei")) == 0 && phonvowels[h] == 'e')splitspot = 1;
			else if((strcmp(tempvowels[h],"oe")) == 0 && phonvowels[h] == 'e')splitspot = 1;
			else if((strcmp(tempvowels[h],"ué")) == 0 && phononsets[h][nphononsets[h]-1] == '8')splitspot = 1;
			else if((strcmp(tempvowels[h],"iau")) == 0 && phononsets[h+1][0] == 'j')splitspot = 1;	
			else if((strcmp(tempvowels[h],"iaie")) == 0 && phononsets[h+1][0] == 'j')splitspot = 1;	
			else if((strcmp(tempvowels[h],"ieu")) == 0 && phononsets[h+1][0] == 'j')splitspot = 1;
			else if((strcmp(tempvowels[h],"ie")) == 0 && phononsets[h+1][0] == 'j' && phonvowels[h+1] == '@')splitspot = 1;		
			else if((strcmp(tempvowels[h],"ion")) == 0 && phononsets[h+1][0] == 'j')splitspot = 1;
			else if((strcmp(tempvowels[h],"io")) == 0 && phononsets[h+1][0] == 'j')splitspot = 1;
			else if((strcmp(tempvowels[h],"ie")) == 0 && phononsets[h+1][0] == 'j')splitspot = 1;
			else if((strcmp(tempvowels[h],"aï")) == 0 && (phononsets[h+1][0] == 'j' || phoncodas[h][0] == 'j') && (nphoncodas[h] == 1 || nphononsets[h+1] == 1))splitspot = 1;
			else if((strcmp(tempvowels[h],"ieu")) == 0 && phononsets[h][nphononsets[h]-1] == 'j')splitspot = 1;
			else if((strcmp(tempvowels[h],"oua")) == 0 && nphoncodas[h] == 0 && phonvowels[h] == 'u')splitspot = 2;
			else if((strcmp(tempvowels[h],"ouè")) == 0 && nphoncodas[h] == 0 && phonvowels[h] == 'u')splitspot = 2;
			else if((strcmp(tempvowels[h],"eui")) == 0 && nphoncodas[h] == 0 && phonvowels[h] == 'u')splitspot = 2;
		

			if(splitspot != -1) {
				strcpy(thebit2, tempvowels[h]+splitspot);
				strcpy(thebit1, tempvowels[h]);
				thebit1[splitspot] = 0;
				
				fp = fopen("SplitVowels.two.txt","a");	
				fprintf(fp, " %s %s %s %s %s %d\n",GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
				fclose(fp);
								
				if(strcmp(tempvowels[h],"ue") == 0 && strcmp(tempcodas[h+1][0],"n") == 0) {		
					for(i=ntempvowels;i>h;i--) {
						strcpy(tempvowels[i],tempvowels[i-1]);
						tempvowelsnum[i] = tempvowelsnum[i-1];
						for(j=0;j<ntempcodas[i-1];j++) {
							strcpy(tempcodas[i][j],tempcodas[i-1][j]);
							tempcodasnum[i][j] = tempcodasnum[i-1][j];
						}
						ntempcodas[i] = ntempcodas[i-1];
					}		
					ntempcodas[h] = 0;
					ntempvowels++;
					//fluence f.l.u.0.n.ce
					strcpy(tempvowels[h],"u");
					strcpy(tempvowels[h+1],"en");
					tempvowelsnum[h] = NGRAPHEMES + FindLetter('u');
					tempvowelsnum[h+1] = findGrapheme("en");
					for(i=0;i<ntempcodas[h+1]-1;i++) {
						strcpy(tempcodas[h+1][i],tempcodas[h+1][i+1]);
						tempcodasnum[h+1][i] = tempcodasnum[h+1][i+1];
					}
					ntempcodas[h+1]--;
				} /*else if((strcmp(tempvowels[h],"aï"))==0) {
					strcpy(tempcodas[h][0],"ï");
					tempcodasnum[h][0] = NGRAPHEMES + FindLetter('ï');

					fp = fopen("SplitVowels.two.ai.txt","a");	
					fprintf(fp, " %s %s %s %s %s %d\n",GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
					fclose(fp);
				} */ else if ( (tempvowels[h][strlen(tempvowels[h])-1] == 'i' || tempvowels[h][strlen(tempvowels[h])-1] == 'e') && \
							((strcmp(tempcodas[h][0],"n")) == 0)) {
					
					bit1[0] = tempvowels[h][strlen(tempvowels[h])-1];
					bit1[1] = 'n';
					bit1[2] = 0; 

						//client.elle
					for(i=ntempvowels;i>h+1;i--) {
						strcpy(tempvowels[i],tempvowels[i-1]);
						tempvowelsnum[i] = tempvowelsnum[i-1];
						for(j=0;j<ntempcodas[i-1];j++) {
							strcpy(tempcodas[i][j],tempcodas[i-1][j]);
							tempcodasnum[i][j] = tempcodasnum[i-1][j];
						}
						ntempcodas[i] = ntempcodas[i-1];
					}		

					for(j=1;j<ntempcodas[h];j++) {
						strcpy(tempcodas[h+1][j-1],tempcodas[h][j]);
						tempcodasnum[h+1][j-1] = tempcodasnum[h][j];
					}
					ntempcodas[h+1] = ntempcodas[h]-1;
					ntempcodas[h] = 0;  //cl.i.[en].t [2]
					strcpy(tempvowels[h+1],bit1);
					tempvowelsnum[h+1] = findGrapheme(bit1);
					//cli[h].ent[h+1],elle[h+2]
					ntempvowels++;

						strcpy(tempvowels[h],thebit1);
						if(strlen(thebit1) > 1)	tempvowelsnum[h] = findGrapheme(thebit1);
						else tempvowelsnum[h] = NGRAPHEMES + FindLetter(thebit1[0]);

					fp = fopen("SplitVowels.two.2.txt","a");	
					fprintf(fp, " %s %s %s %s %s %d\n",GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
					fclose(fp);
				} else if ((strcmp(tempvowels[h],"ie")) == 0 && (strcmp(tempcodas[h][0],"s")) == 0) {
						strcpy(tempvowels[h],"i");
						strcpy(tempcodas[h][0],"e");
						strcpy(tempcodas[h][1],"s");
						ntempcodas[h] = 2;
						tempvowelsnum[h] = NGRAPHEMES + FindLetter('i');
						tempcodasnum[h][0] = NGRAPHEMES + FindLetter('e');
						tempcodasnum[h][1] = NGRAPHEMES + FindLetter('s');
					
				} /*else if((strcmp(tempvowels[h],"ie")) == 0 && ((strcmp(tempvowels[h+1],"u") == 0)) && ntempcodas[h] == 0) {
					strcpy(tempcodas[h-1][ntempcodas[h]-1],"i");
					strcpy(tempvowels[h],"eu");
					tempvowelsnum[h] = NGRAPHEMES + FindLetter('i');
					tempcodasnum[h-1][ntempcodas[h]-1] = NGRAPHEMES + FindLetter('i');
					tempvowelsnum[h] = findGrapheme("eu");
					ntempcodas[h-1]++;			
				}*/ else {  /* clouai */
					for(i=ntempvowels;i>h;i--) {
						strcpy(tempvowels[i],tempvowels[i-1]);
						tempvowelsnum[i] = tempvowelsnum[i-1];
						for(j=0;j<ntempcodas[i-1];j++) {
							strcpy(tempcodas[i][j],tempcodas[i-1][j]);
							tempcodasnum[i][j] = tempcodasnum[i-1][j];
						}
						ntempcodas[i] = ntempcodas[i-1];
					}	
					ntempcodas[h] = 0;

			
					strcpy(tempvowels[h+1],thebit2);
					if(strlen(thebit2) > 1)	tempvowelsnum[h+1] = findGrapheme(thebit2);
					else tempvowelsnum[h+1] = NGRAPHEMES + FindLetter(thebit2[0]);

					strcpy(tempvowels[h],thebit1);
					if(strlen(thebit1) > 1)	tempvowelsnum[h] = findGrapheme(thebit1);
					else tempvowelsnum[h] = NGRAPHEMES + FindLetter(thebit1[0]);

					ntempvowels++;

				}
			
				fp = fopen("SplitVowels.two.txt","a");	
				fprintf(fp, " %s %s %s %s %s %d\n",GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
				fclose(fp);
			}
		}
	}


	for(h=0;h<ntempvowels/*-1*/;h++) {	
		if(ntempvowels > nphonvowels) {
			if(h < ntempvowels -1) {
				if((((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"on")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"e")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"ia")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"a")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"en")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"an")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"o")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"e")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"â")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"é")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"è")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"ou")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"eu")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"ai")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"om")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"on")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"er")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"am")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"u")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"i")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"ée")) == 0) ||\
					((strcmp(tempvowels[h],"ou")) == 0 && (strcmp(tempvowels[h+1],"an")) == 0) ||\
					((strcmp(tempvowels[h],"ou")) == 0 && (strcmp(tempvowels[h+1],"a")) == 0) ||\
					((strcmp(tempvowels[h],"ou")) == 0 && (strcmp(tempvowels[h+1],"â")) == 0) ||\
					((strcmp(tempvowels[h],"ou")) == 0 && (strcmp(tempvowels[h+1],"é")) == 0) ||\
					((strcmp(tempvowels[h],"ou")) == 0 && (strcmp(tempvowels[h+1],"e")) == 0) ||\
					((strcmp(tempvowels[h],"ou")) == 0 && (strcmp(tempvowels[h+1],"ï")) == 0) ||\
					((strcmp(tempvowels[h],"ou")) == 0 && (strcmp(tempvowels[h+1],"î")) == 0) ||\
					((strcmp(tempvowels[h],"ou")) == 0 && (strcmp(tempvowels[h+1],"i")) == 0) ||\
					((strcmp(tempvowels[h],"ou")) == 0 && (strcmp(tempvowels[h+1],"eu")) == 0) ||\
					((strcmp(tempvowels[h],"ou")) == 0 && (strcmp(tempvowels[h+1],"è")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"e")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"i")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"a")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"an")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"on")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"ai")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"é")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"è")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"â")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"ai")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"an")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"uo")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"o")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"y")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"aie")) == 0) ||\
					((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"î")) == 0) ||\
					((strcmp(tempvowels[h],"ui")) == 0 && (strcmp(tempvowels[h+1],"e")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"ou")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"on")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"an")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"en")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"ai")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"e")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"eu")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"è")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"é")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"u")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"a")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"O")) == 0) ||\
					((strcmp(tempvowels[h],"y")) == 0 && (strcmp(tempvowels[h+1],"o")) == 0) ||\
					((strcmp(tempvowels[h],"e")) == 0 && (strcmp(tempvowels[h+1],"o")) == 0) ||\
					((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"in")) == 0) ||\
					((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"in")) == 0) ||\
					((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"î")) == 0) ||\
					((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"ie")) == 0) ||\
					((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"e")) == 0) ||\
					((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"ê")) == 0) ||\
					((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"è")) == 0) ||\
					((strcmp(tempvowels[h],"e")) == 0 && (strcmp(tempvowels[h+1],"y")) == 0) ||\
					((strcmp(tempvowels[h],"a")) == 0 && (strcmp(tempvowels[h+1],"y")) == 0) ||\
					((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"y")) == 0) ||\
					((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"y")) == 0) ||\
					((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"ï")) == 0) ||\
					((strcmp(tempvowels[h],"ï")) == 0 && (strcmp(tempvowels[h+1],"a")) == 0) ||\
					((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"i")) == 0) ) && ntempcodas[h] == 0) {
			

					if (((strcmp(tempvowels[h],"u")) == 0 && (strcmp(tempvowels[h+1],"y")) == 0 && ntempcodas[h] == 0&& phononsets[h+1][0] == 'j') ||\
						((strcmp(tempvowels[h],"e")) == 0 && (strcmp(tempvowels[h+1],"y")) == 0 && ntempcodas[h] == 0&& phononsets[h+1][0] == 'j') ||\
						((strcmp(tempvowels[h],"i")) == 0 && (strcmp(tempvowels[h+1],"y")) == 0 && ntempcodas[h] == 0&& phononsets[h+1][0] == 'j') ||\
						((strcmp(tempvowels[h],"ï")) == 0 && (strcmp(tempvowels[h+1],"a")) == 0 && ntempcodas[h] == 0 && phononsets[h+1][0] == 'j') ||\
						((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"y")) == 0 && ntempcodas[h] == 0 && phononsets[h+1][0] == 'j') ||\
						((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"ï")) == 0 && (strcmp(tempvowels[h+2],"a")) == 0 && ntempcodas[h] == 0) ||\
						((strcmp(tempvowels[h],"o")) == 0 && (strcmp(tempvowels[h+1],"ï")) == 0 && (strcmp(tempvowels[h+2],"a")) == 0 && ntempcodas[h] == 0) ||\
						((strcmp(tempvowels[h],"a")) == 0 && (strcmp(tempvowels[h+1],"y")) == 0 && ntempcodas[h] == 0 && phononsets[h+1][0] == 'j') 		) {
							fp1 = fopen("uy.txt","a");
							fprintf(fp1,"%s %s %d %d %s %s %d\n",GlobalWord,PhonString,ntempcodas[0],ntemponsets[1], tempcodas[0],temponsets[1],FindLetter('i'));
							fclose(fp1);

							strcpy(tempcodas[h][0],tempvowels[h+1]);
							tempcodasnum[h][0] = NGRAPHEMES + FindLetter(tempvowels[h+1][0]);
							ntempcodas[h] = 1;
							for(i=h+1;i<ntempvowels;i++) {
								strcpy(tempvowels[i],tempvowels[i+1]);
								tempvowelsnum[i] = tempvowelsnum[i+1];
								for(j=0;j<ntempcodas[i+1];j++) {
									strcpy(tempcodas[i][j],tempcodas[i+1][j]);
									tempcodasnum[i][j] = tempcodasnum[i+1][j];
								}
								ntempcodas[i] = ntempcodas[i+1];
							}
							ntempcodas[ntempvowels-1] = 0;
							ntempvowels--;
					} else {

						if(phononsets[h][nphononsets[h]-1] == 'j' ||\
							phononsets[h][nphononsets[h]-1] == 'w'||\
							(phononsets[h][0] == 'g' && (strcmp(tempcodas[h-1][ntempcodas[h-1]-1],"g")) == 0) ||\
							phononsets[h][nphononsets[h]-1] == '8') {
							fp1 = fopen("wotdisio.txt","a");
							fprintf(fp1,"%s %s %d %d %s %s %d\n",GlobalWord,PhonString,ntempcodas[0],ntemponsets[1], tempcodas[0],temponsets[1],FindLetter('i'));
							fclose(fp1);

							if(h!=0) {
								strcpy(tempcodas[h-1][ntempcodas[h-1]], tempvowels[h]);
								tempcodasnum[h-1][ntempcodas[h-1]] = tempvowelsnum[h];
								ntempcodas[h-1]++;
							} else {		
								strcpy(temponsets[0][ntemponsets[0]], tempvowels[h]);
								temponsetsnum[0][ntemponsets[0]] = tempvowelsnum[h];
								ntemponsets[0]++;
							}

							for(i=h;i<ntempvowels-1;i++) {
								strcpy(tempvowels[i],tempvowels[i+1]);
								tempvowelsnum[i] = tempvowelsnum[i+1];
								for(j=0;j<ntempcodas[i+1];j++) {
									strcpy(tempcodas[i][j],tempcodas[i+1][j]);
									tempcodasnum[i][j] = tempcodasnum[i+1][j];
								}
								ntempcodas[i] = ntempcodas[i+1];
							}
							ntempcodas[ntempvowels-1] = 0;
							ntempvowels--;
						}
					}
				}
			}
		}
	}

	for(h=0;h<ntempvowels/*-1*/;h++) {	
		splitspot = -1;
		if(h == 100) {
			if (ntemponsets[0] < nphononsets[h]) {
				
			}
		} else {
			/*fp = fopen("SplitVowels.cods.txt","a");	
			fprintf(fp, "%d %d %d %d %s %s %s %s %s %d\n",h,ntempcodas[h-1],nphoncodas[h-1],nphononsets[h],GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
			fclose(fp);*/

			//if(ntempcodas[h-1] < (nphoncodas[h-1]+nphononsets[h])) {
				if((strcmp(tempvowels[h],"oui")) == 0 && phononsets[h][nphononsets[h]-1] == '8') splitspot = 1;	
				else if((strcmp(tempvowels[h],"oui")) == 0 && ntempcodas[h] == 0 && phonvowels[h+1] == 'i')splitspot = 2;
				else if((strcmp(tempvowels[h],"oua")) == 0 && ntempcodas[h] == 0 && phonvowels[h+1] == 'w')splitspot = 2;
				else if((strcmp(tempvowels[h],"ouai")) == 0 && ntempcodas[h] == 0 && phonvowels[h+1] == 'w')splitspot = 2;				
				else if((strcmp(tempvowels[h],"oué")) == 0 && ntempcodas[h] == 0 && phonvowels[h+1] == 'w')splitspot = 2;
				else if((strcmp(tempvowels[h],"ouè")) == 0 && ntempcodas[h] == 0 && phonvowels[h+1] == 'w')splitspot = 2;
				else if((strcmp(tempvowels[h],"ouâ")) == 0 && ntempcodas[h] == 0 && phonvowels[h+1] == 'w')splitspot = 2;		
				else if((strcmp(tempvowels[h],"ouon")) == 0 && ntempcodas[h] == 0 && phonvowels[h+1] == 'w')splitspot = 2;		
				else if((strcmp(tempvowels[h],"oueu")) == 0 && ntempcodas[h] == 0 && phonvowels[h+1] == 'w')splitspot = 2;		
				else if((strcmp(tempvowels[h],"oie")) == 0 && ntempcodas[h] == 0 && phonvowels[h+1] == 'w')splitspot = 1;					
				else if((strcmp(tempvowels[h],"oi")) == 0 && phononsets[h][nphononsets[h]-1] == 'w')splitspot = 1;				
				else if((strcmp(tempvowels[h],"ouâ")) == 0 && phononsets[h][nphononsets[h]-1] == 'w')splitspot = 2;				
				else if((strcmp(tempvowels[h],"oueu")) == 0 && phononsets[h][nphononsets[h]-1] == 'w')splitspot = 2;				
				else if((strcmp(tempvowels[h],"ouon")) == 0 && phononsets[h][nphononsets[h]-1] == 'w')splitspot = 2;			
				else if((strcmp(tempvowels[h],"ouhai")) == 0 && phononsets[h][nphononsets[h]-1] == 'w')splitspot = 2;			
				else if((strcmp(tempvowels[h],"ouai")) == 0 && phononsets[h][nphononsets[h]-1] == 'w')splitspot = 2;			
				else if((strcmp(tempvowels[h],"iau")) == 0 && phononsets[h][nphononsets[h]-1] == 'j')splitspot = 1;
				else if((strcmp(tempvowels[h],"io")) == 0 && phononsets[h][nphononsets[h]-1] == 'j')splitspot = 1;
				else if((strcmp(tempvowels[h],"ieu")) == 0 && phononsets[h][nphononsets[h]-1] == 'j')splitspot = 1;
				else if((strcmp(tempvowels[h],"oie")) == 0 && phononsets[h][nphononsets[h]-1] == 'j')splitspot = 1;
				else if((strcmp(tempvowels[h],"oie")) == 0 && phononsets[h][nphononsets[h]-1] == 'w')splitspot = 1;
				else if((strcmp(tempvowels[h],"eoi")) == 0 && phononsets[h][nphononsets[h]-1] == 'w')splitspot = 1;
				else if((strcmp(tempvowels[h],"ien")) == 0 && phononsets[h][nphononsets[h]-1] == 'j') splitspot = 1;	
				else if((strcmp(tempvowels[h],"ion")) == 0 && phononsets[h][nphononsets[h]-1] == 'j') splitspot = 1;		
				else if((strcmp(tempvowels[h],"uin")) == 0 && phononsets[h][nphononsets[h]-1] == 'j') splitspot = 1;		
				else if((strcmp(tempvowels[h],"ue")) == 0 && phononsets[h][nphononsets[h]-1] == '8')splitspot = 1;
				else if((strcmp(tempvowels[h],"ué")) == 0 && phononsets[h][nphononsets[h]-1] == '8')splitspot = 1;
				else if((strcmp(tempvowels[h],"ie")) == 0 && phononsets[h][nphononsets[h]-1] == 'j')splitspot = 1;	
				else if((strcmp(tempvowels[h],"oue")) == 0 && phononsets[h][nphononsets[h]-1] == 'w')splitspot = 2;
				else if((strcmp(tempvowels[h],"oué")) == 0 && phononsets[h][nphononsets[h]-1 ]== 'w')splitspot = 2;
				else if((strcmp(tempvowels[h],"ouè")) == 0 && phononsets[h][nphononsets[h]-1 ]== 'w')splitspot = 2;
				else if((strcmp(tempvowels[h],"oua")) == 0 && phononsets[h][nphononsets[h]-1] == 'w')splitspot = 2;
				else if((strcmp(tempvowels[h],"oui")) == 0 && phononsets[h][nphononsets[h]-1] == 'w')splitspot = 2;
				else if((strcmp(tempvowels[h],"iaie")) == 0 && phononsets[h][nphononsets[h]-1] == 'j') splitspot = 1;
				else if((strcmp(tempvowels[h],"ui")) == 0 && phononsets[h][nphononsets[h]-1] == '8')splitspot = 1;
				else if((strcmp(tempvowels[h],"ou")) == 0 && phononsets[h][nphononsets[h]-1] == '8')splitspot = 1;
				else if((strcmp(tempvowels[h],"ueu")) == 0 && phononsets[h][nphononsets[h]-1] == '8')splitspot = 1;
				else if((strcmp(tempvowels[h],"uin")) == 0 && phononsets[h][nphononsets[h]-1] == '8')splitspot = 2;		
				else if((strcmp(tempvowels[h],"aï")) == 0 && phononsets[h][nphononsets[h]-1] == 'j')splitspot = 1;
				else if((strcmp(tempvowels[h],"iei")) == 0 && phononsets[h][nphononsets[h]-1] == 'j')splitspot = 1;
		
		//	}


			if(splitspot != -1 ) {
				i = strlen(tempvowels[h]);
				strcpy(thebit2, tempvowels[h]+splitspot);
				thebit2[strlen(tempvowels[h]) - splitspot] = 0;
				strcpy(thebit1,tempvowels[h]);
				thebit1[splitspot] = 0;

					fp = fopen("SplitVowels.en.txt","a");	
					fprintf(fp, " %s %s %s %s %s %d\n",GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
					fclose(fp);


				if(/*h!=0*/1==1) {
					if(h!=0) {
						strcpy(tempcodas[h-1][ntempcodas[h-1]],thebit1);
						if(strlen(thebit1) > 1)	tempcodasnum[h-1][ntempcodas[h-1]] = findGrapheme(thebit1);
						else tempcodasnum[h-1][ntempcodas[h-1]] = NGRAPHEMES + FindLetter(thebit1[0]);
						ntempcodas[h-1]++;
					} else {
						if((strcmp(tempvowels[h],"ouhai")) == 0) {	
							strcpy(tempvowels[h],"ai");
							tempvowelsnum[h] = findGrapheme("ai");
							strcpy(temponsets[h][1],"ou");
							temponsetsnum[h][1] = findGrapheme("ou");
							strcpy(temponsets[h][2],"h");
							temponsetsnum[h][2] = NGRAPHEMES + FindLetter('h');	
							ntemponsets[h] = 3;
							ntempvowels = 100;
						} else {
							strcpy(temponsets[0][ntemponsets[0]],thebit1);
							if(strlen(thebit1) > 1)	temponsetsnum[0][ntemponsets[0]] = findGrapheme(thebit1);
							else temponsetsnum[0][ntemponsets[0]] = NGRAPHEMES + FindLetter(thebit1[0]);
							ntemponsets[0]++;
						}
					}
					if((strcmp(tempvowels[h],"iaie")) == 0) {
						strcpy(tempvowels[h],"ai");
						tempvowelsnum[h] = findGrapheme("ai");
						strcpy(tempcodas[h][0],"en");
						tempcodasnum[h][0] = findGrapheme("en");
					} else if ((strcmp(tempvowels[h],"ouhai")) == 0) {
								//do nothing fixed above.
					} else if ( ( (thebit2[strlen(thebit2)-1] == 'i') || (thebit2[strlen(thebit2)-1] == 'e')|| (thebit2[strlen(thebit2)-1] == 'o')) && \
							((strcmp(tempcodas[h][0],"n")) == 0) ) {
								
							fp = fopen("SplitVowels.in.txt","a");	
							fprintf(fp, " %s %s %s %s %s %d\n",GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
							fclose(fp);
							//croient, zoint
							
							if(strlen(thebit2)>1) {
								strcpy(tempvowels[h],thebit2);
								thebit1[0] = tempvowels[h][strlen(tempvowels[h])-1];
								thebit1[1] = 'n';
								thebit1[2] = 0; //en

								tempvowels[h][strlen(tempvowels[h])-1] = 0;
								if(strlen(tempvowels[h]) == 1) tempvowelsnum[h] = NGRAPHEMES + FindLetter(tempvowels[h][0]);
								else findGrapheme(tempvowels[h]);

								strcpy(tempcodas[h][0],thebit1);
								tempcodasnum[h][0] = findGrapheme(thebit1);
							} else {
								thebit2[1] = 'n';
								thebit2[2] = 0;
								strcpy(tempvowels[h],thebit2);
								if(strlen(tempvowels[h]) == 1) tempvowelsnum[h] = NGRAPHEMES + FindLetter(tempvowels[h][0]);
								else tempvowelsnum[h] = findGrapheme(tempvowels[h]);
								for(i=0;i<ntempcodas[h]-1;i++) {
									strcpy(tempcodas[h][i],tempcodas[h][i+1]);
									tempcodasnum[h][i] = tempcodasnum[h][i+1];
								}
								ntempcodas[h]--;
							}			
					} else {
						strcpy(tempvowels[h],thebit2);
						if(strlen(thebit2) > 1) tempvowelsnum[h] = findGrapheme(thebit2);
						else tempvowelsnum[h] = NGRAPHEMES + FindLetter(thebit2[0]);
					}
					fp = fopen("SplitVowels.txt","a");	
					fprintf(fp, "%c %s %s %s %s %s %s %d\n",thebit2[strlen(thebit2)-1], thebit2, GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
					fclose(fp);

				} else {
					strcpy(temponsets[0][ntemponsets[0]],thebit1);
					if(strlen(thebit1) > 1)	temponsetsnum[0][ntemponsets[0]] = findGrapheme(thebit1);
					else temponsetsnum[0][ntemponsets[0]] = NGRAPHEMES + FindLetter(thebit1[0]);
					ntemponsets[0]++;

					strcpy(tempvowels[h],thebit2);
					if(strlen(thebit2) > 1) tempvowelsnum[h] = findGrapheme(thebit2);
					else tempvowelsnum[h] = NGRAPHEMES + FindLetter(thebit2[0]);

					fp = fopen("SplitVowels.first.txt","a");	
					fprintf(fp, " %s %s %s %s %s %d\n",GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
					fclose(fp);
					PrintWord();
				}
			} 

			//aien
			//affluence split vowels!
			if( ((strcmp(tempvowels[h],"ue")) == 0 && phonvowels[h] == 'y' && (strcmp(tempcodas[h][0],"n"))==0 ) ) {		
				if(h!=0) {
					//a.ff.l.ue.nce.ment
					for(i=ntempvowels;i>h+1;i--) {
						strcpy(tempvowels[i],tempvowels[i-1]);
						tempvowelsnum[i] = tempvowelsnum[i-1];
						for(j=0;j<ntempcodas[i-1];j++) {
							strcpy(tempcodas[i][j],tempcodas[i-1][j]);
							tempcodasnum[i][j] = tempcodasnum[i-1][j];
						}
						ntempcodas[i] = ntempcodas[i-1];
					}

					strcpy(tempvowels[h],"u");
					tempvowelsnum[h] = NGRAPHEMES + FindLetter('u');

					strcpy(tempvowels[h+1],"en");
					tempvowelsnum[h+1] = findGrapheme("en");
					for(j=0;j<ntempcodas[h]-1;j++) {
						strcpy(tempcodas[h+1][j],tempcodas[h][j+1]);
						tempcodasnum[h+1][j] = tempcodasnum[h][j+1];
					}
					ntempcodas[h+1] = ntempcodas[h] - 1;
					ntempcodas[h] = 0;
					ntempvowels++;
									
					fp = fopen("SplitVowels.en.txt","a");	
					fprintf(fp, " %s %s %s %s %s %d\n",GlobalWord, tempvowels[h], tempcodas[h],thebit1,thebit2,tempvowelsnum[h]);
					fclose(fp);
				}
			}	
		}
	}
	//remercient, remuent
	if(strcmp(tempvowels[ntempvowels-1],"en") == 0 && strcmp(tempcodas[ntempvowels-1][0],"t") == 0 && ntempcodas[ntempvowels-1] == 1) {
		if(ntemponsets[0] == 3 && strcmp(temponsets[0][1],"e") == 0) {			
			fp = fopen("entfix2.txt","a");	
			fprintf(fp, " %s %d %d\n",GlobalWord,ntempcodas[0],ntempvowels);
			fclose(fp);
			for(i=ntempvowels;i>0;i--) {
				strcpy(tempvowels[i],tempvowels[i-1]);
				tempvowelsnum[i] = tempvowelsnum[i-1];
			}
			strcpy(tempvowels[0],"e");
			tempvowelsnum[0] = NGRAPHEMES + FindLetter('e');			
			ntempcodas[ntempvowels] = 0; ntemponsets[ntempvowels] = 0;
		
			//(ntempvowels == 3) {  //rem..e(v0).rc(c1)..i(v1).en(v2)t(c2).. copies rc back re.merc.i.ent
			//demandent. d.e.m.an.d (c0).en.t
			for(h=ntempvowels-1;h>0;h--) {
				for(i=0;i<ntempcodas[h];i++) {
					strcpy(tempcodas[h][i],tempcodas[h-1][i]);
					tempcodasnum[h][i] = tempcodasnum[h-1][i];
				}
				ntempcodas[h] = ntempcodas[h-1];
			}

			strcpy(tempcodas[0][0],temponsets[0][2]); //the m
			tempcodasnum[0][0] = temponsetsnum[0][2];
			ntempcodas[0] = 1;
			ntemponsets[0] = 1;
			strcpy(tempcodas[ntempvowels-1][ntempcodas[ntempvowels-1]],"en");
			tempcodasnum[ntempvowels-1][ntempcodas[ntempvowels-1]] = findGrapheme("en");
			strcpy(tempcodas[ntempvowels-1][ntempcodas[ntempvowels-1]+1],"t");
			tempcodasnum[ntempvowels-1][ntempcodas[ntempvowels-1]+1] = FindLetter('t');

			ntempcodas[ntempvowels-1]+=2;	

		//	fp = fopen("entfix2.txt","a");	
			//fprintf(fp, " %s %d %d %d\n",GlobalWord, i, ntempcodas[0],ntempcodas[1]);
			//fclose(fp);


			PrintWord();
		}
	}
	

	/// gratuitement
			//gratuitement evenement			
	//PrintWord();
	PrintWord();
#endif
}




GraphemeParsing::GraphemeParsing(void)
{
	int i;
	for(i=0;i<1000;i++) {
		 OnsetCounts[i] = 0;
		 VowelCounts[i] = 0;
		 CodaCounts[i] = 0;
	}
}


GraphemeParsing::~GraphemeParsing(void)
{
}
