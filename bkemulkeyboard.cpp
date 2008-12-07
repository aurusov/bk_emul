#include "stdafx.h"
#include "bkemulkeyboard.h"
#include "bkemul.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace bkemul;

// --------------------------------------------------------------
// ---------- BKEmulKeyboard
// --------------------------------------------------------------
BKEmulKeyboard::BKEmulKeyboard()
{
}

BKEmulKeyboard::~BKEmulKeyboard()
{
	key_list.clear();
}

void BKEmulKeyboard::reset()
{
	RUS = Shift = AR2 = SU = KeyPressed = StopPressed = false;
	ZAGL = true;
	// 0100 = 1 - ������� ������
	// 0200 = 1 - ������� ���������� ��������������� ����������, ������ ���� = 1
	R_177716_byte_read = 0300;
	emul.memory.set_byte( 0177660, 0100 );
	key_list.clear();
}

// ���������, ���� �� ������� � ������ �������
bool BKEmulKeyboard::isKeyInList( char key ) const
{
	std::vector< char >::const_iterator it = key_list.begin();
	while ( it != key_list.end() ) {
		if ( key == *it++ ) {
			return true;
		}
	}
	return false;
}

// �������� ������� � ������ �������, ���� � ��� ��� ���
void BKEmulKeyboard::addToKeyList( char key )
{
	if ( !isKeyInList( key ) ) {
		key_list.push_back( key );
	}
}

// ������� ������� �� ������ �������
void BKEmulKeyboard::removeFromKeyList( char key )
{
	std::vector< char >::iterator it = key_list.begin();
	while ( it != key_list.end() ) {
		if ( key == *it ) {
			key_list.erase( it );
			break;
		}
		it++;
	}
}

bool BKEmulKeyboard::keyDown( UINT key, UINT flags )
{
	bool BK_sysKey = false;
	switch ( key ) {
		// Esc = ����
		case VK_ESCAPE:
			if ( !StopPressed ) {
				emul.cpu.setPR_4();
			}
			StopPressed = true;
			BK_sysKey   = true;
			return true;
		// Shift = ������ ������� (��������� ����� ���������)
		case VK_SHIFT:
			Shift     = true;
			BK_sysKey = true;
			return true;
		// Alt = ��2 (Alt-left) ��� �� (Alt-right)
		case VK_MENU:
			if ( flags & 0x1000000 ) {
				SU = true;
			} else {
				AR2 = true;
			}
			BK_sysKey = true;
			return true;
		// CapsLock = ����/���
		case VK_CAPITAL:
			ZAGL      = !ZAGL;
			BK_sysKey = true;
			return true;
	}
	// ���� �� ��������� ������� ��, � ������������ (� �� ������� IBM PC)
	BYTE BKScanCode;
	if ( !BK_sysKey && getBKScanCode( key, BKScanCode ) ) {
		addToKeyList( key );
		if ( !KeyPressed && key_list.size() == 1 ) {
			// ��������� ������� ������� (������ 177716, ������ 6, ��� 0100 = 0 - ������, 1 - ������)
			R_177716_byte_read &= 0277;
			KeyPressed = true;
			// Ctrl = ���/��� (��� = Ctrl-left / ��� = Ctrl-right)
			if ( key == VK_CONTROL ) {
				bool bit_24 = flags & 1 << 24 ? true : false;
				RUS = !bit_24;
				BKScanCode = bit_24 ? 017 : 016;
			}
			// ��������� ��� ������� �������
			emul.memory.set_byte( 0177662, BKScanCode );
			// �������� �� ���������� ��� ������� (������� 177660, ������ 7, ��� 0200 = 1 - � �������� ������ ���������� ���� ��� ������� �������, 0 - ���)
			if ( !(emul.memory.get_byte( 0177660 ) & 0200 ) ) {
				// � ������� ������ ���������� �������� ��� (������ 177660, ������ 7, ��� 0200 = 1 - ��������, 0 - ��������)
				emul.memory.set_byte( 0177660, emul.memory.get_byte( 0177660 ) | 0200 );
			}
			// ��������� �� ���������� � ���������� (������ 177660, ������ 6, ��� 0100 = 0 - ���������, 1 - ���������)
			if ( !(emul.getMemoryByte( 0177660 ) & 0100) && emul.cpu.getPrior() == 0 ) {
				if ( AR2 || BKScanCode <= 4 || BKScanCode == 013) {
					emul.cpu.setPR_274();
				} else {
					emul.cpu.setPR_60();
				}
			}
			return true;
		}
	}
	return false;
}

