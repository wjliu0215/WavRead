#pragma warning(disable : 4996)//fopen报错忽略

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "wave.h"

int main()
{
	unsigned int i, j, sz, len;
	FILE* fp = NULL;

	Wav wav;
	RIFF_t riff;
	FMT_t fmt;
	Data_t data;

	fp = fopen("F:\\C代码\\X_room.wav", "rb");
	if (!fp) {
		printf("can't open audio file\n");
		exit(1);
	}

	fread(&wav, 1, sizeof(wav), fp);


	riff = wav.riff;
	fmt = wav.fmt;
	data = wav.data;

	printf("ChunkID \t%c%c%c%c\n", riff.ChunkID[0], riff.ChunkID[1], riff.ChunkID[2], riff.ChunkID[3]);
	printf("ChunkSize \t%d\n", riff.ChunkSize);
	printf("Format \t\t%c%c%c%c\n", riff.Format[0], riff.Format[1], riff.Format[2], riff.Format[3]);

	printf("\n");

	printf("Subchunk1ID \t%c%c%c%c\n", fmt.Subchunk1ID[0], fmt.Subchunk1ID[1], fmt.Subchunk1ID[2], fmt.Subchunk1ID[3]);
	printf("Subchunk1Size \t%d\n", fmt.Subchunk1Size);
	printf("AudioFormat \t%d\n", fmt.AudioFormat);
	printf("NumChannels \t%d\n", fmt.NumChannels);
	printf("SampleRate \t%d\n", fmt.SampleRate);
	printf("ByteRate \t%d\n", fmt.ByteRate);
	printf("BlockAlign \t%d\n", fmt.BlockAlign);
	printf("BitsPerSample \t%d\n", fmt.BitsPerSample);

	printf("\n");

	printf("blockID \t%c%c%c%c\n", data.Subchunk2ID[0], data.Subchunk2ID[1], data.Subchunk2ID[2], data.Subchunk2ID[3]);
	printf("blockSize \t%d\n", data.Subchunk2Size);
	printf("\n");

	//	duration = Subchunk2Size / ByteRate
	printf("duration \t%f\n", (float)data.Subchunk2Size / fmt.ByteRate);
	printf("\n");

	sz = data.Subchunk2Size / 2;
	printf("%d", sz);


	short* pBuff = (short*)calloc(sz, sizeof(short));
	float* wavbuf = (float*)calloc(sz, sizeof(float));
	// rearrange data

	fseek(fp, 44, SEEK_SET);
	fread(pBuff, sizeof(short), sz, fp);



	len = sz / fmt.NumChannels;


		for (i = 0; i < len; i++)
		{
			for (j = 0; j < fmt.NumChannels; j++)
			{
				wavbuf[j + i * fmt.NumChannels] = (float)pBuff[j + i * fmt.NumChannels] / 32767;
				//printf("%f\t%f\n", (float)pBuff[i * fmt.NumChannels] / 32767, (float)pBuff[1 + i * fmt.NumChannels] / 32767);
			}
		}
	free(pBuff);

	
	fprintf(fp,"\t通道1\t\t通道2");
	for(i=0;i<len;i=i+1)
	{
		printf("\n第%d行:\t%f\t\t%f",i+1,wavbuf[i * fmt.NumChannels],wavbuf[i * fmt.NumChannels +1]);
	}
	
	return 0;
}
