#include <CStdFile.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	char *pData; int iSize;
	CStdFile MyFile;
	if (argc < 2)
	{
		fprintf(stderr, "%s infile [outfile]", argv[0]);
		return 1;
	}
	MyFile.Load(argv[1], (uint8_t **)&pData, &iSize, 0, true);
	if (argc < 3)
	{
		fwrite(pData, 1, iSize, stdout);
	}
	else
	{
		MyFile.Create(argv[2], false);
		MyFile.Write(pData, iSize);
	}
	MyFile.Close();
}