bool BKEmulKeyboard::keyUp( UINT key, UINT flags )
{
	removeFromKeyList( key );
	bool BK_sysKey = false;
	switch ( key ) {
		// Esc = ����
		case VK_ESCAPE:
			StopPressed = false;
			BK_sysKey   = true;
			return true;
		// Shift = ������ ������� (��������� ����� ���������)
		case VK_SHIFT:
			Shift     = false;
			BK_sysKey = true;
			return true;
		// Alt = ��2 (Alt-left) ��� �� (Alt-right)
		case VK_MENU:
			if ( flags & 0x1000000 ) {
				SU = false;
			} else {
				AR2 = false;
			}
			BK_sysKey = true;
			return true;
	}
	// ���� �� ��������� ������� ��, � ������������ (� �� ������� IBM PC)
	BYTE BKScanCode;
	if ( !BK_sysKey && getBKScanCode( key, BKScanCode ) ) {
		if ( KeyPressed && key_list.empty() ) {
			// ��������� ������� ������� (������ 177716, ������ 6, ��� 0100 = 0 - ������, 1 - ������)
			R_177716_byte_read |= 0100;
			KeyPressed = false;
			return true;
		}
	}
	return false;
}

bool BKEmulKeyboard::getBKScanCode( UINT PCKey, BYTE& BKScanCode )
{
	typedef struct {
		int  pc;
		BYTE bk;
	} KeyTable;

	const int control_key_count   = 45;
	const int table_key_count     = 35;
	const int shift_key_eng_count = 21;
	const int shift_key_rus_count = 15;

	static KeyTable table_cnt[control_key_count] = {
		{VK_F6     , 0000}, // ���
		{VK_F1     , 0001}, // ����
		{VK_F4     , 0002}, // ��� ��
		{VK_F2     , 0003}, // ��
		{VK_F5     , 0004}, // ���� ���
		{VK_LEFT   , 0010}, // �����
		{VK_NUMPAD4, 0010}, // �����
		{VK_RETURN , 0012}, // ����
		{VK_F3     , 0013}, // =|==>
		{VK_F7     , 0014}, // ���
		{0         , 0015}, // ��������� ���������
		{VK_CONTROL, 0016}, // ��� // ��� � ��� �������������� � �������� ����� ��������� ������.
		{VK_CONTROL, 0017}, // ��� // �� GetBKScanCode ������ ������� true, �.�. ��� ������������ ������� �� (�� ����, ��2 � ��.)
		{0         , 0020}, // ����� ���������
		{VK_NUMPAD5, 0022}, // �������� ��������� �������
		{VK_HOME   , 0023}, // ������� ������� � ������ ������� ������
		{VK_TAB    , 0024}, // ���
		{0         , 0025}, // ����������� ������� � ������ ��������� ������
		{VK_DELETE , 0026}, // |<===
		{VK_INSERT , 0027}, // |===>
		{VK_BACK   , 0030}, // <=== �����
		{VK_RIGHT  , 0031}, // ������
		{VK_NUMPAD6, 0031}, // ������
		{VK_UP     , 0032}, // �����
		{VK_NUMPAD8, 0032}, // �����
		{VK_DOWN   , 0033}, // ����
		{VK_NUMPAD2, 0033}, // ����
		{VK_NUMPAD7, 0034}, // �����-�����
		{VK_NUMPAD9, 0035}, // �����-������
		{VK_NUMPAD3, 0036}, // ����-������
		{VK_NUMPAD1, 0037}, // ����-�����
		{VK_SPACE  , 0040}, // ������
		{0x30      , 0060}, // 0
		{0x31      , 0061}, // 1
		{0x32      , 0062}, // 2
		{0x33      , 0063}, // 3
		{0x34      , 0064}, // 4
		{0x35      , 0065}, // 5
		{0x36      , 0066}, // 6
		{0x37      , 0067}, // 7
		{0x38      , 0070}, // 8
		{0x39      , 0071}, // 9
		{0xBD      , 0055}, // -
		{0xBB      , 0075}, // =
	};

	static KeyTable table_eng[table_key_count] = {
		{0x41      , 0141}, // a
		{0x42      , 0142}, // b
		{0x43      , 0143}, // c
		{0x44      , 0144}, // d
		{0x45      , 0145}, // e
		{0x46      , 0146}, // f
		{0x47      , 0147}, // g
		{0x48      , 0150}, // h
		{0x49      , 0151}, // i
		{0x4A      , 0152}, // j
		{0x4B      , 0153}, // k
		{0x4C      , 0154}, // l
		{0x4D      , 0155}, // m
		{0x4E      , 0156}, // n
		{0x4F      , 0157}, // o
		{0x50      , 0160}, // p
		{0x51      , 0161}, // q
		{0x52      , 0162}, // r
		{0x53      , 0163}, // s
		{0x54      , 0164}, // t
		{0x55      , 0165}, // u
		{0x56      , 0166}, // v
		{0x57      , 0167}, // w
		{0x58      , 0170}, // x
		{0x59      , 0171}, // y
		{0x5A      , 0172}, // z
		{0xC0      , 0140}, // `
		{0xDC      , 0134}, // '\'
		{0xDB      , 0133}, // [
		{0xDD      , 0135}, // ]
		{0xBA      , 0073}, // ;
		{0xDE      , 0047}, // '
		{0xBC      , 0054}, // ,
		{0xBE      , 0056}, // .
		{0xBF      , 0057}, // /
	};

	static KeyTable table_eng_zagl[table_key_count] = {
		{0x41      , 0101}, // A
		{0x42      , 0102}, // B
		{0x43      , 0103}, // C
		{0x44      , 0104}, // D
		{0x45      , 0105}, // E
		{0x46      , 0106}, // F
		{0x47      , 0107}, // G
		{0x48      , 0110}, // H
		{0x49      , 0111}, // I
		{0x4A      , 0112}, // J
		{0x4B      , 0113}, // K
		{0x4C      , 0114}, // L
		{0x4D      , 0115}, // M
		{0x4E      , 0116}, // N
		{0x4F      , 0117}, // O
		{0x50      , 0120}, // P
		{0x51      , 0121}, // Q
		{0x52      , 0122}, // R
		{0x53      , 0123}, // S
		{0x54      , 0124}, // T
		{0x55      , 0125}, // U
		{0x56      , 0126}, // V
		{0x57      , 0127}, // W
		{0x58      , 0130}, // X
		{0x59      , 0131}, // Y
		{0x5A      , 0132}, // Z
		{0xC0      , 0140}, // `
		{0xDC      , 0134}, // '\'
		{0xDB      , 0133}, // [
		{0xDD      , 0135}, // ]
		{0xBA      , 0073}, // ;
		{0xDE      , 0047}, // '
		{0xBC      , 0054}, // ,
		{0xBE      , 0056}, // .
		{0xBF      , 0057}, // /
	};

	static KeyTable table_rus[table_key_count] = {
		{0xBE      , 0100}, // �
		{0x46      , 0101}, // �
		{0xBC      , 0102}, // �
		{0x57      , 0103}, // �
		{0x4C      , 0104}, // �
		{0x54      , 0105}, // �
		{0x41      , 0106}, // �
		{0x55      , 0107}, // �
		{0xDB      , 0110}, // �
		{0x42      , 0111}, // �
		{0x51      , 0112}, // �
		{0x52      , 0113}, // �
		{0x4B      , 0114}, // �
		{0x56      , 0115}, // �
		{0x59      , 0116}, // �
		{0x4A      , 0117}, // �
		{0x47      , 0120}, // �
		{0x5A      , 0121}, // �
		{0x48      , 0122}, // �
		{0x43      , 0123}, // �
		{0x4E      , 0124}, // �
		{0x45      , 0125}, // �
		{0xBA      , 0126}, // �
		{0x44      , 0127}, // �
		{0x4D      , 0130}, // �
		{0x53      , 0131}, // �
		{0x50      , 0132}, // �
		{0x49      , 0133}, // �
		{0xDE      , 0134}, // �
		{0x4F      , 0135}, // �
		{0x58      , 0136}, // �
		{0xDD      , 0137}, // �
		{0xDC      , 0057}, // /
		{0xC0      , 0047}, // '
		{0xBF      , 0056}, // .
	};

	static KeyTable table_rus_zagl[table_key_count] = {
		{0xBE      , 0140}, // �
		{0x46      , 0141}, // �
		{0xBC      , 0142}, // �
		{0x57      , 0143}, // �
		{0x4C      , 0144}, // �
		{0x54      , 0145}, // �
		{0x41      , 0146}, // �
		{0x55      , 0147}, // �
		{0xDB      , 0150}, // �
		{0x42      , 0151}, // �
		{0x51      , 0152}, // �
		{0x52      , 0153}, // �
		{0x4B      , 0154}, // �
		{0x56      , 0155}, // �
		{0x59      , 0156}, // �
		{0x4A      , 0157}, // �
		{0x47      , 0160}, // �
		{0x5A      , 0161}, // �
		{0x48      , 0162}, // �
		{0x43      , 0163}, // �
		{0x4E      , 0164}, // �
		{0x45      , 0165}, // �
		{0xBA      , 0166}, // �
		{0x44      , 0167}, // �
		{0x4D      , 0170}, // �
		{0x53      , 0171}, // �
		{0x50      , 0172}, // �
		{0x49      , 0173}, // �
		{0xDE      , 0174}, // �
		{0x4F      , 0175}, // �
		{0x58      , 0176}, // �
		{0xDD      , 0177}, // �
		{0xDC      , 0057}, // /
		{0xC0      , 0047}, // '
		{0xBF      , 0056}, // .
	};

	static KeyTable table_shift_eng[shift_key_eng_count] = {
		{0xC0      , 0176}, // ~
		{0x31      , 0041}, // !
		{0x32      , 0100}, // @
		{0x33      , 0043}, // #
		{0x34      , 0044}, // $
		{0x35      , 0045}, // %
		{0x36      , 0136}, // ^
		{0x37      , 0046}, // &
		{0x38      , 0052}, // *
		{0x39      , 0050}, // (
		{0x30      , 0051}, // )
		{0xBD      , 0137}, // _
		{0xBB      , 0053}, // +
		{0xDC      , 0174}, // |
		{0xDB      , 0173}, // {
		{0xDD      , 0175}, // }
		{0xBA      , 0072}, // :
		{0xDE      , 0042}, // "
		{0xBC      , 0074}, // <
		{0xBE      , 0076}, // >
		{0xBF      , 0077}  // ?
	};

	static KeyTable table_shift_rus[shift_key_rus_count] = {
		{0x31      , 0041}, // !
		{0x32      , 0042}, // "
		{0xC0      , 0042}, // "
		{0x33      , 0043}, // #
		{0x34      , 0073}, // ;
		{0x35      , 0045}, // %
		{0x36      , 0072}, // :
		{0x37      , 0077}, // ?
		{0x38      , 0052}, // *
		{0x39      , 0050}, // (
		{0x30      , 0051}, // )
		{0xBB      , 0053}, // +
		{0xDC      , 0057}, // /
		{0xBF      , 0054}, // ,
		{0xBD      , 0055}, // -
	};

	KeyTable* table;
	int count;
	if ( Shift ) {
		if ( RUS ) {
			table = table_shift_rus;
			count = shift_key_rus_count;
		} else {
			table = table_shift_eng;
			count = shift_key_eng_count;
		}
	} else {
		table = table_cnt;
		count = control_key_count;
	}
	int i;
	for ( i = 0; i < count; i++ ) {
		if ( table[i].pc == PCKey ) {
			BKScanCode = table[i].bk;
			if ( SU ) {
				BKScanCode &= 037;
			}
			return true;
		}
	}
	if ( RUS ) {
		if ( ZAGL ) {
			table = Shift ? table_rus : table_rus_zagl;
		} else {
			table = Shift ? table_rus_zagl : table_rus;
		}
	} else {
		if ( ZAGL ) {
			table = Shift ? table_eng : table_eng_zagl;
		} else {
			table = Shift ? table_eng_zagl : table_eng;
		}
	}
	for ( i = 0; i < table_key_count; i++ ) {
		if ( table[i].pc == PCKey ) {
			BKScanCode = table[i].bk;
			if ( SU ) {
				BKScanCode &= 037;
			}
			return true;
		}
	}
	return false;
}
