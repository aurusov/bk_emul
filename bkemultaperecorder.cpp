#include "stdafx.h"
#include "bkemultaperecorder.h"
#include "bkemul.h"
#include "resource.h"

#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace bkemul;
using namespace std;

// --------------------------------------------------------------
// ---------- BKEmulTapeRecorder
// --------------------------------------------------------------
BKEmulTapeRecorder::BKEmulTapeRecorder():
	findFile( NULL ),
	last_file( false )
{
}

BKEmulTapeRecorder::~BKEmulTapeRecorder()
{
	closeFind();
}

void BKEmulTapeRecorder::reset()
{
}

void BKEmulTapeRecorder::closeFind()
{
	if ( findFile ) {
		findFile->Close();
		delete findFile;
		findFile = NULL;
	}
}

void BKEmulTapeRecorder::copyToBK( const char* from, WORD to, WORD size ) const
{
	for ( WORD i = 0; i < size; i++ ) {
		emul.setMemoryByte( to + i, *(from + i) );
	}
}

void BKEmulTapeRecorder::copyFromBK( WORD from, char* to, WORD size ) const
{
	for ( WORD i = 0; i < size; i++ ) {
		*to++ = emul.getMemoryByte( from + i );
	}
}

bool BKEmulTapeRecorder::isFileExists( const string& fileName )
{
	CFileFind finder;
	return finder.FindFile( fileName.c_str() ) ? true : false;
}

