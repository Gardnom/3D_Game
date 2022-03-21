#include "File.h"

#ifdef _WIN32
#include <Windows.h>
#include <commdlg.h>

std::string File::BrowseForFile(std::string startPath) {
	char szFile[100];
	szFile[0] = '\0';

	std::string fileName;
	fileName.reserve(100);

	OPENFILENAME op;
	ZeroMemory(&op, sizeof(op));
	op.lStructSize = sizeof(op);
	op.lpstrTitle = "Open file";
	op.lpstrInitialDir = startPath.c_str();
	op.hwndOwner = NULL;
	op.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	op.lpstrFile = szFile;
	op.nMaxFile = sizeof(szFile);

	BOOL result = GetOpenFileNameA(&op);
	if (result) {
		std::string fileName = szFile;
		return fileName;
	}

	else {
		printf("Failed to open file dialog, error: %ld", CommDlgExtendedError());
		return "";
	}
}
#endif