void BKEmulTapeRecorder::doEMT36()
{
	WORD memory  = emul.cpu.R1();
	BYTE command = emul.getMemoryByte( memory );
	// �������������� ����� ��������� �������� �� ������� ���������
	BYTE answer  = 4;
	WORD address = emul.getMemoryWord( memory+2 );
	WORD length  = emul.getMemoryWord( memory+4 );
	char name[17];
	copyFromBK( memory + 6, name, 16 );
	name[16] = 0;
	string file_name = name;

	char* buf = NULL;

	try {
		switch ( command ) {
			// ������� ��������� �����������
			// �������� ��������� ��� ������
			case 0: answer = 0; break;
			// ���� ��������� �����������
			// �������� ��������� ��� ������
			case 1: answer = 0; break;
			// ������ �����
			case 2: {
				bool canSave = true;
				if ( isFileExists( file_name ) ) {
					emul.setPause( true );
					std::string name = file_name;
					trim( name );
					canSave = AfxMessageBox( format( IDS_FILEEXISTS, name.c_str() ).c_str(), MB_ICONQUESTION | MB_YESNO ) == IDYES;
					emul.setPause( false );
				}
				if ( canSave ) {
					ofstream file( file_name.c_str(), ios_base::out | ios_base::binary );
					// ���������� ����� � ����� ����� � �����
					file.write( reinterpret_cast<char*>(&address), 2 );
					file.write( reinterpret_cast<char*>(&length), 2 );
					// ���������� �������� ���� �� ������ �� � ���������� ������ �� ����� ����������
					buf = new char[ length ];
					copyFromBK( address, buf, length );
					// ������������ ���� � �����
					file.write( buf, length );
					delete buf;
					buf = NULL;
					// �������� ��������� ��� ������
					answer = 0;
				} else {
					// ������� �� ������� ���������
					answer = 4;
				}
				break;
			}
			// ��������� ������ ����� (��������� � ������ ������ �����)
			case 4: file_name = "";
			// ������ �����
			case 3: {
				if ( isFileExists( file_name ) ) {
					ifstream file( file_name.c_str(), ios_base::in | ios_base::binary );
					WORD load_address;
					WORD load_length;
					file.read( reinterpret_cast<char*>(&load_address), 2 );
					file.read( reinterpret_cast<char*>(&load_length), 2 );
					// ����� �������� �������
					emul.setMemoryWord( memory + 22, load_address );
					// ����� �������� �������
					emul.setMemoryWord( memory + 24, load_length );
					// ��� �������� �������
					int len = 16 - file_name.length();
					for ( int i = 0; i < len; i++ ) {
						file_name += ' ';
					}
					copyToBK( file_name.c_str(), memory + 26, 16 );
					// ���� ����� ������� ����� ����������, �� ������ � �����, ����������� �� �����
					if ( address == 0 ) {
						address = load_address;
					}
					length = load_length;
					// ������ �� ������ � ���������� ������
					buf = new char[ length ];
					file.read( buf, length );
					// �������� �������� � ������ �� �� ����� ����������
					copyToBK( buf, address, length );
					// ��������� �������� ����������� ����������� EMT36, ������� ��
					// ��������� ����� ������ �������
					// ����� ������ �������
					emul.setMemoryWord( 0264, load_address );
					// ����� �������
					emul.setMemoryWord( 0266, load_length );
					// �������� ��������� ��� ������
					answer = 0;
					delete buf;
					buf = NULL;
				} else {
					// �� ������ ���� � ����� ������
					answer = 1;
					// ��������� ��������� �� ������ ��������� ���� � ����������,
					// ���� �� ��� �� ������ (���� ������� ����� �� callback-�������, �.�.
					// ����� ����� ����������� ���������� �� ���������� �������� �����
					// ������� ���������� �����)
					if ( last_file ) {
						last_file = false;
						// ��� ����� �����������, ���������� ���������� ����� ����
						closeFind();
						// ������� �� ������� ���������
						answer = 4;
					} else {
						bool canFind = true;
						if ( !findFile ) {
							// ����������� �����
							findFile = new CFileFind;
							char dir[ MAX_PATH + 1 ];
							::GetCurrentDirectory( MAX_PATH, dir );
							dir[ MAX_PATH ] = 0;
							string mask = dir;
							string::size_type pos = mask.find_last_of( '\\' );
							if ( pos == string::npos ) {
								pos = mask.find_last_of( '/' );
							}
							if ( pos != mask.length() - 1 ) {
								mask += '\\';
							}
							trimRight( file_name );
							mask += file_name + "*.*";
							canFind = findFile->FindFile( mask.c_str() ) ? true : false;
						}
						if ( canFind ) {
							// ������� ������/��������� ���� � ����������
							if ( findFile->FindNextFile() ) {
								bool flag = true;
								while ( flag && ( findFile->IsDots() || findFile->IsDirectory() ) ) {
									flag = findFile->FindNextFile() ? true : false;
								}
								if ( !findFile->IsDots() && !findFile->IsDirectory() ) {
									file_name = findFile->GetFileName();
								} else {
									// ��� ����� �����������, ���������� ���������� ����� ����
									closeFind();
									// ������� �� ������� ���������
									answer = 4;
								}
							} else {
								// ������� ��������� ���� � ���������
								if ( !findFile->IsDots() && !findFile->IsDirectory() ) {
									file_name = findFile->GetFileName();
									last_file = true;
								} else {
									// ��� ����� �����������, ���������� ���������� ����� ����
									closeFind();
									// ������� �� ������� ���������
									answer = 4;
								}
							}
						} else {
							// �� ����� ����� ������ ����
							closeFind();
							// ������� �� ������� ���������
							answer = 4;
						}
					}
					// �������� ����� ���������� �����
					if ( answer == 1 ) {
						// ��� �������� �������
						int len = 16 - file_name.length();
						for ( int i = 0; i < len; i++ ) {
							file_name += ' ';
						}
						copyToBK( file_name.c_str(), memory + 26, 16 );

						ifstream file( file_name.c_str(), ios_base::in | ios_base::binary );
						WORD load_address;
						WORD load_length;
						file.read( reinterpret_cast<char*>(&load_address), 2 );
						file.read( reinterpret_cast<char*>(&load_length), 2 );
						// ����� �������� �������
						emul.setMemoryWord( memory + 22, load_address );
						// ����� �������� �������
						emul.setMemoryWord( memory + 24, load_length );
					}
				}
				break;
			}
		}
		// ����� ����� ��� ���� �������
		// ���� ������
		emul.setMemoryByte( memory + 1, answer );
		// ����� ����� ������
		emul.setMemoryByte( 0301, answer );

		emul.cpu.BK_doRTI();

	} catch ( BKEmul::BKMemoryAccessError& /*e*/ ) {
		// ������ ������� � ������ �� (������� � ���)
		answer = 4;
		emul.setMemoryByte( memory + 1, answer );
		emul.setMemoryByte( 0301, answer );
		emul.cpu.BK_doRTI();
		if ( buf ) {
			delete buf;
			buf = NULL;
		}
		throw;
	}
}
